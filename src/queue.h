#ifndef QUEUE_H
#define QUEUE_H

#include <QObject>
#include "progmodel.h"
#include "logfile.h"

class Queue : public QObject
{
    Q_OBJECT

private:
    // Internal methods
    // Internal attributes
    ProgModel * model;

public:
    explicit Queue(QObject *parent = 0);
    void setModel(ProgModel * model);

signals:

public slots:
    // General methods
    void addToQueue(unsigned int progid, QString name, float duration);
};

#endif // QUEUE_H
