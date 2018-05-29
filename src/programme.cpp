#include "programme.h"
#include <QDebug>

Programme::Programme() :
    name(""),
    duration(0u),
    progId(""),
    timeadded(0u),
    channel(""),
    episode(""),
    web(""),
    description("")
{
}

Programme::Programme(QString progId, QString name, qint32 duration, qint64 timeadded, QString channel, QString episode, QString web, QString description) :
    name(name),
    duration(duration),
    progId(progId),
    timeadded(timeadded),
    channel(channel),
    episode(episode),
    web(web),
    description(description)
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

qint64 Programme::getTimeAdded () const {
    return timeadded;
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

void Programme::setName (const QString &value) {
    name = value;
}

void Programme::setDuration (qint32 value) {
    duration = value;
}

void Programme::setProgId (unsigned int value) {
    progId = value;
}

void Programme::setTimeAdded(qint64 value) {
    timeadded = value;
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
