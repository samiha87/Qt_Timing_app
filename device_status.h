#ifndef DEVICE_STATUS_H
#define DEVICE_STATUS_H

#include <QObject>
/*! \brief this class contains status of mt device
 *
 */

class DeviceStatus
{
public:
    /*! \brief constructor of DeviceStatus
     */
    DeviceStatus();
    /*! \brief Set battery level
     * \param QString, battery level
     */
    void setBattery(QString input);
    /*! \brief Get battery level
     * \return QString, battery level
     */
    QString getBattery();

    void setType(QString type);
    QString getType();
private:
    QString batteryLevel;
    QString deviceType;
};

#endif // DEVICE_STATUS_H
