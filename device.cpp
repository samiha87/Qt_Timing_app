/***************************************************************************
**
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QTimer>

#include "device.h"
#include "mt_device_defines.h"


Device::Device():
    connected(false), controller(0), m_deviceScanState(false), randomAddress(false)
{
    //! [les-devicediscovery-1]
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
    connect(discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(deviceScanFinished()));
    //! [les-devicediscovery-1]

    connect(&quickSearchTimer, SIGNAL(timeout()),this, SLOT(deviceScanFinished()));
    tx = 0;
    transmitService = 0;
    transmitPointer = 0;
    searchParams = null;
    setUpdate("Search");
}

Device::~Device()
{
    delete discoveryAgent;
    delete controller;
    qDeleteAll(devices);
    qDeleteAll(m_services);
    qDeleteAll(m_characteristics);
    devices.clear();
    m_services.clear();
    m_characteristics.clear();
}

void Device::startDeviceDiscovery(QString input)
{
    qDeleteAll(devices);
    qDeleteAll(m_services);
    qDeleteAll(m_characteristics);
    devices.clear();
    m_services.clear();
    m_characteristics.clear();

    emit devicesUpdated();
    searchParams = input;
    if(input == "one") {
        setUpdate("Scanning for MT device");
        quickSearchTimer.start(3000);
    } else if (input == "all") {
        setUpdate("Scanning for MT devices");
    }
    discoveryAgent->start();

    if (discoveryAgent->isActive()) {
        m_deviceScanState = true;
        Q_EMIT qstateChanged();
    }
}

void Device::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        DeviceInfo *d = new DeviceInfo(info);
        if(d->getName().startsWith("MT") || d->getName().startsWith("HMSoft")) {
            setUpdate("Found device: " + d->getName());
            devices.append(d);
            emit devicesUpdated();
        }
    }
}

void Device::deviceScanFinished()
{
    emit devicesUpdated();
    quickSearchTimer.stop();
    m_deviceScanState = false;
    emit qstateChanged();
    if (devices.isEmpty())
        setUpdate("No MT devices found...");
    else
        setUpdate("Finished");
}

QVariant Device::getDevices()
{
    return QVariant::fromValue(devices);
}

QVariant Device::getServices()
{
    return QVariant::fromValue(m_services);
}

QVariant Device::getCharacteristics()
{
    return QVariant::fromValue(m_characteristics);

}

QString Device::getUpdate()
{
    return m_message;
}

void Device::scanServices(const QString &address)
{
    // We need the current device for service discovery.

    for (int i = 0; i < devices.size(); i++) {
        if (((DeviceInfo*)devices.at(i))->getAddress() == address )
            currentDevice.setDevice(((DeviceInfo*)devices.at(i))->getDevice());
    }

    if (!currentDevice.getDevice().isValid()) {
        qWarning() << "Not a valid device";
        return;
    }

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();
    qDeleteAll(m_services);
    m_services.clear();
    emit servicesUpdated();

    setUpdate("Back\n(Connecting to MT...)");
    //if(controller == 0) return;

    if (controller && m_previousAddress != currentDevice.getAddress()) {
        controller->disconnectFromDevice();
        delete controller;
        controller = 0;
    }

    if (!controller) {
        controller = new QLowEnergyController(currentDevice.getDevice());
        connect(controller, SIGNAL(connected()),
                this, SLOT(deviceConnected()));
        connect(controller, SIGNAL(error(QLowEnergyController::Error)),
                this, SLOT(errorReceived(QLowEnergyController::Error)));
        connect(controller, SIGNAL(disconnected()),
                this, SLOT(deviceDisconnected()));
        connect(controller, SIGNAL(serviceDiscovered(QBluetoothUuid)),
                this, SLOT(addLowEnergyService(QBluetoothUuid)));
        connect(controller, SIGNAL(discoveryFinished()),
                this, SLOT(serviceScanDone()));
    }

    if (isRandomAddress())
        controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    else
        controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    controller->connectToDevice();
    m_previousAddress = currentDevice.getAddress();
}

void Device::addLowEnergyService(const QBluetoothUuid &serviceUuid)
{
    QLowEnergyService *service = controller->createServiceObject(serviceUuid);
    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }
    ServiceInfo *serv = new ServiceInfo(service);
    m_services.append(serv);
    if(serv->isTransfer()) {
        // Search characteristics and connect
        connectToService(serv->getUuid());
    }
    emit servicesUpdated();
}

void Device::serviceScanDone()
{
    setUpdate("Back\n(Service scan done!)");
    // force UI in case we didn't find anything
    if (m_services.isEmpty())
        emit servicesUpdated();
}

void Device::connectToService(const QString &uuid)
{
    QLowEnergyService *service = 0;
    for (int i = 0; i < m_services.size(); i++) {
        ServiceInfo *serviceInfo = (ServiceInfo*)m_services.at(i);
        if (serviceInfo->getUuid() == uuid) {
            service = serviceInfo->service();
            break;
        }
    }

    if (!service)
        return;


    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();

    if (service->state() == QLowEnergyService::DiscoveryRequired) {
        connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                this, SLOT(serviceDetailsDiscovered(QLowEnergyService::ServiceState)));
        service->discoverDetails();
        setUpdate("Back\n(Discovering details...)");
        return;
    }

    //discovery already done
    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    foreach (const QLowEnergyCharacteristic &ch, chars) {
        CharacteristicInfo *cInfo = new CharacteristicInfo(ch);
        m_characteristics.append(cInfo);
    }

    QTimer::singleShot(0, this, SIGNAL(characteristicsUpdated()));
}

void Device::deviceConnected()
{
    setUpdate("Back\n(Discovering services...)");
    connected = true;
    controller->discoverServices();
}

void Device::errorReceived(QLowEnergyController::Error /*error*/)
{
    qWarning() << "Error: " << controller->errorString();
    setUpdate(QString("Back\n(%1)").arg(controller->errorString()));
}

void Device::setUpdate(QString message)
{
    m_message = message;
    emit updateChanged();
}

void Device::disconnectFromMtGate()
{
    //transmitData("h!d");
    QTimer::singleShot(300, this, SLOT(disconnectFromDevice()));
}

void Device::disconnectFromDevice()
{
    // UI always expects disconnect() signal when calling this signal
    // TODO what is really needed is to extend state() to a multi value
    // and thus allowing UI to keep track of controller progress in addition to
    // device scan progress
    // Disable communication from MT system

        //disabled notifications -> assume disconnect intent
    if(transmitService != 0) {
        delete transmitService;
        transmitService = 0;
        transmitPointer = 0;
    }

    if (controller->state() != QLowEnergyController::UnconnectedState)
        controller->disconnectFromDevice();
    else
        deviceDisconnected();
}

void Device::deviceDisconnected()
{
    qWarning() << "Disconnect from device";
    emit disconnected();
}


void Device::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered) {
        // do not hang in "Scanning for characteristics" mode forever
        // in case the service discovery failed
        // We have to queue the signal up to give UI time to even enter
        // the above mode
        if (newState != QLowEnergyService::DiscoveringServices) {
            QMetaObject::invokeMethod(this, "characteristicsUpdated",
                                      Qt::QueuedConnection);
        }
        return;
    }

    QLowEnergyService *service = qobject_cast<QLowEnergyService *>(sender());
    if (!service)
        return;

    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    foreach (const QLowEnergyCharacteristic &ch, chars) {
        CharacteristicInfo *cInfo = new CharacteristicInfo(ch);
        m_characteristics.append(cInfo);
        // Check

        for(int i = 0; i < service->characteristics().count(); i++) {
            qDebug() << service->serviceUuid();
            if(service->serviceUuid().toString().contains("0000ffe0") ) {
                if(transmitService != 0) {
                    delete transmitService;
                    transmitService = 0;
                    transmitPointer = 0;
                }
                transmitService = service;
                transmitPointer = i;

                // enable notifications, required for two way communication

                const QLowEnergyCharacteristic hrChar = transmitService->characteristics().at(transmitPointer);

                if (!hrChar.isValid()) {
                    break;
                }

                QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(
                            QBluetoothUuid::ClientCharacteristicConfiguration);
                if (m_notificationDesc.isValid()) {
                    // Enable notifications
                    transmitService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
                }

                connect(transmitService, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serialStateChanged(QLowEnergyService::ServiceState)));
                connect(transmitService, SIGNAL(characteristicChanged(QLowEnergyCharacteristic, QByteArray)), this, SLOT(serialReadValue(QLowEnergyCharacteristic,QByteArray)));
                connect(transmitService, SIGNAL(descriptorWritten(QLowEnergyDescriptor, QByteArray)), this, SLOT(serialDescriptorWrite(QLowEnergyDescriptor,QByteArray)));
                setUpdate("Back");
                qDebug() << "Transmit Service set";
            }
        }
    }
    emit characteristicsUpdated();
}

void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        setUpdate("The Bluetooth adaptor is powered off, power it on before doing discovery.");
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        setUpdate("Writing or reading from the device resulted in an error.");
    else
        setUpdate("An unknown error has occurred.");

    m_deviceScanState = false;
    emit devicesUpdated();
    emit qstateChanged();
}

bool Device::state()
{
    return m_deviceScanState;
}
// 00002a05-0000-1000-8000-00805f9b34fb for service 00001801-0000-1000-8000-00805f9b34fb
bool Device::hasControllerError() const
{
    if (controller && controller->error() != QLowEnergyController::NoError)
        return true;
    return false;
}

bool Device::isRandomAddress() const
{
    return randomAddress;
}

void Device::setRandomAddress(bool newValue)
{
    randomAddress = newValue;
    emit randomAddressChanged();
}

void Device::transmitData(QString cmd)
{
    if(transmitService == 0) {
        qDebug() << "transmitService not found";
        return;
    }

    QByteArray msg;
    msg.append('#');    // Start byte
    msg.append('b');    // Bluetooth message
    msg.append(cmd.toLocal8Bit());  //
    msg.append('*');    // End byte
    transmitService->writeCharacteristic(transmitService->characteristics().at(transmitPointer), msg, QLowEnergyService::WriteWithoutResponse);
}

void Device::readData()
{
    if(transmitService == 0) {
        qDebug() << "transmitService not found";
        return;
    }

    for(int y = 0; y < transmitService->characteristics().count(); y++) {
        transmitService->readCharacteristic(transmitService->characteristics().at(transmitPointer));
        qDebug() << transmitService->characteristics().at(y).value();
    }
}

void Device::serialStateChanged(QLowEnergyService::ServiceState s)
{
    //qDebug() << "Serial state changed";
    switch (s) {
    case QLowEnergyService::ServiceDiscovered:
    {
        const QLowEnergyCharacteristic hrChar = transmitService->characteristics().at(transmitPointer);
        if (!hrChar.isValid()) {
            qDebug() << "Data not found";
            break;
        }

        const QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(
                    QBluetoothUuid::ClientCharacteristicConfiguration);
        if (m_notificationDesc.isValid()) {
            transmitService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
           // qDebug() << "Receiving enabled";
        }

        break;
    }
    default:
        //nothing for now
        break;
    }
}

void Device::serialReadValue(const QLowEnergyCharacteristic &c,
                                     const QByteArray &value)
{
    // ignore any other characteristic change -> shouldn't really happen though
   // qDebug() << "Serial read value: " << value;
    if (c.uuid().toString().contains("0000ffe0"))
        return;

    //const char *data = value.constData();
    QByteArray startByte = "#";
    QByteArray endByte = "*";

    if(value.contains(startByte) && value.contains(endByte)) {
       // qDebug() << "Device::serialReadValue, whole message";
        mtMessage.clear();
        QByteArray temp;
        temp = value;
        int start = temp.indexOf(startByte);
        int end = temp.indexOf(endByte);
        temp = temp.mid( (start + 1), (end ));
        mtMessage.append(temp);
        emit mtMessageReceived(mtMessage);
        //qDebug() << "Device::serialReadValue(): Message " << mtMessage;
        return;

    } else if(value.contains(startByte) && !enableMessageReading) {
        mtMessage.clear();
        QByteArray temp;
        temp = value;
        int start = temp.indexOf(startByte);
        temp.remove(0, (start + 1) );
        mtMessage.append(temp);
        //qDebug() << "Device::serialReadValue, start byte";
        enableMessageReading = true;
    } else if(enableMessageReading && value.contains(endByte)) {
        //qDebug() << "Device::serialReadValue, end byte";
        enableMessageReading = false;   // Disable message reading
        QByteArray temp;
        temp = value;
        int end = temp.indexOf(endByte);
        temp = temp.mid(0, (end));
        mtMessage.append(temp);
        emit mtMessageReceived(mtMessage);
        //qDebug() << "Device::serialReadValue(): Message " << mtMessage;
    } else if(enableMessageReading){
        mtMessage.append(value);
    }
}

void Device::serialDescriptorWrite(const QLowEnergyDescriptor &d,
                                         const QByteArray &value)
{
    // Disconnect from device
    const QLowEnergyCharacteristic hrChar = transmitService->characteristics().at(transmitPointer);

    if (!hrChar.isValid()) {
        return;
    }

    QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);

    if (d.isValid() && d == m_notificationDesc && value == QByteArray("0000")) {
        //disabled notifications -> assume disconnect intent
        controller->disconnectFromDevice();
        if(transmitService != 0) {
            delete transmitService;
            transmitPointer = 0;
        }
    }
}

/* Set device type
 * ! = hex type, S = string types
 * Checkbyte, Command, target, device type
 * f = commandDeviceTypeHex
 * l = Target local device, t = target remote device
 * b = Device type start, s = Device type split, e = Device type end
 *
 */
void Device::setDeviceType(QString type)
{
    // Format

    if(type == "Start") {
        transmitData("S!flb");
    } else if (type == "Split") {
        transmitData("S!fls");
    } else if (type == "End") {
        transmitData("S!fle");
    } else if (type == "Master") {
        transmitData("S!flm");
    }
}

QString Device::getDeviceType()
{
    return "Unknown";
}


