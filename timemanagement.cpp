#include <QStringList>
#include <QDebug>

#include "timemanagement.h"
#include "device_status.h"

// TODO change this page as devicePage
TimeManagement::TimeManagement(QObject *parent):  QObject(parent)
{
    startActive = true;
    splitActive = false;
    endActive = false;
    deviceStateChanged();
}

TimeManagement::~TimeManagement()
{
    int count = timeSlotList.count();
    for(int i = 0; i < count; i++) {
        TimeSlot *toRemove = (TimeSlot *)timeSlotList.at(i);
        toRemove->deleteLater();
    }
    timeSlotList.clear();
}

QVariant TimeManagement::getTimeSlots()
{
    return QVariant::fromValue(timeSlotList);
}

QVariant TimeManagement::getBattery()
{
    return QVariant(deviceStatus.getBattery());
}

QVariant TimeManagement::getType()
{
    return QVariant(deviceStatus.getType());
}

void TimeManagement::createTimeSlots(int amount)
{
    for(int i = 0; i < amount; i++) {
        timeSlot = new TimeSlot();
        timeSlot->setTimeSlotUiId(i);
        timeSlotList.append(timeSlot);
    }
    emit timeSlotsUpdated();
}

void TimeManagement::addTimeSlot()
{
    timeSlot = new TimeSlot();
    timeSlot->setTimeSlotUiId(timeSlotList.count() + 1);
    timeSlotList.append(timeSlot);
    emit timeSlotsUpdated();
}

void TimeManagement::setTime(int slot, QString time)
{

    if(timeSlotList.count() < slot) {
        return;
    }

    TimeSlot *ts;
    ts = (TimeSlot *)timeSlotList.at(slot);
    ts->setTimeSlotTime(time);
    emit timeSlotsUpdated();
}

void TimeManagement::setTimeState(int slot, QString state)
{
    if(timeSlotList.count() < slot) {
        return;
    }

    TimeSlot *ts;
    ts = (TimeSlot *)timeSlotList.at(slot);
    ts->setTimeSlotState(state);
    emit timeSlotsUpdated();
}

void TimeManagement::addTime(QString input)
{
    // Parse message here
    Q_UNUSED(input);
    emit timeSlotsUpdated();
}

// TODO make a class messageManager
/*! \patterns
 * "#MT,GT,UI,0,00:00:00,c,1,1,00:00:00,c,2,2,00:00:00,c,3,3,00:00:00,c,4,4,00:00:00,c,5,5,00:00:00,c,6,6,00:00:00,c,7,start;;"
 * "#MT,GT,BATTERY,C67;;"
 * "#MT,GT,BATTERY,67;;"
 * "#MT,GT,DEBUG,<debug message>;;"
 */
void TimeManagement::parseMessage(QString msg)
{
    //qDebug() << "TimeManagement::parseMessage() " << msg;

    QByteArray endByte = "*";

    if(!msg.contains(",")) return;
    QStringList message = msg.split(",");

    if(message.count() > 0 && message.at(0) == "MT" ) {
    } else {
        //qDebug() << "TimeManagement::parseMessage: invalid message";
        return;
    }

    if( (message.count() > 1) && (message.at(1) == "GT") ) {

    } else {
       // qDebug() << "TimeManagement::parseMessage: invalid message";
        return;
    }

    if(message.count() > 2 && message.at(2) == "UI") {
        for (int i = 3; i < message.count(); i += 4) {
           // qDebug() << "TimeManagement::parseMessage(): slot: " << message.at(i);
            if((i + 1) < message.count()) {
                setTime(message.at(i).toInt(), message.at(i + 1));
               // qDebug() << "TimeManagement::parseMessage(): time: " << message.at(i + 1);
            }
            if((i + 2) < message.count()) {
                //setTime(message.at(i).toInt(), message.at(i + 1));
                setTimeState(message.at(i).toInt(), message.at(i+2));
                if(message.at(i + 2) == "x") {
                    setTime(message.at(i).toInt(), "SKIP");
                } else if (message.at(i + 2) == "r") {
                    //setRunning(true);
                }
              //  qDebug() << "TimeManagement::parseMessage(): timing state: " << message.at(i + 2);
            }
            if((i + 3) < message.count()) {

            }

            // Get device type
            if((i + 4) < message.count()) {
                if(message.at(i+4) != deviceStatus.getType()) {
                    deviceStatus.setType(message.at((i+4)));
                    emit deviceStateChanged();
                }
            }
        }
    } else if(message.count() > 3 && message.at(2) == "DB") {
       // qDebug() << "TimeManagement::Db_incoming";
    } else if (message.count() >=3 && message.at(2) == "BATTERY") {
        //qDebug() << "TimeManagement::parseMessage(): Battery:" << msg;
        QString alter;
        alter = message.at(3);
        if(alter.contains(endByte)) {
            alter.replace(QString(endByte),QString("  "));
        }
        deviceStatus.setBattery(alter.simplified());
        emit batteryChanged();
    } else if ( message.count() > 3 && message.at(2) == "DEBUG") {

        qDebug() << "TimeManagement::parseMessage(): Debug: " << msg;
    }
}

void TimeManagement::setDeviceType(DeviceTypes types)
{
    switch(types) {
        case mtStart:
            startActive = true;
            splitActive = false;
            endActive = false;
        break;
    case mtSplit:
            startActive = false;
            splitActive = true;
            endActive = false;
        break;
    case mtEnd:
            startActive = false;
            splitActive = true;
            endActive = false;
        break;
    case mtMaster:
            startActive = false;
            splitActive = false;
            endActive = false;
            masterActive = true;
        break;
    case mtUnknown:
            startActive = true;
            splitActive = false;
            endActive = false;
        break;
    }
}

bool TimeManagement::getStartState()
{
    return startActive;
}

bool TimeManagement::getSplitState()
{
    return splitActive;
}

bool TimeManagement::getEndState()
{
    return endActive;
}

bool TimeManagement::getMasterState()
{
    return masterActive;
}
