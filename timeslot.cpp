#include "timeslot.h"
#include <QDebug>
#include "memoryhandler.h"
#include "serverthread.h"

TimeSlot::TimeSlot()
{
    timeSlotUiId = 0;
    running = 0;
    instanceTime = "00:00:00";
    instanceKey =" ";
    instanceParticipant = "";
    timeSlotState = eNeutral;
    instanceIdentity = 0;
}

void TimeSlot::copy(TimeSlot *ts)
{
    timeSlotUiId = ts->getTimeSlotUiId();
    instanceTime = ts->getTimeSlotTime();
}

void TimeSlot::setTimeSlotUiId(int uiId)
{
    timeSlotUiId = uiId;
    emit timeSlotChanged();
}

int TimeSlot::getTimeSlotUiId() const
{
    return timeSlotUiId;
}

void TimeSlot::setTimeSlotTime(QString time)
{
    instanceTime = time;
    emit timeSlotChanged();
}

void TimeSlot::setKey(QString key)
{
    instanceKey = key;
    instanceParticipant = getParticipantFromKey(key);
    emit timeSlotChanged();
}

QString TimeSlot::getParticipantFromKey(QString key)
{
    if(key.contains("18661426211297224")) {
        return "Markus Kainulainen";
    }

    if(key.contains("244182230310")) {
        return "Sami Hamici";
    }
    return "";
}

QString TimeSlot::getParticipant() const
{
    return instanceParticipant;
}

void TimeSlot::setTimeSlotState(QString state, int identity)
{
    if(timeSlotState == parseTimeSlotState(state)) return;
    // IF current timeslot state is Time running and next is state is Time stopped, send json query to
    if(instanceIdentity != identity && instanceKey != "" & instanceKey != " ") {

        if(mh->searchIdentity(identity)) {
            // already processed
        } else if(parseTimeSlotState(state) == TimeSlot::eSkipped && identity > 10 && instanceKey != "") {
            instanceIdentity = identity;
            mh->insertTimeIdentity(timeSlotUiId, identity);
            instanceTime = "SKIP";
            ServerThread st;
            st.postData(instanceIdentity, instanceKey, instanceTime);
            qDebug() << "TimeSlot::setTimeSlotState(): post to server Skip";
        } else if(parseTimeSlotState(state) == TimeSlot::eStopped && identity > 10 && instanceKey != "") {
            instanceIdentity = identity;
            mh->insertTimeIdentity(timeSlotUiId, identity);
            ServerThread st;
            st.postData(instanceIdentity, instanceKey, instanceTime);
            qDebug() << "TimeSlot::setTimeSlotState(): post to server)";;
        }
    }
    timeSlotState = parseTimeSlotState(state);
    emit timeSlotChanged();
}

QString TimeSlot::getTimeSlotTime() const
{
    return instanceTime;
}

QString TimeSlot::getTimeSlotState()
{
    switch(timeSlotState) {
        case eNeutral:
            return "lightsteelblue";
        break;
        case eRunning:
            return "green";
        break;
        case eStarted:
            return "yellow";
        break;
        case eStopped:
            return "red";
        break;
        default:
            return "lightsteelblue";
        break;
    }
}

void TimeSlot::setRunning(bool input)
{
    running = input;
    emit runningChanged();
}

bool TimeSlot::isRunning()
{
    bool state = running;
    running = false;
    return state;
}

void TimeSlot::setTimeSlotOrder(int order)
{
    instanceOrder = order;
}

int TimeSlot::getTimeSlotOrder() const
{
    return instanceOrder;
}

TimeSlot::eSlotStates TimeSlot::parseTimeSlotState(QString state)
{
    if(state == "r") return eRunning;
    else if(state == "x") {
        return eSkipped;
    } else if (state == "s") {
        // Lap started
        return eStarted;
    } else if (state == "e") {
        // Lap finished
       // MemoryHandler mem;
        //mem.appendCSV("RiderX",instanceTime);
        return eStopped;
    } else {
        return eNeutral;
    }
}

void TimeSlot::setMemory(MemoryHandler *memory) {
    mh = memory;
}


