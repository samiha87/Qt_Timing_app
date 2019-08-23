#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <QObject>
#include "device.h"

class WiFiHandler
{
public:
    WiFiHandler(Device *device);

    bool requestNetworks();
    bool connectToNetwork(QString ssid, QString password);

private:
    Device *dev;

};

#endif // WIFIHANDLER_H
