#ifndef NETMANAGER_H
#define NETMANAGER_H

#include <QtNetwork>
#include <QObject>
#include <QFileInfo>
#include <QMap>
#include <QSettings>
#include <QDir>

class Netmanager : public QObject
{
    Q_OBJECT
    //    Q_PROPERTY(QStringList clientList READ clientList  NOTIFY clientListChanged)
    //    Q_PROPERTY(QStringList storeList READ storeList  NOTIFY storeListChanged)
    //    Q_PROPERTY(QStringList deviceList READ deviceList  NOTIFY deviceListChanged)
public:
    Netmanager();
    bool DeleteDirectory(const QString &path);

    //    Q_INVOKABLE void update_Clothes(const QString &clientId , const QString &barcode ,const QString & deviceId , const QString &storeId);

    Q_INVOKABLE void Getstorequeu(const QString &storeid);
    Q_INVOKABLE void GetFittingList(const QString &storeId , const QString &fittingAreaId );
    Q_INVOKABLE void GetFittingInfo(const QString &storeId , const QString &userFlag = 000);
    Q_INVOKABLE void Getplayresource();
    Q_INVOKABLE QString GetStoreID();
    Q_INVOKABLE void GetPlayitem();
    Q_INVOKABLE void Getpersonnumber(const QString number);
    Q_INVOKABLE void Getcurrentclothes(const QString&code);
    Q_INVOKABLE void Getclothesinfo(const QString &clothesid);
    Q_INVOKABLE QString GetLocationNum();
    void getdownloadfiles();
    void compareversion();
    bool updating();


    enum NetState
    {
        GET_FITTING,
        GET_FITTINGLIST,
        GET_FITTINGINFO,
        GET_PLAYRESOURCR,
        GET_DOWNLOADFILE,
        GET_PERSONNUMBER,
        GET_CLOTHESINFO,
        COMPAREVER
    };

    struct waitinformation{
        QString zoom;
        QString code;
        int creattime;
    };

    struct Clothesinfo{
        QString clothespath;
        QString barcode;
        QString name;
        QString info;
        QString price;
    };

    struct Msginfo{
        QString code;
        QString aera;
    };
signals:
    void zoneswaitnumber(QString number , QString ftimes , QString mtimes);
    void zoneswaitanumber(QString number , QString roomnumber);
    void zoneswaitbnumber(QString number , QString roomnumber);
    void zoneswaitcnumber(QString number , QString roomnumber);
    void playitem(QString itempath);
    void filedowncomplete();
    void usercurrentnum(QString number,QString timestr);
    void downloadmsg(double value,QString msg);
    void displaymsg(QString msg , QString code , QString aera);
    void disclothes(QVariantMap info);
    void comparecomplete();
    void exequit();
    void displaymsgnull();

public slots:


private slots:
    void readNetworkReply(QNetworkReply *reply);
    void downloadProgress(qint64, qint64);
    void shootmsg();
    void shootdis();
private:
    QNetworkAccessManager *update_manager;
    QNetworkReply * dowmreply;
    QVector<QString> m_downloadfiles;
    QStringList m_playList;
    NetState m_state;
    QString storeID;
    QString LocationID;
    QTime downloadTime;
    int m_nTime = 0;
    QTimer msgTimer;
    QTimer disTimer;
    QList<waitinformation> m_information;
    QList<QVariantMap> m_clothesinfo;
    int msgnum = 0;
    QVector<Msginfo> msgvector;
    bool updatestate = false;





    QString size(qint64 bytes)
    {
        QString strUnit;
        double dSize = bytes * 1.0;
        if (dSize <= 0)
        {
            dSize = 0.0;
        }
        else if (dSize < 1024)
        {
            strUnit = "Bytes";
        }
        else if (dSize < 1024 * 1024)
        {
            dSize /= 1024;
            strUnit = "KB";
        }
        else if (dSize < 1024 * 1024 * 1024)
        {
            dSize /= (1024 * 1024);
            strUnit = "MB";
        }
        else
        {
            dSize /= (1024 * 1024 * 1024);
            strUnit = "GB";
        }

        return QString("%1 %2").arg(QString::number(dSize, 'f', 2)).arg(strUnit);
    }

    // 速度转KB/S、MB/S、GB/S
    QString speed(double speed)
    {
        QString strUnit;
        if (speed <= 0)
        {
            speed = 0;
            strUnit = "Bytes/S";
        }
        else if (speed < 1024)
        {
            strUnit = "Bytes/S";
        }
        else if (speed < 1024 * 1024)
        {
            speed /= 1024;
            strUnit = "KB/S";
        }
        else if (speed < 1024 * 1024 * 1024)
        {
            speed /= (1024 * 1024);
            strUnit = "MB/S";
        }
        else
        {
            speed /= (1024 * 1024 * 1024);
            strUnit = "GB/S";
        }

        QString strSpeed = QString::number(speed, 'f', 2);
        return QString("%1 %2").arg(strSpeed).arg(strUnit);
    }

    // 秒转*d *h *m *s
    QString timeFormat(int seconds)
    {
        QString strValue;
        QString strSpacing(" ");
        if (seconds <= 0)
        {
            strValue = QString("%1s").arg(0);
        }
        else if (seconds < 60)
        {
            strValue = QString("%1s").arg(seconds);
        }
        else if (seconds < 60 * 60)
        {
            int nMinute = seconds / 60;
            int nSecond = seconds - nMinute * 60;

            strValue = QString("%1m").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1s").arg(nSecond);
        }
        else if (seconds < 60 * 60 * 24)
        {
            int nHour = seconds / (60 * 60);
            int nMinute = (seconds - nHour * 60 * 60) / 60;
            int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

            strValue = QString("%1h").arg(nHour);

            if (nMinute > 0)
                strValue += strSpacing + QString("%1m").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1s").arg(nSecond);
        }
        else
        {
            int nDay = seconds / (60 * 60 * 24);
            int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
            int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
            int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

            strValue = QString("%1d").arg(nDay);

            if (nHour > 0)
                strValue += strSpacing + QString("%1h").arg(nHour);

            if (nMinute > 0)
                strValue += strSpacing + QString("%1m").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1s").arg(nSecond);
        }

        return strValue;
    }

    static bool compareData(const waitinformation &Amount1, const waitinformation &Amount2);


};

#endif // NETMANAGER_H
