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
    QueueItem(unsigned int progId, QString name, float length, STATUS status, TYPE type);

    QString getName () const;
    float getLength () const;
    unsigned int getProgId () const;
    STATUS getStatus () const;
    float getProgress () const;
    TYPE getType () const;
    QString getTypeString () const;

    void setName (const QString &value);
    void setLength (float value);
    void setProgId (unsigned int value);
    void setStatus (STATUS status);
    void setProgress (float progress);
    void setType (TYPE type);

private:
    QString name;
    float length;
    unsigned int progId;
    STATUS status;
    float progress;
    TYPE type;
};


#endif // QUEUEITEM_H
