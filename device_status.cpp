#include "device_status.h"

DeviceStatus::DeviceStatus()
{
    batteryLevel = "0%";
}

void DeviceStatus::setBattery(QString input)
{
    batteryLevel = "Battery-" + input + "%";
}

QString DeviceStatus::getBattery()
{
    return batteryLevel;
}

void DeviceStatus::setType(QString type)
{
    deviceType = "Type: " + type;
}

QString DeviceStatus::getType()
{
    return deviceType;
}
