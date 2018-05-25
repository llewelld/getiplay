#include "queuemodel.h"
#include <QDebug>

QueueModel::QueueModel(QObject *parent) : QAbstractListModel(parent) {
    roles[ProgIdRole] = "progId";
    roles[NameRole] = "name";
    roles[DurationRole] = "duration";
    roles[StatusRole] = "status";
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
            QueueItem::STATUS status;
            QueueItem::TYPE type;
            title = in.readLine();
            progId = in.readLine();
            length = in.readLine().toFloat();
            status = static_cast<QueueItem::STATUS>(in.readLine().toInt());
            if (status == QueueItem::STATUS_DOWNLOADING) {
                status = QueueItem::STATUS_REMOTE;
            }
            type = static_cast<QueueItem::TYPE>(in.readLine().toInt());
            addProgramme(new QueueItem(progId, title, length, status, type));
        }
        file.close();
    }
}

QueueItem * QueueModel::findNextRemote() {
    QList<QueueItem* >::const_iterator iter;
    QueueItem * programme;

    programme = nullptr;
    for (iter = programmes.constBegin(); (programme == nullptr) && (iter != programmes.constEnd()); ++iter) {
        if ((*iter)->getStatus() == QueueItem::STATUS_REMOTE) {
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
