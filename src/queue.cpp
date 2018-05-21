#include <QDebug>
#include "harbour-getiplay.h"
#include "settings.h"
#include "queue.h"

Queue::Queue(QObject *parent) :
    QObject(parent),
    model(nullptr)
{
}

void Queue::setModel(ProgModel * model) {
    this->model = model;
}

void Queue::addToQueue(unsigned int progid, QString name, float duration) {
    model->addProgramme(Programme(progid, name, duration));
}

