#ifndef QUEUE_H
#define QUEUE_H

#include <QObject>
#include "queuemodel.h"
#include "download.h"
#include "logfile.h"

class Queue : public QObject
{
    Q_OBJECT

private:
    // Internal methods
    void takeAction();
    void setActiveStatus(QueueItem::STATUS status);

    // Internal attributes
    QueueModel * model;
    QString downloadingId;
    Download * download;
    QueueItem * active;

public:
    explicit Queue(QObject *parent = 0, Download *download = 0);
    void setModel(QueueModel * model);

signals:
    void statusChanged(QString progidchanged, int status);

public slots:
    // General methods
    void addToQueue(QString progid, QString name, quint32 duration, int type);
    int getStatusFromId(QString progid);

private slots:
    void statusChanged(int status);
    void progressChanged(float progress);
};

#endif // QUEUE_H
