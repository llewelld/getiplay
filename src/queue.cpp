#include <QDebug>
#include "harbour-getiplay.h"
#include "settings.h"
#include "queue.h"

Queue::Queue(QObject *parent, Download *download) :
    QObject(parent),
    model(nullptr),
    downloadingId(""),
    download(download),
    active(nullptr)
{
    connect(download, SIGNAL(statusChanged(int)), this, SLOT(statusChanged(int)));
    connect(download, SIGNAL(progressChanged(float)), this, SLOT(progressChanged(float)));
}

void Queue::setModel(QueueModel * model) {
    this->model = model;
    takeAction();
}

void Queue::addToQueue(QString progid, QString name, quint32 duration, int type) {
    model->addProgramme(new QueueItem(progid, name, duration, QueueItem::STATUS_REMOTE, static_cast<QueueItem::TYPE>(type)));
    takeAction();
}

void Queue::takeAction() {
    DOWNLOADSTATUS status;
    bool running;

    qDebug() << "Queue taking action";

    status = download->getStatus();
    qDebug() << "Download status: " << status;
    running = true;
    if ((status <= DOWNLOADSTATUS_UNINITIALISED) || (status >= DOWNLOADSTATUS_CANCEL)) {
        running = false;
        qDebug() << "No download in progress";
    }
    if ((active == nullptr) && (model->rowCount() > 0) && (running == false)) {
        // Find the next remote item in the queue
        qDebug() << "Searching for next item in queue";
        active = model->findNextRemote();
        if (active != nullptr) {
            qDebug() << "Found remote programme in queue. Downloading.";
            // Start downloading the next item in the queue
            setActiveStatus(QueueItem::STATUS_DOWNLOADING);
            downloadingId = active->getProgId();
            download->startDownload(downloadingId, active->getTypeString().toLocal8Bit());
        }
    }
}

void Queue::statusChanged(int status) {
    qDebug() << "Queue download status changed to: " << status;
    if (status >= DOWNLOADSTATUS_CANCEL) {
        if (active != nullptr) {
            model->refreshItem(active);
            setActiveStatus(QueueItem::STATUS_LOCAL);
            active = nullptr;
        }
        takeAction();
    }
    if (downloadingId != "") {
        emit statusChanged(downloadingId, status);
    }
}

void Queue::progressChanged(float progress) {
    if (active != nullptr) {
        active->setProgress(progress);
        model->refreshItem(active);
    }
}

int Queue::getStatusFromId(QString progid) {
    QueueItem * programme;
    QueueItem::STATUS status;

    programme = model->findFromId(progid);
    if (programme) {
        status = programme->getStatus();
    }
    else {
        status = QueueItem::STATUS_REMOTE;
    }

    return status;
}

void Queue::setActiveStatus(QueueItem::STATUS status) {
    if (active != nullptr) {
        active->setStatus(status);
        if (downloadingId != "") {
            emit statusChanged(downloadingId, status);
        }
    }
}

