#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

class ServerThread : public QObject
{
    Q_OBJECT
public:
    explicit ServerThread(QObject *parent = 0);
    void postData(int identity, QString key, QString time);

private slots:
    void onFinished(QNetworkReply *rep);
};

#endif // SERVERTHREAD_H
