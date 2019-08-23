#ifndef MEMORYHANDLER_H
#define MEMORYHANDLER_H

#include <QObject>
#include <QStringList>

class MemoryHandler : public QObject
{
    Q_OBJECT
public:
    explicit MemoryHandler(QObject *parent = 0);
    void storeTimeToCSV(QString rider, QString time);
    void writeCSV(QString txt, QString filename);
    void appendCSV(QString txt, QString filename);
    QStringList readCSV(QString filename);

    void insertTimeIdentity(int timeSlotId, int identity);
    bool searchIdentity(int identity);
    void clearTimeIdentity();

signals:

public slots:
private:
    QList<int> timeIdentityMemory;

};

#endif // MEMORYHANDLER_H
