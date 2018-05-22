#include "queueitem.h"
#include <QDebug>

QueueItem::QueueItem() :
    name(""),
    length(0.0f),
    progId(0u),
    status(QueueItem::STATUS_REMOTE),
    progress(0.0f),
    type(QueueItem::TYPE_INVALID)
{
}

QueueItem::QueueItem(unsigned int progId, QString name, float length, QueueItem::STATUS status, QueueItem::TYPE type) :
    name(name),
    length(length),
    progId(progId),
    status(status),
    progress(0.0f),
    type(type)
{
}

QString QueueItem::getName () const {
    return name;
}

float QueueItem::getLength () const {
    return length;
}

unsigned int QueueItem::getProgId () const {
    return progId;
}

QueueItem::STATUS QueueItem::getStatus () const {
    return status;
}

float QueueItem::getProgress () const {
    float progress;
    progress = qBound(0.0f, this->progress, 1.0f);
    if (status <= QueueItem::STATUS_REMOTE) {
        progress = 0.0;
    }
    if (status >= QueueItem::STATUS_LOCAL) {
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


void QueueItem::setLength (float value) {
    length = value;
}

void QueueItem::setProgId (unsigned int value) {
    progId = value;
}

void QueueItem::setStatus (QueueItem::STATUS value) {
    status = value;
}

void QueueItem::setProgress (float value) {
    progress = value;
}

void QueueItem::setType (TYPE value) {
    type = value;
}
