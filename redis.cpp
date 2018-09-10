#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include <QGuiApplication>
#include "redis.h"

Redis::Redis(QObject *parent) : QObject(parent)
{
    keepactive = false;
    //    QSettings setting("Config.ini",QSettings::IniFormat);
    //    setting.setIniCodec("GB2312");
    //    storeID = setting.value("Store/Id","001").toString();
    //    m_redisIP = setting.value("Net/redisip","http://crazyfitting.ur.com.cn").toString();
    //    m_redisPort = setting.value("Net/redisport","9003").toInt();



    m_redisIP = "192.168.4.212";
    m_redisPort = 9003;

    qDebug() << storeID << m_redisIP <<m_redisPort;

    connect(this,SIGNAL(reconnect()),this,SLOT(restartconnect()));

}



void Redis::startSubscribe()
{
    m_redis = new QtRedis(m_redisIP,m_redisPort);
    keepredis = new QtRedis(m_redisIP,m_redisPort);
    connect(m_redis, SIGNAL(returnData(QtRedis::Reply)), this, SLOT(slotMessage(QtRedis::Reply)));
    connect(&keepactivetimer,SIGNAL(timeout()),this,SLOT(keepactiveTimeout()));

    if (!m_redis->openConnection())
    {
        qDebug() << "Could not connect to server...";
        keepactive = false;
    }else
    {
        keepactive = true;
    }

    if (!keepredis->openConnection())
    {
        qDebug() << "Could not connect to server...";
        keepactive = false;
    }else
    {
        keepactive = true;
    }

    qDebug() << "Connected to server...";

    m_redis->subscribe(QString("UNMANNED_SHOP:AD_VIDEOCHANGE"));
    m_redis->subscribe("Keepalive");

    keepactivetimer.start(20000);

}


void Redis::slotMessage(QtRedis::Reply reply)
{

    if(reply.channel == "Keepalive")
    {
        keepactive = true;
        qDebug() << "redis is keppactive.........";
        return;
    }

    QTime time;
    qDebug() << time.currentTime();
    qDebug() << "Channel:" << reply.channel << "Pattern:" << reply.pattern;
    qDebug() << reply.value.toString();


//    QString modelcode = "";
//    QString username = "";
//    QStringList templist = reply.value.toString().split(',');
//    if(templist.count()>2){
//        qDebug() << templist.count()
//                 << templist.at(0)
//                 << templist.at(1);
//        QStringList templist3 = QString(templist.at(0)).split('\"');
//        QStringList templist2 = QString(templist.at(2)).split('\"');

//        if(templist2.count()>4){
//            modelcode = templist2.at(3);
//            qDebug()<<modelcode;
//        }
//        if(templist3.count()>3){
//            username = templist3.at(3);
//        }



//    }

//    if(modelcode != ""){
//        emit opendoormodel(modelcode);
//    }

//    if(modelcode != ""){
//        emit opendooruser(username);
//    }


    emit vlaueschange();

    //    QString msg;

    //    QScriptEngine engine;
    //    QScriptValue sc =  engine.evaluate("value=" + reply.value.toString());
    //    if (sc.property("Content").isArray())   //解析json数组
    //    {
    //        QScriptValueIterator it(sc.property("Content"));
    //        while(it.hasNext())
    //        {
    //            it.next();
    //            QString roomid =  it.value().property("OrderArea").toString();
    //            QString personID =  it.value().property("OrderNo").toString();
    //            QString tempstr = QString(" 请 <font color='red'>%1</font> 号顾客前往试衣区 <font color='red'>%2</font> 稍后,即将进入试衣间。").arg(personID).arg(roomid);
    //            QString englishstr = QString("Please <font color='red'>%1</font> customers to go to the dressing area <font color='red'>%2</font> waiting to enter the dressing room").arg(personID).arg(roomid);

    //            if(roomid!=""&&personID!="")
    //            {
    //                msgvector.append(tempstr);
    //                msgvector.append(englishstr);
    //            }


    //        }

    //    }


}

void Redis::restartconnect()
{
    m_redis->disconnectHost();
    keepredis->disconnectHost();
    if (!m_redis->openConnection())
    {
        qDebug() << "Could not connect to server...";
        keepactive = false;
    }else
    {
        keepactive = true;
    }

    if (!keepredis->openConnection())
    {
        qDebug() << "Could not connect to server...";
        keepactive = false;
    }else
    {
        keepactive = true;
    }

    qDebug() << "Connected to server...";

    m_redis->subscribe(QString("UNMANNED_SHOP:AD_VIDEOCHANGE"));
    m_redis->subscribe("Keepalive");
}




void Redis::keepactiveTimeout()
{
    if(!keepactive){
        emit reconnect();
    }else{
        keepactive = false;
        keepredis->publish(QString("Keepalive"),"online");
    }
}
