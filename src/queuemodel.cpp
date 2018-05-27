#include "queuemodel.h"
#include "queueitem.h"
#include <QDebug>

QueueModel::QueueModel(QObject *parent) : QAbstractListModel(parent) {
    roles[ProgIdRole] = "progId";
    roles[NameRole] = "name";
    roles[DurationRole] = "duration";
    roles[StatusRole] = "qstatus";
    roles[ProgressRole] = "progress";
    roles[TypeRole] = "type";
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
            title = in.readLine();
            progId = in.readLine();
            length = in.readLine().toFloat();
            status = static_cast<Queue::STATUS>(in.readLine().toInt());
            if (status == Queue::STATUS_DOWNLOADING) {
                status = Queue::STATUS_REMOTE;
            }
            type = static_cast<QueueItem::TYPE>(in.readLine().toInt());
            if (progId != nullptr) {
                addProgramme(new QueueItem(progId, title, length, status, type));
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
