#ifndef QUEUE_H
#define QUEUE_H

#include <QObject>
#include <QFileSystemWatcher>
#include "download.h"
#include "logfile.h"

class QueueModel;
class QueueItem;

class Queue : public QObject
{
    Q_OBJECT
    Q_ENUMS(STATUS)

public:
    enum STATUS {
        STATUS_INVALID = -1,

        STATUS_UNQUEUED,
        STATUS_ERROR,
        STATUS_REMOTE,
        STATUS_DOWNLOADING,
        STATUS_LOCAL,
        STATUS_DELETED,

        STATUS_NUM
    };

private:
    // Internal methods
    void takeAction();
    void setActiveStatus(STATUS status);
    STATUS downloadStatusToQueueStatus(DOWNLOADSTATUS download);
    void addFileWatch(QString filename);
    void removeFileWatch(QString filename);

    // Internal attributes
    QueueModel * model;
    QString downloadingId;
    Download * download;
    QueueItem * active;
    QFileSystemWatcher filewatcher;

public:
    explicit Queue(QObject *parent = 0, Download *download = 0);
    void setModel(QueueModel * model);

    Q_INVOKABLE bool addToQueue(QString progid, QString name, quint32 duration, int type, QString episode, qint64 available, QString channel, QString web, QString description, QString imageid);
    Q_INVOKABLE int getStatusFromId(QString progid);
    Q_INVOKABLE void removeFromQueue(QString progid);
    Q_INVOKABLE void deleteAndRemoveFromQueue(QString progid);
    Q_INVOKABLE QVariant getDetails(QString progid);

signals:
    void statusChanged(QString progidchanged, int status);

private slots:
    void statusChanged(int status);
    void progressChanged(float progress);
    void fileChanged(const QString &path);
};

#endif // QUEUE_H
