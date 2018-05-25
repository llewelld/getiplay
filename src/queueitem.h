#ifndef QUEUEITEM_H
#define QUEUEITEM_H

#include <QObject>

class QueueItem
{
public:
    enum TYPE {
        TYPE_INVALID = -1,

        TYPE_RADIO,
        TYPE_TV,

        TYPE_NUM
    };

    enum STATUS {
        STATUS_INVALID = -1,

        STATUS_ERROR,
        STATUS_REMOTE,
        STATUS_DOWNLOADING,
        STATUS_LOCAL,

        STATUS_NUM
    };

    QueueItem();
    QueueItem(QString progId, QString name, quint32 duration, STATUS status, TYPE type);

    QString getName () const;
    quint32 getDuration() const;
    QString getProgId () const;
    STATUS getStatus () const;
    float getProgress () const;
    TYPE getType () const;
    QString getTypeString () const;

    void setName (const QString &value);
    void setDuration (quint32 value);
    void setProgId (QString value);
    void setStatus (STATUS status);
    void setProgress (float progress);
    void setType (TYPE type);

private:
    QString name;
    quint32 duration;
    QString progId;
    STATUS status;
    float progress;
    TYPE type;
};


#endif // QUEUEITEM_H
