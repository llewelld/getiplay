#ifndef QUEUEITEM_H
#define QUEUEITEM_H

#include <QObject>
#include "queue.h"

class QueueItem
{
public:
    enum TYPE {
        TYPE_INVALID = -1,

        TYPE_RADIO,
        TYPE_TV,

        TYPE_NUM
    };

    QueueItem();
    QueueItem(QString progId, QString name, quint32 duration, Queue::STATUS status, TYPE type, QString filename, QString episode, qint64 available, QString channel, QString web, QString description);

    QString getName () const;
    quint32 getDuration() const;
    QString getProgId () const;
    Queue::STATUS getStatus () const;
    float getProgress () const;
    TYPE getType () const;
    QString getTypeString () const;
    QString getFilename () const;
    QString getEpisode () const;
    qint64 getAvailable() const;
    QString getChannel() const;
    QString getWeb() const;
    QString getDescription() const;

    void setName (const QString &value);
    void setDuration (quint32 value);
    void setProgId (QString &value);
    void setStatus (Queue::STATUS status);
    void setProgress (float progress);
    void setType (TYPE type);
    void setFilename (QString &value);
    void setEpisode (QString &value);
    void setAvailable (qint64 value);
    void setChannel (QString &value);
    void setWeb (QString &value);
    void setDescription (QString &value);

    bool fileExists();
    bool deleteFile();

private:
    QString name;
    quint32 duration;
    QString progId;
    Queue::STATUS status;
    float progress;
    TYPE type;
    QString filename;

    QString episode;
    qint64 available;
    QString channel;
    QString web;
    QString description;
};


#endif // QUEUEITEM_H
