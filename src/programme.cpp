#include "programme.h"
#include <QDebug>

Programme::Programme()
{
    name = "";
    length = 0.0f;
    progId = 0u;
}

Programme::Programme(unsigned int progId, QString name, float length) :
    name(name), length(length), progId(progId)
{
}

QString Programme::getName () const {
    return name;
}

float Programme::getLength () const {
    return length;
}

unsigned int Programme::getProgId () const {
    return progId;
}

void Programme::setName (const QString &value) {
    name = value;
}


void Programme::setLength (float value) {
    length = value;
}

void Programme::setProgId (unsigned int value) {
    progId = value;
}

