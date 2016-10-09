#include "timeslot.h"
#include <QDebug>
#include "memoryhandler.h"

TimeSlot::TimeSlot()
{
    timeSlotUiId = 0;
    running = 0;
    instanceTime = "00:00:00";
    timeSlotState = eNeutral;
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

void TimeSlot::setTimeSlotState(QString state)
{
    if(timeSlotState == parseTimeSlotState(state)) return;

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
    else if(state == "x") return eNeutral;
    else if (state == "s") {
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
