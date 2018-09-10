#ifndef DOWNLOADLIST_H
#define DOWNLOADLIST_H

#include <QObject>

class Downloadlist : public QObject
{
    Q_OBJECT
public:
    explicit Downloadlist(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DOWNLOADLIST_H
