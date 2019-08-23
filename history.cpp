#include "history.h"

History::History()
{

}

void History::initialize()
{
    loc = 0;
    historyMaxLimit = 0;
}

void History::addToHistory(TimeSlot *ts)
{
    TimeSlot *tsCopy = new TimeSlot;
    tsCopy->copy(ts);
    historyList.insert(loc, tsCopy);
    loc++;
    if(loc > historyMaxLimit) {
        loc = 0;
    }
}

void History::setMaxLimit(int maxVal)
{
    historyMaxLimit = maxVal;
}
