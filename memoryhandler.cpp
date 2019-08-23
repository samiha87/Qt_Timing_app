#include <QFile>

#include <QDebug>
#include <QTextStream>
#include <QStandardItemModel>
#include <QDateTime>

#include "memoryhandler.h"

MemoryHandler::MemoryHandler(QObject *parent) : QObject(parent)
{

}

void MemoryHandler::storeTimeToCSV(QString rider, QString time)
{
    QString toWrite = (rider + ";" + time);
    QDate currentDate = QDateTime::currentDateTime().date();
    QString date = currentDate.toString();
    appendCSV(toWrite, date);
}

void MemoryHandler::writeCSV(QString txt,QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    QByteArray line = txt.toLocal8Bit() + ',';
    file.write(line);
}

void MemoryHandler::appendCSV(QString txt, QString filename)
{
    QFile file(filename);
    QByteArray temporaryFile;
    // If file doesn't exit try to create new file
    // If file exist read old information and store it to temporary file
    if(file.open(QIODevice::ReadWrite)) {
        while(!file.atEnd()) {
            temporaryFile = file.readLine();
        }
    }
    QByteArray toWrite = (temporaryFile + "\n" + txt.toLocal8Bit());
    file.write(toWrite);
    file.close();   // Close file here
}

QStringList MemoryHandler::readCSV(QString filename)
{
    QFile file(filename);
    QStringList wordList;

    if(!file.open(QIODevice::ReadOnly)) {
        return wordList;
    }

    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        wordList.append(line.split(',').first());
    }
    qDebug() << wordList;

    file.close();   // Close file
    return wordList;
}

void MemoryHandler::insertTimeIdentity(int timeSlotId, int identity)
{
    timeIdentityMemory.insert(timeSlotId, identity);
}

bool MemoryHandler::searchIdentity(int identity)
{
    if(timeIdentityMemory.contains(identity)) return true;
    return false;
}

void MemoryHandler::clearTimeIdentity()
{
    timeIdentityMemory.clear();
}
