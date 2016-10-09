#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <QObject>

class TimeSlot: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString slotUiId READ getTimeSlotUiId NOTIFY timeSlotChanged)
    // Param 1 QML:kutsu, Param 2: mistä luetaan, Paran 3: milloin luetaan
    Q_PROPERTY(QString getTimeFromSlot READ getTimeSlotTime NOTIFY timeSlotChanged)
    Q_PROPERTY(QString getTimeSlotState READ getTimeSlotState NOTIFY timeSlotChanged )
    Q_PROPERTY(bool isRunning READ isRunning WRITE setRunning NOTIFY timeSlotChanged)
public:
    TimeSlot();

    void setTimeSlotUiId(int uiId);
    int getTimeSlotUiId() const;

    void setTimeSlotOrder(int order);
    int getTimeSlotOrder() const;

    void setTimeSlotState(QString state);
    QString getTimeSlotState();

    void setTimeSlotTime(QString time);
    QString getTimeSlotTime() const;

    void setRunning(bool input);
    bool isRunning();



Q_SIGNALS:
    void timeSlotChanged();
    void runningChanged();
private:

    enum eSlotStates{
        eNeutral,
        eRunning,
        eStarted,
        eStopped,
        eUnknown
    };

    eSlotStates parseTimeSlotState(QString state);
    int timeSlotUiId;
    QString instanceTime;
    int instanceOrder;
    bool running;
    eSlotStates timeSlotState;
};

#endif // TIMESLOT_H
