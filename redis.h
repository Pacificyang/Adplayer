#ifndef REDIS_H
#define REDIS_H

#include <QObject>
#include <QtRedis>
#include <QTime>
#include <QSettings>
#include <QTimer>


class Redis : public QObject
{
    Q_OBJECT
public:
    explicit Redis(QObject *parent = 0);

    void startSubscribe();

signals:
    void vlaueschange();
    void reconnect();
    void opendoormodel(const QString&modecode);
    void opendooruser(const QString&username);

public slots:
    void slotMessage(QtRedis::Reply reply);
    void keepactiveTimeout();

private slots:
    void restartconnect();


private:
    QtRedis *m_redis;
    QString storeID;
    QString m_redisIP;
    int m_redisPort;


    QTimer keepactivetimer;
    QtRedis *keepredis;
    bool keepactive;


};

#endif // REDIS_H
