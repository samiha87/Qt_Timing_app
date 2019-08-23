#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <QObject>
#include "memoryhandler.h"

class TimeSlot: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString slotUiId READ getTimeSlotUiId NOTIFY timeSlotChanged)
    // Param 1 QML:kutsu, Param 2: mistä luetaan, Paran 3: milloin luetaan
    Q_PROPERTY(QString getTimeFromSlot READ getTimeSlotTime NOTIFY timeSlotChanged)
     Q_PROPERTY(QString getParticipant READ getParticipant NOTIFY timeSlotChanged)
    Q_PROPERTY(QString getTimeSlotState READ getTimeSlotState NOTIFY timeSlotChanged )
    Q_PROPERTY(bool isRunning READ isRunning WRITE setRunning NOTIFY timeSlotChanged)
public:
    TimeSlot();

    void setTimeSlotUiId(int uiId);
    int getTimeSlotUiId() const;

    void setTimeSlotOrder(int order);
    int getTimeSlotOrder() const;

    void setTimeSlotState(QString state, int identity);
    QString getTimeSlotState();

    void setTimeSlotTime(QString time);
    QString getTimeSlotTime() const;

    void setKey(QString key);
    QString getParticipant() const;
    QString getParticipantFromKey(QString key);

    void setRunning(bool input);
    bool isRunning();
    void copy(TimeSlot *ts);
    void setMemory(MemoryHandler *memory);


Q_SIGNALS:
    void timeSlotChanged();
    void runningChanged();
private:

    enum eSlotStates{
        eNeutral,
        eRunning,
        eStarted,
        eStopped,
        eSkipped,
        eUnknown
    };

    eSlotStates parseTimeSlotState(QString state);
    int timeSlotUiId;
    QString instanceTime;
    QString instanceParticipant;
    QString instanceKey;
    int instanceOrder;
    bool running;
    eSlotStates timeSlotState;
    int instanceIdentity;
    MemoryHandler *mh;
};

#endif // TIMESLOT_H
