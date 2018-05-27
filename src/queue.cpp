#include <QDebug>
#include "harbour-getiplay.h"
#include "settings.h"
#include "queue.h"
#include "queueitem.h"
#include "queuemodel.h"

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

bool Queue::addToQueue(QString progid, QString name, quint32 duration, int type) {
    QueueItem * found;
    bool added;

    added = false;
    qDebug() << "Adding " << progid << " to queue";

    // Check whether it's already there
    found = model->findFromId(progid);

    if (found == nullptr) {
        model->addProgramme(new QueueItem(progid, name, duration, Queue::STATUS_REMOTE, static_cast<QueueItem::TYPE>(type)));
        emit statusChanged(progid, Queue::STATUS_REMOTE);
        takeAction();
        added = true;
    }

    return added;
}

void Queue::removeFromQueue(QString progid) {
    qDebug() << "Remove " << progid << " from queue";
    model->removeFirstWithProgId(progid);
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
            setActiveStatus(Queue::STATUS_DOWNLOADING);
            downloadingId = active->getProgId();
            download->startDownload(downloadingId, active->getTypeString().toLocal8Bit());
        }
    }
}

Queue::STATUS Queue::downloadStatusToQueueStatus(DOWNLOADSTATUS download) {
    Queue::STATUS status;

    switch (download) {
    case DOWNLOADSTATUS_UNINITIALISED:
        status = Queue::STATUS_REMOTE;
        break;
    case DOWNLOADSTATUS_INITIALISING:
    case DOWNLOADSTATUS_DOWNLOADING:
    case DOWNLOADSTATUS_CONVERTING:
        status = Queue::STATUS_DOWNLOADING;
        break;
    case DOWNLOADSTATUS_DONE:
        status = Queue::STATUS_LOCAL;
        break;
    default:
    case DOWNLOADSTATUS_CANCEL:
    case DOWNLOADSTATUS_ERROR:
        status = Queue::STATUS_ERROR;
        break;
    }

    return status;
}


void Queue::statusChanged(int status) {
    qDebug() << "Queue download status changed to: " << status;
    if (status >= DOWNLOADSTATUS_CANCEL) {
        if (active != nullptr) {
            setActiveStatus(Queue::STATUS_LOCAL);
            model->refreshItem(active);
            active = nullptr;
        }
        takeAction();
    }
    else {
        if (downloadingId != "") {
            Queue::STATUS queuestatus = downloadStatusToQueueStatus(static_cast<DOWNLOADSTATUS>(status));
            emit statusChanged(downloadingId, queuestatus);
        }
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
    Queue::STATUS status;

    programme = model->findFromId(progid);
    if (programme) {
        status = programme->getStatus();
    }
    else {
        status = Queue::STATUS_UNQUEUED;
    }

    return status;
}

void Queue::setActiveStatus(Queue::STATUS status) {
    if (active != nullptr) {
        if (status != active->getStatus()) {
            active->setStatus(status);
            if (downloadingId != "") {
                emit statusChanged(downloadingId, status);
            }
        }
    }
}

