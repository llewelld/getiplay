#include <QDebug>

#include "settings.h"
#include "queueitem.h"
#include "queuemodel.h"

QueueModel::QueueModel(QString filename, QObject *parent) : QAbstractListModel(parent),
    filename(filename) {
    roles[ProgIdRole] = "progId";
    roles[NameRole] = "name";
    roles[DurationRole] = "duration";
    roles[StatusRole] = "qstatus";
    roles[ProgressRole] = "progress";
    roles[TypeRole] = "type";
    roles[FilenameRole] = "filename";
    roles[ChannelRole] = "channel";
    roles[DescriptionRole] = "description";
    roles[EpisodeRole] = "episode";
    roles[AvailableRole] = "available";
    roles[WebRole] = "web";
    roles[ImageIdRole] = "imageid";
    roles[PositionRole] = "position";

    if (filename != "") {
        QFile file;
        file.setFileName(filename);
        importFromFile(file);
    }
}

QueueModel::~QueueModel() {
    if (filename != "") {
        qDebug() << "Exporting queue to: " << filename;
        QFile file;
        file.setFileName(filename);
        exportToFile(file);
    }
}

QHash<int, QByteArray> QueueModel::roleNames() const {
    return roles;
}

void QueueModel::addProgramme(QueueItem * programme)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    programmes << programme;
    endInsertRows();
}

void QueueModel::removeFirstWithProgId(QString progid) {
    QList<QueueItem* >::const_iterator iter;
    int index;
    int found;

    index = 0;
    found = -1;
    for (iter = programmes.constBegin(); (found < 0) && (iter != programmes.constEnd()); ++iter) {
        if (((*iter)->getProgId().compare(progid, Qt::CaseSensitive)) == 0) {
            found = index;
        }
        index++;
    }

    if (found >= 0) {
        beginRemoveRows(QModelIndex(), found, found);
        programmes.removeAt(found);
        endRemoveRows();
    }
}

int QueueModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent)
    return programmes.count();
}

QVariant QueueModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > programmes.count())
        return QVariant();

    const QueueItem * programme = programmes[index.row()];
    if (role == ProgIdRole)
        return programme->getProgId();
    else if (role == NameRole)
        return programme->getName();
    else if (role == DurationRole)
        return programme->getDuration();
    else if (role == StatusRole)
        return programme->getStatus();
    else if (role == ProgressRole)
        return programme->getProgress();
    else if (role == TypeRole)
        return programme->getType();
    else if (role == FilenameRole)
        return programme->getFilename();
    else if (role == ChannelRole)
        return programme->getChannel();
    else if (role == DescriptionRole)
        return programme->getDescription();
    else if (role == EpisodeRole)
        return programme->getEpisode();
    else if (role == AvailableRole)
        return programme->getAvailable();
    else if (role == WebRole)
        return programme->getWeb();
    else if (role == ImageIdRole)
        return programme->getImageId();
    else if (role == PositionRole)
        return programme->getPosition();

    return QVariant();
}

void QueueModel::clear() {
    programmes.clear();
}

void QueueModel::exportToFile(QFile & file) {
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        for (QList<QueueItem *>::iterator progIter = programmes.begin(); progIter != programmes.end(); progIter++) {
            out << (*progIter)->getName() << endl;
            out << (*progIter)->getProgId() << endl;
            out << (*progIter)->getDuration() << endl;
            out << (*progIter)->getStatus() << endl;
            out << (*progIter)->getType() << endl;
            out << (*progIter)->getFilename() << endl;
            out << (*progIter)->getEpisode() << endl;
            out << (*progIter)->getAvailable() << endl;
            out << (*progIter)->getChannel() << endl;
            out << (*progIter)->getWeb() << endl;
            QString value = (*progIter)->getDescription();
            out << Settings::escape(value) << endl;
            out << (*progIter)->getImageId() << endl;
            out << (*progIter)->getPosition() << endl;
        }
        file.close();
    }
}

void QueueModel::importFromFile(QFile & file) {
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString title;
            QString progId;
            float length;
            Queue::STATUS status;
            QueueItem::TYPE type;
            QString filename;
            QString episode;
            qint64 available;
            QString channel;
            QString web;
            QString description;
            QString imageid;
            quint32 position;

            title = in.readLine();
            progId = in.readLine();
            length = in.readLine().toFloat();
            status = static_cast<Queue::STATUS>(in.readLine().toInt());
            if (status == Queue::STATUS_DOWNLOADING) {
                status = Queue::STATUS_REMOTE;
            }
            type = static_cast<QueueItem::TYPE>(in.readLine().toInt());
            filename = in.readLine();
            episode = in.readLine();
            available = in.readLine().toULongLong();
            channel = in.readLine();
            web = in.readLine();
            QString value = in.readLine();
            description = Settings::unescape(value);
            imageid = in.readLine();
            position = in.readLine().toULong();

            if (progId != nullptr) {
                addProgramme(new QueueItem(progId, title, length, status, type, filename, episode, available, channel, web, description, imageid, position));
            }
        }
        file.close();
    }
}

QueueItem * QueueModel::findNextRemote() {
    QList<QueueItem* >::const_iterator iter;
    QueueItem * programme;

    programme = nullptr;
    for (iter = programmes.constBegin(); (programme == nullptr) && (iter != programmes.constEnd()); ++iter) {
        if ((*iter)->getStatus() == Queue::STATUS_REMOTE) {
            programme = (*iter);
        }
    }

    return programme;
}

QueueItem * QueueModel::findFromId(QString progid) {
    QList<QueueItem* >::const_iterator iter;
    QueueItem * programme;

    programme = nullptr;
    for (iter = programmes.constBegin(); (programme == nullptr) && (iter != programmes.constEnd()); ++iter) {
        if (((*iter)->getProgId().compare(progid, Qt::CaseSensitive)) == 0) {
            programme = (*iter);
        }
    }

    return programme;
}

void QueueModel::refreshItem(int index) {
    if ((index >= 0) && (index < programmes.length())) {
        emit dataChanged(this->index(index, 0), this->index(index, 0));
    }
}

void QueueModel::refreshItem(QueueItem * programme) {
    int index = programmes.indexOf(programme);
    refreshItem(index);
}

void QueueModel::pruneQueue() {
    QList<QueueItem* >::iterator iter = programmes.begin();

    while (iter != programmes.end()) {
        if (((*iter)->getStatus() == Queue::STATUS_LOCAL) && ((*iter)->fileExists() == false)) {
            iter = programmes.erase(iter);
        }
        else {
            iter++;
        }
    }
}

QString QueueModel::removePath(const QString &path) {
    QList<QueueItem* >::iterator iter = programmes.begin();
    QString found = "";
    int index;

    index = 0;
    while (iter != programmes.end()) {
        if (((*iter)->getStatus() == Queue::STATUS_LOCAL) && (path.compare((*iter)->getFilename()) == 0) && ((*iter)->fileExists() == false)) {
            found = (*iter)->getProgId();
            beginRemoveRows(QModelIndex(), index, index);
            iter = programmes.erase(iter);
            endRemoveRows();
        }
        else {
            index++;
            iter++;
        }
    }

    return found;
}

void QueueModel::monitorPaths(QFileSystemWatcher &filewatcher) {
    foreach (QueueItem *programme, programmes) {
        QString filename = programme->getFilename();
        if (filename != "") {
            filewatcher.addPath(filename);
        }
    }
}

unsigned int QueueModel::getCompleted() {
    unsigned int completed = 0;
    QList<QueueItem* >::const_iterator iter;

    for (iter = programmes.constBegin(); (iter != programmes.constEnd()); ++iter) {
        if ((*iter)->getCompleted()) {
            completed++;
        }
    }

    return completed;

}
