#include "serverthread.h"
#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QObject>
#include <QJsonDocument>
#include <QDebug>

ServerThread::ServerThread(QObject *parent) : QObject(parent)
{

}

void ServerThread::postData(int identity, QString key, QString time) {
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

    //mgr->get(QNetworkRequest(QUrl("http://axess.fi/axspeed/")));
    QString timeIdentity;
    timeIdentity = key + ","+ time;
    //mgr->get(QNetworkRequest(QUrl("http://axess.fi/axspeed/?json={%22" + timeIdentity + "%22}")));
    mgr->get(QNetworkRequest(QUrl("http://ec2-52-90-178-39.compute-1.amazonaws.com:8000/"+ timeIdentity + "/post/" )));
}

void ServerThread::onFinished(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString str(bts);
    qDebug() << "OK: " << str;
}
