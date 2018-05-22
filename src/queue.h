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
    int downloadingId;
    Download * download;
    QueueItem * active;

public:
    explicit Queue(QObject *parent = 0, Download *download = 0);
    void setModel(QueueModel * model);

signals:
    void statusChanged(unsigned int progidchanged, int status);

public slots:
    // General methods
    void addToQueue(unsigned int progid, QString name, float duration, int type);
    int getStatusFromId(unsigned int progid);

private slots:
    void statusChanged(int status);
    void progressChanged(float progress);
};

#endif // QUEUE_H
