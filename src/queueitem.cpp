#include "queueitem.h"
#include <QDebug>

QueueItem::QueueItem() :
    name(""),
    duration(0u),
    progId(""),
    status(Queue::STATUS_REMOTE),
    progress(0.0f),
    type(QueueItem::TYPE_INVALID)
{
}

QueueItem::QueueItem(QString progId, QString name, quint32 duration, Queue::STATUS status, QueueItem::TYPE type) :
    name(name),
    duration(duration),
    progId(progId),
    status(status),
    progress(0.0f),
    type(type)
{
}

QString QueueItem::getName () const {
    return name;
}

quint32 QueueItem::getDuration () const {
    return duration;
}

QString QueueItem::getProgId () const {
    return progId;
}

Queue::STATUS QueueItem::getStatus () const {
    return status;
}

float QueueItem::getProgress () const {
    float progress;
    progress = qBound(0.0f, this->progress, 1.0f);
    if (status <= Queue::STATUS_REMOTE) {
        progress = 0.0;
    }
    if (status >= Queue::STATUS_LOCAL) {
        progress = 1.0;
    }
    return progress;
}

QueueItem::TYPE QueueItem::getType () const {
    return type;
}

QString QueueItem::getTypeString () const {
    QString typestring;
    const QString names[TYPE_NUM] = {"radio", "tv"};

    if ((type > TYPE_INVALID) && (type < TYPE_NUM)) {
        typestring = names[type];
    }
    else {
        typestring = "invalid";
    }

    return typestring;
}

void QueueItem::setName (const QString &value) {
    name = value;
}


void QueueItem::setDuration (quint32 value) {
    duration = value;
}

void QueueItem::setProgId (QString value) {
    progId = value;
}

void QueueItem::setStatus (Queue::STATUS value) {
    status = value;
}

void QueueItem::setProgress (float value) {
    progress = value;
}

void QueueItem::setType (TYPE value) {
    type = value;
}
