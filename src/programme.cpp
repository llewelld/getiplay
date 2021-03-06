#include "programme.h"
#include <QDebug>

Programme::Programme() :
    name(""),
    duration(0u),
    progId(""),
    episode(""),
    available(0u),
    channel(""),
    web(""),
    description(""),
    imageId("")
{
}

Programme::Programme(QString progId, QString name, qint32 duration, qint64 available, QString channel, QString episode, QString web, QString description, QString imageId) :
    name(name),
    duration(duration),
    progId(progId),
    episode(episode),
    available(available),
    channel(channel),
    web(web),
    description(description),
    imageId(imageId)
{
}

QString Programme::getName () const {
    return name;
}

qint32 Programme::getDuration () const {
    return duration;
}

QString Programme::getProgId () const {
    return progId;
}

qint64 Programme::getAvailable () const {
    return available;
}

QString Programme::getChannel () const {
    return channel;
}

QString Programme::getEpisode () const {
    return episode;
}

QString Programme::getWeb () const {
    return web;
}

QString Programme::getDescription () const {
    return description;
}

QString Programme::getImageId () const {
    return imageId;
}

void Programme::setName (const QString &value) {
    name = value;
}

void Programme::setDuration (qint32 value) {
    duration = value;
}

void Programme::setProgId (unsigned int value) {
    progId = value;
}

void Programme::setAvailable(qint64 value) {
    available = value;
}

void Programme::setChannel(QString value) {
    channel = value;
}

void Programme::setEpisode(QString value) {
    episode = value;
}

void Programme::setWeb(QString value) {
    web = value;
}

void Programme::setDescription(QString value) {
    description = value;
}

void Programme::setImageId(QString value) {
    imageId = value;
}

