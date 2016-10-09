#ifndef TIMEMANAGAMENT_H
#define TIMEMANAGAMENT_H

#include <QObject>
#include <QVariant>

#include "timeslot.h"
#include "device_status.h"

class TimeManagement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant timeSlotListQml READ getTimeSlots NOTIFY timeSlotsUpdated)
    Q_PROPERTY(QVariant getStartActive READ getStartState NOTIFY deviceStateChanged)
    Q_PROPERTY(QVariant getSplitActive READ getSplitState NOTIFY deviceStateChanged)
    Q_PROPERTY(QVariant getEndActive READ getEndState NOTIFY deviceStateChanged)
    Q_PROPERTY(QVariant getMasterActive READ getMasterState NOTIFY deviceStateChanged)
    Q_PROPERTY(QVariant battery READ getBattery NOTIFY batteryChanged)
    Q_PROPERTY(QVariant type READ getType NOTIFY deviceStateChanged)

public:
    explicit TimeManagement(QObject* parent = 0);

    enum DeviceTypes {
        mtStart,
        mtSplit,
        mtEnd,
        mtMaster,
        mtUnknown
    };

    ~TimeManagement();
    QVariant getTimeSlots();
    QVariant getBattery();
    QVariant getType();

    void setTime(QString time, int slot);
    QString getTime(int slot);
    void createTimeSlots(int amount);
    void addTimeSlot();
    void addTime(QString input);

    void setTime(int slot, QString time);
    void setTimeState(int slot, QString state);
    void setDeviceType(DeviceTypes types);

    bool getStartState();
    bool getSplitState();
    bool getEndState();
    bool getMasterState();

public slots:
    void parseMessage(QString msg);

Q_SIGNALS:
     void timeSlotsUpdated();
     void deviceStateChanged();
     void batteryChanged();

private:
     QList<QObject*> timeSlotList;
     TimeSlot *timeSlot;
     DeviceStatus deviceStatus;

     bool startActive;
     bool splitActive;
     bool endActive;
     bool masterActive;

     QString timeState;

};
#endif // TIMEMANAGAMENT_H
