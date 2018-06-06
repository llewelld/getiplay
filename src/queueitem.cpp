#include <QDebug>
#include <QFileInfo>

#include "settings.h"
#include "queueitem.h"

QueueItem::QueueItem() :
    name(""),
    duration(0u),
    progId(""),
    status(Queue::STATUS_REMOTE),
    progress(0.0f),
    type(QueueItem::TYPE_INVALID),
    filename(""),
    episode(""),
    available(0u),
    channel(""),
    web(""),
    description(""),
    imageid("")
{
}

QueueItem::QueueItem(QString progId, QString name, quint32 duration, Queue::STATUS status, QueueItem::TYPE type, QString filename, QString episode, qint64 available, QString channel, QString web, QString description, QString imageid) :
    name(name),
    duration(duration),
    progId(progId),
    status(status),
    progress(0.0f),
    type(type),
    filename(filename),
    episode(episode),
    available(available),
    channel(channel),
    web(web),
    description(description),
    imageid(imageid)
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

QString QueueItem::getFilename () const {
    return filename;
}

QString QueueItem::getEpisode () const {
    return episode;
}

qint64 QueueItem::getAvailable() const {
    return available;
}

QString QueueItem::getChannel() const {
    return channel;
}

QString QueueItem::getWeb() const {
    return web;
}

QString QueueItem::getDescription() const {
    return description;;
}

QString QueueItem::getImageId() const {
    return imageid;
}

void QueueItem::setName (const QString &value) {
    name = value;
}

void QueueItem::setDuration (quint32 value) {
    duration = value;
}

void QueueItem::setProgId (QString &value) {
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

void QueueItem::setFilename (QString &value) {
    filename = value;
}

void QueueItem::setEpisode (QString &value) {
    episode = value;
}

void QueueItem::setAvailable(qint64 value) {
    available = value;
}

void QueueItem::setChannel (QString &value) {
    channel = value;
}

void QueueItem::setWeb (QString &value) {
    web = value;
}

void QueueItem::setDescription (QString &value) {
    description = value;
}

void QueueItem::setImageId (QString &value) {
    imageid = value;
}

bool QueueItem::fileExists() {
    bool exists = false;

    qDebug() << "Checking file existence for: " << name;
    qDebug() << "Filename: " << filename;
    if (filename != "") {
        exists = QFile(filename).exists();
        qDebug() << "File check returned: " << exists;
    }

    return exists;
}

bool QueueItem::deleteFile() {
    qDebug() << "Deleting file " << filename;
    bool deleted = false;
    if (filename != "") {
        QFile file(filename);
        QFileInfo fileinfo(file);

        // Only delete files, not folders or anything else
        if (fileinfo.isFile()) {
            qDebug() << "Performing the deletion";
            file.remove();
            deleted = true;
        }
    }

    return deleted;
}

bool QueueItem::getCompleted() {
    bool completed = true;
    switch (status) {
    case Queue::STATUS_DOWNLOADING:
    case Queue::STATUS_REMOTE:
        completed = false;
        break;
    default:
        completed = true;
        break;
    }
    return completed;
}
