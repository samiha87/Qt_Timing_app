#include "wifihandler.h"

WiFiHandler::WiFiHandler(Device *device)
{
    dev = device;
}

bool WiFiHandler::requestNetworks() {
    QByteArray transmit = {0xA0, 0xA2}; // Search for connections
    dev->transmitData(transmit);
    return true;
}

bool WiFiHandler::connectToNetwork(QString ssid, QString password) {
    QByteArray transmit = {0xA0, 0xA1};
    transmit += ssid.toUtf8();
    transmit += "::";
    transmit += password.toUtf8();
    dev->transmitData(transmit);
    return true;
}
