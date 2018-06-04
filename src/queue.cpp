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
    active(nullptr),
    filewatcher(this),
    downloading(0),
    completed(0)
{
    connect(download, SIGNAL(statusChanged(int)), this, SLOT(statusChanged(int)));
    connect(download, SIGNAL(progressChanged(float)), this, SLOT(progressChanged(float)));
    connect(&filewatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}

void Queue::setModel(QueueModel * model) {
    // Clear out all files currently being watched
    filewatcher.removePaths(filewatcher.files());

    this->model = model;

    // Add in the new file paths
    model->monitorPaths(filewatcher);
    setCompleted(model->getCompleted());
    setDownloading(model->rowCount() - completed);

    takeAction();
}

bool Queue::addToQueue(QString progid, QString name, quint32 duration, int type, QString episode, qint64 available, QString channel, QString web, QString description, QString imageid) {
    QueueItem * found;
    bool added;

    added = false;
    qDebug() << "Adding " << progid << " to queue";

    // Check whether it's already there
    found = model->findFromId(progid);

    if (found == nullptr) {
        model->addProgramme(new QueueItem(progid, name, duration, Queue::STATUS_REMOTE, static_cast<QueueItem::TYPE>(type), "", episode, available, channel, web, description, imageid));
        emit statusChanged(progid, Queue::STATUS_REMOTE);
        incrementDownloading();
        takeAction();
        added = true;
    }

    return added;
}

void Queue::removeFromQueue(QString progid) {
    qDebug() << "Remove " << progid << " from queue";
    QueueItem * item = model->findFromId(progid);
    if (item != nullptr) {
        if (item->getCompleted()) {
            decrementCompleted();
        }
        else {
            decrementDownloading();
        }
        removeFileWatch(item->getFilename());

        model->removeFirstWithProgId(progid);
    }
}

void Queue::deleteAndRemoveFromQueue(QString progid) {
    qDebug() << "Delete " << progid;

    QueueItem * item = model->findFromId(progid);
    if (item != nullptr) {
        if (item->getCompleted()) {
            decrementCompleted();
        }
        else {
            decrementDownloading();
        }
        removeFileWatch(item->getFilename());

        item->deleteFile();
        model->removeFirstWithProgId(progid);
    }
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
            downloadingId = active->getProgId();
            setActiveStatus(Queue::STATUS_DOWNLOADING);
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
    Queue::STATUS queuestatus;

    qDebug() << "Queue download status changed to: " << status;
    queuestatus = downloadStatusToQueueStatus(static_cast<DOWNLOADSTATUS>(status));

    // We must update the details before setting the new status
    if ((status == DOWNLOADSTATUS_DONE) && (active != nullptr)){
        // Extract the resulting info
        quint32 duration = download->getDuration();
        active->setDuration(duration);
        QString filename = download->getFilename();
        active->setFilename(filename);
        addFileWatch(filename);
    }

    setActiveStatus(queuestatus);

    if (status >= DOWNLOADSTATUS_CANCEL) {
        // We're done with this download; move on to the next
        active = nullptr;
        decrementDownloading();
        incrementCompleted();
        takeAction();
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
            model->refreshItem(active);
            if (downloadingId != "") {
                emit statusChanged(downloadingId, status);
            }
        }
    }
}

void Queue::addFileWatch(QString filename) {
    if (filename != "") {
        filewatcher.addPath(filename);
    }
}

void Queue::removeFileWatch(QString filename) {
    if (filename != "") {
        filewatcher.removePath(filename);
    }
}

void Queue::fileChanged(const QString &path) {
    // Search for the filename
    QString found = model->removePath(path);
    if (found != "") {
        emit statusChanged(found, Queue::STATUS_DELETED);
        removeFileWatch(path);
    }
}

QVariant Queue::getDetails(QString progid) {
    QVariantMap details;

    QueueItem const * item = model->findFromId(progid);

    if (item != nullptr) {
        details.insert("filename", item->getFilename());
    }

    return details;
}

unsigned int Queue::getCompleted() {
    return completed;
}

unsigned int Queue::getDownloading() {
    return downloading;
}

void Queue::setCompleted(unsigned int value) {
    completed = value;
    emit completedChanged(value);
}

void Queue::setDownloading(unsigned int value) {
    downloading = value;
    emit downloadingChanged(value);
}

void Queue::incrementDownloading() {
    emit downloadingChanged(++downloading);
}

void Queue::decrementDownloading() {
    emit downloadingChanged(--downloading);
}

void Queue::incrementCompleted() {
    emit completedChanged(++completed);
}

void Queue::decrementCompleted() {
    emit completedChanged(--completed);
}
