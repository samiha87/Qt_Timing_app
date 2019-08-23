#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>

#include "timeslot.h"

class History
{
    Q_OBJECT
public:
    History();
    void initialize();
    void addToHistory(TimeSlot *ts);
    void setMaxLimit(int maxVal);
private:
    QList<QObject*> historyList;

    int loc;
    int historyMaxLimit;
};

#endif // HISTORY_H
