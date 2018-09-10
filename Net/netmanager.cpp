#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "netmanager.h"
#include <QSettings>
#include <QtConcurrent>

Netmanager::Netmanager()
{

    QString filepath = QDir::toNativeSeparators(
                qApp->applicationDirPath()
                + QDir::separator()
                +"Playresource"
                + QDir::separator());

    QDir dir(filepath);

    if(!dir.exists()){
        dir.mkpath(filepath);

    }

    QSettings setting("Config.ini",QSettings::IniFormat);
    setting.setIniCodec("GB2312");
    storeID = setting.value("Store/Id","001").toString();
    LocationID = setting.value("Location/Num",0).toString();


    update_manager = new QNetworkAccessManager;

    connect(update_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(readNetworkReply(QNetworkReply*)));


    connect(&msgTimer,SIGNAL(timeout()),this,SLOT(shootmsg()));

    msgTimer.setInterval(2150);
    msgTimer.start();

    connect(&disTimer,SIGNAL(timeout()),this,SLOT(shootdis()));
    disTimer.setInterval(5000);
}


void Netmanager::compareversion()
{
    m_state = COMPAREVER;
    QString url = "http://crazyfitting.ur.com.cn/fittingfiles/app/sys-win-show.json";

    update_manager->get(QNetworkRequest(QUrl(url)));
}




void Netmanager::readNetworkReply(QNetworkReply *reply)
{

    if (reply->error() != QNetworkReply::NoError)
    {

        QString aim , err;
        switch (reply->error()) {
        case QNetworkReply::ConnectionRefusedError:
            err = "失败原因:远程服务器拒绝连接。";
            break;
        case QNetworkReply::ContentNotFoundError:
            err = "失败原因:网络请求404";
            break;
        case QNetworkReply::HostNotFoundError:
            err = "失败原因:请求地址不存在";
            break;
        case QNetworkReply::TimeoutError :
            err = "失败原因:请求连接超时";
            break;
        case QNetworkReply::InternalServerError :
            err = "失败原因:服务器存储空间不足";
            break;
        default:
            err = reply->errorString();
            qDebug() << "net error ：" << reply->errorString();

            break;
        }


        if(m_state == GET_PLAYRESOURCR){
            emit filedowncomplete();
        }

        return;
    }

    switch (m_state) {
    case GET_FITTINGINFO:{

        QByteArray message = reply->readAll();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(message);
        QJsonObject jsonObject = jsonDoc.object();
        int state = jsonObject["state"].toInt();
        if(!state > 0 )
        {
            return;
        }

        QJsonObject dataArray = jsonObject.value("data").toObject();
        QJsonArray zonesArray = dataArray.value("zones").toArray();

        msgvector.clear();
        m_information.clear();
        msgnum=0;
        QString allnumber = QString::number(dataArray.value("allNum").toInt());
        QString ftimes =  QString::number(dataArray.value("Fwaittimes").toInt());
        QString mtimes =  QString::number(dataArray.value("Mwaittimes").toInt());
        emit zoneswaitnumber(allnumber,ftimes ,mtimes);

        for(int i = 0 ; i < zonesArray.count() ; i++)
        {
            QJsonObject dataObject = zonesArray.at(i).toObject();



            if(dataObject.value("name") == "A"){
                emit zoneswaitanumber(QString::number(dataObject.value("num").toInt()),QString::number(dataObject.value("roomNum").toInt()));
            }

            if(dataObject.value("name") == "B"){
                emit zoneswaitbnumber(QString::number(dataObject.value("num").toInt()),QString::number(dataObject.value("roomNum").toInt()));
            }

            if(dataObject.value("name") == "C"){
                emit zoneswaitcnumber(QString::number(dataObject.value("num").toInt()),QString::number(dataObject.value("roomNum").toInt()));
            }
            QJsonArray array = dataObject.value("bufferQueue").toArray();
            for(int j = 0 ; j< array.count();j++)
            {
                QJsonObject bufferObject = array.at(j).toObject();
                QJsonArray bufferarray = bufferObject.value("queueItems").toArray();
                for(int k = 0 ; k < bufferarray.count(); k++)
                {
                    QJsonObject queueobject = bufferarray.at(k).toObject();

                    waitinformation info;
                    info.code = queueobject.value("orderNo").toString();
                    info.creattime =  QDateTime::fromString(queueobject.value("callTime").toString(),"yyyy-MM-dd hh:mm:ss").toTime_t();
                    info.zoom = dataObject.value("name").toString();

                    m_information.append(info);
                }
            }

        }
        qSort(m_information.begin(),m_information.end(),compareData);
        for(int i = 0 ;i < 5; i++)
        {
            if(i < m_information.count())
            {
                QString roomid =  m_information.at(i).zoom;
                QString personID =  m_information.at(i).code;

                if(roomid!=""&&personID!="")
                {
                    Msginfo info;
                    info.code = personID;
                    info.aera = roomid;
                    msgvector.append(info);
                }
            }
        }
    }

        break;

    case GET_PLAYRESOURCR:{
        QByteArray message = reply->readAll();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(message);
        QJsonObject jsonObject = jsonDoc.object();
        int state = jsonObject["state"].toInt();
        if(!state > 0 )
        {
            emit filedowncomplete();
            return;
        }
        qDebug()<<"parse resource data .......";

        QJsonArray dataArray = jsonObject.value("data").toArray();


        QString filepath = QDir::toNativeSeparators(
                    qApp->applicationDirPath()
                    + QDir::separator()
                    +"Playresource"
                    + QDir::separator());
        //qDebug()<<"filepath"<<filepath;

        QDir dir(filepath);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList filelist = dir.entryInfoList();
        QStringList list;
        for(int j = 0 ; j < filelist.count() ; j++)
        {
            list.append(filelist.at(j).fileName());
        }

        qDebug()<<"dataArray.count():"<<dataArray.count();
        for(int i = 0 ; i < dataArray.count() ; i++)
        {
            QJsonObject dataObject = dataArray.at(i).toObject();

            QString netfile = dataObject.value("path").toString();
            QFileInfo info(netfile);
            if(list.contains(info.fileName()))
            {
                list.removeOne(info.fileName());  //if exist already,delete the filename from download list
            }else{
                m_downloadfiles.append(dataObject.value("path").toString()); //not exists,append to m_downloadfiles
            }

        }

        for(int j = 0 ; j < list.count() ; j ++)
        {
            QFile::remove(filepath + list.at(j));
        }






        //对比本地文件及网上文件列表  没有的就删除 有的跳过（在容器中删除掉··）


        if (!m_downloadfiles.isEmpty()) {
            qDebug() << "Download video files";
            getdownloadfiles();
            return;
        }else {
            emit filedowncomplete();
            return;
        }

    }
        break;

    case GET_DOWNLOADFILE:{
        QByteArray message = reply->readAll();
        QFileInfo fileInfo(m_downloadfiles.takeFirst());

        QString fileName = QDir::toNativeSeparators(
                    qApp->applicationDirPath()
                    + QDir::separator() 
                    +"Playresource"
                    + QDir::separator()
                    + fileInfo.fileName());

        QFile output(fileName);
        if (output.open(QIODevice::WriteOnly)) {
            output.write(message);
            output.close();
        }

        //        emit addPlayitem(fileName);

        if (!m_downloadfiles.isEmpty()) {
            getdownloadfiles();
            return;
        } else {
            emit filedowncomplete();
            return;
        }

    }
        break;

    case GET_PERSONNUMBER:{
        QByteArray message = reply->readAll();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(message);
        QJsonObject jsonObject = jsonDoc.object();
        int state = jsonObject["state"].toInt();
        if(!state > 0 )
        {
            return;
        }

        QJsonObject dataArray = jsonObject.value("data").toObject();
        if(dataArray.keys().contains("userCurrentNO")&&dataArray.keys().contains("userWaitTime"))
        {
            emit usercurrentnum(QString::number(dataArray.value("userCurrentNO").toInt()),QString::number(dataArray.value("userWaitTime").toInt()));

        }

    }
        break;
    case GET_CLOTHESINFO:{
        QByteArray message = reply->readAll();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(message);
        QJsonObject jsonObject = jsonDoc.object();
        int state = jsonObject["state"].toInt();
        if(!state > 0 )
        {
            return;
        }
        QVariantMap positiveinfo;
        QVariantMap backinfo;

        QJsonObject dataArray = jsonObject.value("data").toObject();
        QJsonArray modelsarray = dataArray.value("models").toArray();
        QJsonArray imgarray = modelsarray.first().toObject().value("imgs").toArray();

        for(int i = 0;i<imgarray.count();i++)
        {
            if(imgarray.at(i).toObject().value("face").toInt() == 1)
            {
                positiveinfo.insert("path",imgarray.at(i).toObject().value("imgPath").toString());
                positiveinfo.insert("barcode", imgarray.at(i).toObject().value("barCode").toString());
            }
            if(imgarray.at(i).toObject().value("face").toInt() == 2)
            {
                backinfo.insert("path",imgarray.at(i).toObject().value("imgPath").toString());
                backinfo.insert("barcode", imgarray.at(i).toObject().value("barCode").toString());
            }
        }

        QJsonObject clothesobject = dataArray.value("clothes").toObject();
        positiveinfo.insert("name",clothesobject.value("clothesName").toString());
        backinfo.insert("name",clothesobject.value("clothesName").toString());
        positiveinfo.insert("price",clothesobject.value("price").toInt());
        backinfo.insert("price",clothesobject.value("price").toInt());

        //"<font color='red'>%1</font>"

        QJsonArray materialsarray = dataArray.value("materials").toArray();
        QString info;
        for(int i = 0 ; i < materialsarray.count();i++)
        {
            QJsonObject object = materialsarray.at(i).toObject();
            QString name  = object.value("name").toString();
            QString sort = QString::number(object.value("sort").toInt());
            if(i == 0)
            {
                info = "<b style='LINE-HEIGHT:10px'>" + sort;
                info.append("%");
                info.append(name);
            }else if(i == materialsarray.count()-1)
            {
                info.append("<br />");
                info.append(sort);
                info.append("%");
                info.append(name);
                info.append("</b>");
            }else
            {
                info.append("<br />");
                info.append(sort);
                info.append("%");
                info.append(name);
            }
        }
        positiveinfo.insert("info",info);
        backinfo.insert("info",info);


        m_clothesinfo.append(positiveinfo);
        m_clothesinfo.append(backinfo);



        emit disclothes(m_clothesinfo.takeFirst());

        disTimer.start();

    }
        break;
    case COMPAREVER:{

        QSettings setting("Config.ini",QSettings::IniFormat);
        setting.setIniCodec("GB2312");
        int version = setting.value("Ver/version").toInt();

        QByteArray message = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(message);
        QJsonObject jsonObject = jsonDoc.object();


        if(version >= jsonObject.value("versionCode").toString().toInt())
        {
            updatestate = false;
        }else
        {
            updatestate = true;
        }
        emit comparecomplete();
    }
        break;
    default:
        break;
    }



    //上传成功

    reply->deleteLater();
}

void Netmanager::Getclothesinfo(const QString &clothesid)
{
    m_state = GET_CLOTHESINFO;

    QString url = "http://service.crazyfitting.com/";
    QString content = QString("app/clothes/dressImgByBarCode?barCode=%1").arg(clothesid);
    url.append(content);
    //    QString url = "http://service.crazyfitting.com/app/clothes/dressImgByBarCode?barCode=WE31R5EN2000800M";

    update_manager->get(QNetworkRequest(QUrl(url)));


}

void Netmanager::Getcurrentclothes(const QString&code)
{
    QString filepath = QDir::toNativeSeparators(
                qApp->applicationDirPath()
                + QDir::separator()
                +"idress"
                + QDir::separator());

    if(code == "I17DR02E0001SS0F"||code == "I17DR02E0002SS0F")
    {
        m_playList.clear();
        m_playList.append(QString("file:///%1").arg(filepath+"2.png"));
        m_playList.append(QString("file:///%1").arg(filepath+"1.png"));
        m_playList.append(QString("file:///%1").arg(filepath+"13.png"));
        GetPlayitem();

    }else if(code == "I17DR02E0003SS0F")
    {
        m_playList.clear();
        m_playList.append(QString("file:///%1").arg(filepath+"4.png"));
        m_playList.append(QString("file:///%1").arg(filepath+"3.png"));
        m_playList.append(QString("file:///%1").arg(filepath+"33.png"));
        GetPlayitem();
    }

    else if(code == "I17DR02E0004SS0F")
    {
        m_playList.clear();
        m_playList.append(QString("file:///%1").arg(filepath+"6.png"));
        m_playList.append(QString("file:///%1").arg(filepath+"5.png"));
        m_playList.append(QString("file:///%1").arg(filepath+"53.png"));
        GetPlayitem();
    }
    else if(code == "I17DR02E0005SS0F"||code == "I17DR02E0006SS0F")
    {
        m_playList.clear();
        m_playList.append(QString("file:///%1").arg(filepath+"8.png"));
        m_playList.append(QString("file:///%1").arg(filepath+"7.png"));
        m_playList.append(QString("file:///%1").arg(filepath+"73.png"));
        GetPlayitem();
    }
}






void Netmanager::Getstorequeu(const QString &storeid)
{
    m_state = GET_FITTING;

    QString url = "http://crazyfitting.ur.com.cn/queue/base/";
    QString content = QString("fittingarea/list/data/by/store?storeId=%1").arg(storeid);
    url.append(content);


    update_manager->get(QNetworkRequest(QUrl(url)));



}



void Netmanager::GetFittingList(const QString &storeId ,const QString &fittingAreaId )
{

    m_state = GET_FITTINGLIST;


    QString url = "http://crazyfitting.ur.com.cn/";
    QString content = QString("/queue/normal/buffer/list/data?storeId=%1&&fittingAreaId=%2").arg(storeId).arg(fittingAreaId);
    url.append(content);
    update_manager->get(QNetworkRequest(QUrl(url)));

}


void Netmanager::GetFittingInfo(const QString &storeId , const QString &userFlag)
{
    m_state = GET_FITTINGINFO;

    QString url = "http://crazyfitting.ur.com.cn/";
    QString content = QString("/queue/normal/info?storeId=%1&userFlag=%2&bufferData=true").arg(storeId).arg(userFlag);
    url.append(content);

    QNetworkRequest req;
    req.setUrl(QUrl(url));


    update_manager->get(req);
}


void Netmanager::Getplayresource()
{
    m_state = GET_PLAYRESOURCR;

    QString url = "http://crazyfitting.ur.com.cn/";
    QString content = QString("queue/base/get/playForce/resource?storeId=%1&playMode=1").arg(storeID);
    url.append(content);

    QNetworkRequest req;
    req.setUrl(QUrl(url));


    update_manager->get(req);
    qDebug() << "send Getplayresource get request!!!";

}


void Netmanager::getdownloadfiles()
{
    m_state = GET_DOWNLOADFILE;

    QString url = "http://crazyfitting.ur.com.cn/" + m_downloadfiles.first();


    dowmreply = update_manager->get(QNetworkRequest(QUrl(url)));

    connect(dowmreply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));

    downloadTime.restart();
    m_nTime = 0;
}



void Netmanager::Getpersonnumber(const QString number)
{
    m_state = GET_PERSONNUMBER;

    QString url = "http://crazyfitting.ur.com.cn/";
    QString content = QString("/queue/normal/get/by/NO?storeId=%1&no=%2").arg(storeID).arg(number.right(5));
    url.append(content);

    QNetworkRequest req;
    req.setUrl(QUrl(url));


    update_manager->get(req);

}


QString Netmanager::GetStoreID()
{
    return storeID;
}

void Netmanager::GetPlayitem()
{
    if(m_playList.isEmpty())
    {
        QString filepath = QDir::toNativeSeparators(
                    qApp->applicationDirPath()
                    + QDir::separator()
                    +"Playresource"
                    + QDir::separator());

        QDir dir(filepath);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList filelist = dir.entryInfoList();
        for(int j = 0 ; j < filelist.count() ; j++)
        {
            //            qDebug() << filelist.at(j).absoluteFilePath();
            m_playList.append(QString("file:///%1").arg(filelist.at(j).absoluteFilePath()));
        }

        if(!m_playList.isEmpty())
        {
            //            return m_playList.takeFirst();
            emit playitem(m_playList.takeFirst());
        }else{
            //            return QString();
            emit playitem("");
        }
    }else{
        //        return m_playList.takeFirst();
        qDebug() << m_playList.first();
        emit playitem(m_playList.takeFirst());

    }

}

bool Netmanager::compareData(const waitinformation &Amount1, const waitinformation &Amount2)
{
    if (Amount1.creattime < Amount2.creattime)
    {
        return true;
    }
    return false;
}


void Netmanager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    // 总时间
    int nTime = downloadTime.elapsed();

    // 本次下载所用时间
    nTime -= m_nTime;

    // 下载速度
    double dBytesSpeed = (bytesReceived * 1000.0) / nTime;
    double dSpeed = dBytesSpeed;

    //剩余时间
    qint64 leftBytes = (bytesTotal - bytesReceived);
    double dLeftTime = (leftBytes * 1.0) / dBytesSpeed;




    double bt = bytesTotal *1.0;
    double br = bytesReceived *1.0;
    double value = br / bt;

    QString nettext = QString("下载速度 %1  当前下载大小 : %2   文件大小 : %3 ").arg(speed(dSpeed)).arg(size(bytesReceived)).arg(size(bytesTotal));


    emit downloadmsg(value,nettext);
    //    m_pSpeedInfoLabel->setText(speed(dSpeed));
    //    m_pLeftTimeInfoLabel->setText(timeFormat(qCeil(dLeftTime)));
    //    m_pFileSizeInfoLabel->setText(size(bytesTotal));
    //    m_pDownloadInfoLabel->setText(size(bytesReceived));
    //    m_pProgressBar->setMaximum(bytesTotal);
    //    m_pProgressBar->setValue(bytesReceived);

    // 获取上一次的时间
    m_nTime = nTime;
}


void Netmanager::shootmsg()
{
    if(msgnum < msgvector.count())
    {
        QString code = msgvector.at(msgnum).code;
        QString aera = msgvector.at(msgnum).aera;
        QString msg = QString("<font>%1</font>  Please proceed to Area <font>%2</font> to standby").arg(code).arg(aera);

        emit displaymsg(msg,code,aera);
        msgnum++;
        if(msgnum == msgvector.count())
        {
            msgnum = 0;
        }
    }else if(msgvector.count() == 0)
    {
        emit displaymsgnull();
    }




}

void Netmanager::shootdis()
{
    if(m_clothesinfo.count()>0)
    {
        emit disclothes(m_clothesinfo.takeFirst());
    }else{
        disTimer.stop();
        GetPlayitem();
    }
}

bool Netmanager::updating()
{
    return updatestate;
}

QString Netmanager::GetLocationNum()
{
    return LocationID;
}
