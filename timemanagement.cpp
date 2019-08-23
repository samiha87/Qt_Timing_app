#include <QStringList>
#include <QDebug>

#include "timemanagement.h"
#include "device_status.h"

// TODO change this page as devicePage
TimeManagement::TimeManagement(QObject *parent):  QObject(parent)
{
    startActive = true;
    splitActive = false;
    endActive = false;
    deviceStateChanged();
}

TimeManagement::~TimeManagement()
{
    int count = timeSlotList.count();
    for(int i = 0; i < count; i++) {
        TimeSlot *toRemove = (TimeSlot *)timeSlotList.at(i);
        toRemove->deleteLater();
    }

    timeSlotList.clear();
}

QVariant TimeManagement::getTimeSlots()
{
    return QVariant::fromValue(timeSlotList);
}

QVariant TimeManagement::getBattery()
{
    return QVariant(deviceStatus.getBattery());
}

QVariant TimeManagement::getType()
{
    return QVariant(deviceStatus.getType());
}

void TimeManagement::createTimeSlots(int amount)
{// create common memory
    for(int i = 0; i < amount; i++) {
        timeSlot = new TimeSlot();
        timeSlot->setTimeSlotUiId(i);
        timeSlot->setMemory(&mh);
        timeSlotList.append(timeSlot);
    }
    emit timeSlotsUpdated();
}

void TimeManagement::addTimeSlot()
{
    timeSlot = new TimeSlot();
    timeSlot->setTimeSlotUiId(timeSlotList.count() + 1);
    timeSlot->setMemory(&mh);
    timeSlotList.append(timeSlot);
    emit timeSlotsUpdated();
}

void TimeManagement::setTime(int slot, QString time)
{

    if(timeSlotList.count() < slot) {
        return;
    }

    TimeSlot *ts;
    ts = (TimeSlot *)timeSlotList.at(slot);
    ts->setTimeSlotTime(time);
    emit timeSlotsUpdated();
}

void TimeManagement::setRFID(int slot, QString key)
{

    if(timeSlotList.count() < slot) {
        return;
    }
    TimeSlot *ts;
    ts = (TimeSlot *)timeSlotList.at(slot);
    ts->setKey(key);
    emit timeSlotsUpdated();
}

void TimeManagement::setTimeFromMS(int slot, long milliSeconds)
{
    if(timeSlotList.count() < slot) {
        return;
    }

    setTime(slot, convertMsToClockFormat(milliSeconds) );
}

void TimeManagement::setTimeState(int slot,int identity, QString state)
{
    if(timeSlotList.count() < slot) {
        return;
    }

    TimeSlot *ts;
    ts = (TimeSlot *)timeSlotList.at(slot);

    ts->setTimeSlotState(state, identity);
    emit timeSlotsUpdated();
}

void TimeManagement::addTime(QString input)
{
    // Parse message here
    Q_UNUSED(input);
    emit timeSlotsUpdated();
}

// TODO make a class messageManager
/*! \patterns
 * "#MT,GT,UI,0,00:00:00,c,1,1,00:00:00,c,2,2,00:00:00,c,3,3,00:00:00,c,4,4,00:00:00,c,5,5,00:00:00,c,6,6,00:00:00,c,7,start;;"
 * "#MT,GT,BATTERY,C67;;"
 * "#MT,GT,BATTERY,67;;"
 * "#MT,GT,DEBUG,<debug message>;;"
 */

/*
0	:	Start byte
1	:	Start byte
2	:	Message style
3	:	Seperator

0	:	UI Slot
1	:	Time multiplier2
2	:	Time multiplier 1
3	:	Time
4	:	Slot state
5	:	Time identity
6	:	RFID Byte 0
7	:	RFID Byte 1
8	:	RFID Byte 2
9	:	RFID Byte 3
10	:	RFID Byte 4
11	:	RFID Byte 5
12	:	RFID Byte 6
13	:	RFID Byte 7
14	:	End of line

16	:	End byte
17	:	End byte

*/

void TimeManagement::parseMessage(QByteArray msg)
{
    qDebug() << "TimeManagement::parseMessage() " << msg;
    QByteArray endByte = "*";

    if(!msg.contains(',')) return;
    //QByteArrayList message = msg.split(',');
    QByteArrayList message;
    // Seperate message

    if(msg.at(0) == 'U') {
        msg.remove(0, msg.indexOf(',') + 1);    // Remove all until first comma
        qDebug() << "TimeManagement::parseMessage() U: " << msg;
        for(int i = 0; i < 8; i++) {
            if(msg.count() > 13 && msg.at(14) == ',') {

                message.append(msg.mid(0, 15));
                msg.remove(0, 15);
                // Remove last comma
                qDebug() << "TimeManagement::parseMessage() Parsed" << msg;
            } else {
                break;
            }
        }

        for (int i = 0; i < message.count(); i++) {
            if(message.at(i).count() < 13) return;

            qDebug() << "*************************************************************************************************************";
            qDebug() << "TimeManagement::parseMessage(): slot: " << QString::number(i) << " " << message.at(i);
            qDebug() << "TimeManagement::parseMessage(): slotId " << QString::number( (int)message.at(i).at(0) );           // Slot id
            qDebug() << "TimeManagement::parseMessage(): Time multiplier 2 " << QString::number( (int)message.at(i).at(1) );  // Time multiplier 2
            qDebug() << "TimeManagement::parseMessage(): Time multiplier 1" << QString::number( (int)message.at(i).at(2) );  // Time multiplier 1
            qDebug() << "TimeManagement::parseMessage(): Time " << QString::number( (int)message.at(i).at(3) );             // Time
            qDebug() << "TimeManagement::parseMessage(): Timer state " << message.at(i).at(4);      // Timer state, 0 still, 1 started, 2 running, 3 just stopped
            qDebug() << "TimeManagement::parseMessage(): Time identity " << QString::number( message.at(i).at(5) );    // Time identity
            qDebug() << "TimeManagement::parseMessage(): RFID high byte " << QString::number( (int)message.at(i).at(6) );   // RFID High byte   0
            qDebug() << "TimeManagement::parseMessage(): RFID low byte " << QString::number( (int)message.at(i).at(7) );    // RFID low byte    1
            qDebug() << "TimeManagement::parseMessage(): RFID low byte " << QString::number( (int)message.at(i).at(8) );    // RFID low byte    2
            qDebug() << "TimeManagement::parseMessage(): RFID low byte " << QString::number( (int)message.at(i).at(9) );    // RFID low byte    3
            qDebug() << "TimeManagement::parseMessage(): RFID low byte " << QString::number( (int)message.at(i).at(10) );    // RFID low byte   4
            qDebug() << "TimeManagement::parseMessage(): RFID high byte " << QString::number( (int)message.at(i).at(11) );   // RFID High byte  5
            qDebug() << "TimeManagement::parseMessage(): RFID low byte " << QString::number( (int)message.at(i).at(12) );    // RFID low byte   6
            qDebug() << "TimeManagement::parseMessage(): RFID low byte " << QString::number( (int)message.at(i).at(13) );    // RFID low byte   7
            qDebug() << "TimeManagement::parseMessage(): Calculated time " << QString::number( ( (int)message.at(i).at(1) * (255 * 255)) + ((int)message.at(i).at(2) * 255) + (int)message.at(i).at(3));
            qDebug() << "*************************************************************************************************************";

            long totalTime =  ( (int)message.at(i).at(1) * (255 * 255)) + ((int)message.at(i).at(2) * 255) + (int)message.at(i).at(3);
            int timeIdentity = (int)message.at(i).at(5);
            //qDebug() << "TimeManagement::parseMessage(): setTimeFromMS " << (int)message.at(i).at(0) << " time: " << totalTime;
            setTimeFromMS( ((int)message.at(i).at(0)), totalTime);
            //qDebug() << "TimeManagement::parseMessage(): setTimeState " << (int)message.at(i).at(0) << " to " << message.at(i).at(4) ;

            setTimeState( ((int)message.at(i).at(0)),  timeIdentity,  QString(message.at(i).at(4)) );
            QString tempKey = QString::number( (int)message.at(i).at(6)) + QString::number( (int)message.at(i).at(7)) + QString::number( (int)message.at(i).at(8)) +
                            QString::number( (int)message.at(i).at(9)) + QString::number( (int)message.at(i).at(10)) + QString::number( (int)message.at(i).at(11)) +
                            QString::number( (int)message.at(i).at(12)) + QString::number( (int)message.at(i).at(13));
            setRFID(((int)message.at(i).at(0)), tempKey);
        }
    } else if(msg.at(0) == 'B' && msg.count() > 2) {

        QString alter;
        // Convert char values to string
        int convertToString = (int)msg.at(2);
        alter = QString::number(convertToString);
        if(alter.contains(endByte)) {
            alter.replace(QString(endByte),QString("  "));
        }
        qDebug() << "TimeManagement::parseMessage(): Battery is " << alter;
        deviceStatus.setBattery(alter.simplified());
        emit batteryChanged();
    }
    return;

    if(message.count() > 1 && message.at(0) == "U") {

    } else if(message.count() > 0 && message.at(0) == "DB") {
       // qDebug() << "TimeManagement::Db_incoming";
    } else if (message.count() > 1 && message.at(0) == "B") {
        QString alter;
        // Convert char values to string
        int convertToString = (int)message.at(1).at(0);
        alter = QString::number(convertToString);
        if(alter.contains(endByte)) {
            alter.replace(QString(endByte),QString("  "));
        }
        qDebug() << "TimeManagement::parseMessage(): Battery is " << alter;
        deviceStatus.setBattery(alter.simplified());
        emit batteryChanged();
    } else if ( message.count() > 3 && message.at(2) == "DEBUG") {

        qDebug() << "TimeManagement::parseMessage(): Debug: " << msg;
    }
}

void TimeManagement::setDeviceType(DeviceTypes types)
{
    switch(types) {
        case mtStart:
            startActive = true;
            splitActive = false;
            endActive = false;
        break;
    case mtSplit:
            startActive = false;
            splitActive = true;
            endActive = false;
        break;
    case mtEnd:
            startActive = false;
            splitActive = true;
            endActive = false;
        break;
    case mtMaster:
            startActive = false;
            splitActive = false;
            endActive = false;
            masterActive = true;
        break;
    case mtUnknown:
            startActive = true;
            splitActive = false;
            endActive = false;
        break;
    }
}

bool TimeManagement::getStartState()
{
    return startActive;
}

bool TimeManagement::getSplitState()
{
    return splitActive;
}

bool TimeManagement::getEndState()
{
    return endActive;
}

bool TimeManagement::getMasterState()
{
    return masterActive;
}

QString TimeManagement::convertMsToClockFormat(long mSeconds)
{
  long hundreadOfSeconds = 0;
  long tempSeconds = 0 ;
  long tempMinutes = 0;
  long tempHours = 0;

  int hSeconds = 0;
  int seconds = 0;
  int minutes = 0;
  int hours = 0;

  hundreadOfSeconds = (mSeconds / 10);
  hSeconds = (int)(hundreadOfSeconds%100);

  tempSeconds = (hundreadOfSeconds/100);
  seconds = (int)(tempSeconds%60);

  tempMinutes = (tempSeconds/60);
  minutes = (int)(tempMinutes%60);

  tempHours = (tempMinutes/60);
  hours = (tempHours%24);

  // Transform into uiFormat
  QString toUi;
  QString uiHours;
  QString uiMinutes;
  QString uiSeconds;
  QString uiHSeconds;

  if(hours < 10) {
    uiHours = "0" + QString::number(hours);
  } else {
    uiHours = QString::number(hours);
  }
  //qDebug() << "TimeManagement::convertMsToClockFormat() hours" << uiHours;

  if(minutes < 10) {
    uiMinutes = "0" + QString::number(minutes);
  } else {
    uiMinutes = QString::number(minutes);
  }

  //qDebug() << "TimeManagement::convertMsToClockFormat() minutes " <<  uiMinutes;
  if(seconds < 10) {
    uiSeconds = "0" + QString::number(seconds);
  } else {
    uiSeconds = QString::number(seconds);
  }

  //qDebug() << "TimeManagement::convertMsToClockFormat() hundread of seconds " << uiSeconds;
  if(hSeconds < 10) {
    uiHSeconds = "0" + QString::number(hSeconds);
  } else {
    uiHSeconds = QString::number(hSeconds);
  }
  //qDebug() << "TimeManagement::convertMsToClockFormat() hundread of seconds " << uiHSeconds;
  toUi = ( uiMinutes + ":" + uiSeconds + ":" + uiHSeconds );
  return toUi;
}
