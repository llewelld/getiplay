#include "progmodel.h"
#include <QDebug>

ProgModel::ProgModel(QObject *parent) : QAbstractListModel(parent) {
    roles[ProgIdRole] = "progId";
    roles[EpisodeRole] = "episode";
    roles[DurationRole] = "duration";
    roles[ChannelRole] = "channel";
    roles[TimeaddedRole] = "timeadded";
    roles[WebRole] = "web";
    roles[NameRole] = "name";
    roles[DescRole] = "description";
}

QHash<int, QByteArray> ProgModel::roleNames() const {
    return roles;
}

void ProgModel::replaceAll(const ProgModel &model) {
    beginResetModel();
    programmes.clear();
    programmes.append(model.programmes);
    endResetModel();
}

void ProgModel::addProgramme(const Programme &programme)
{
    //programmes.append(programme);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    programmes << programme;
    endInsertRows();
}

int ProgModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent)
    return programmes.count();
}

QVariant ProgModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > programmes.count())
        return QVariant();

    const Programme &programme = programmes[index.row()];
    if (role == ProgIdRole)
        return programme.getProgId();
    else if (role == EpisodeRole)
        return programme.getEpisode();
    else if (role == DurationRole)
        return programme.getDuration();
    else if (role == ChannelRole)
        return programme.getChannel();
    else if (role == TimeaddedRole)
        return programme.getTimeAdded();
    else if (role == WebRole)
        return programme.getWeb();
    else if (role == NameRole)
        return programme.getName();
    else if (role == DescRole)
        return programme.getDescription();
    return QVariant();
}

void ProgModel::clear() {
    programmes.clear();
}

void ProgModel::exportToFile(QFile & file) {
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        for (QList<Programme>::iterator progIter = programmes.begin(); progIter != programmes.end(); progIter++) {
            out << progIter->getProgId() << endl;
            out << progIter->getEpisode() << endl;
            out << progIter->getDuration() << endl;
            out << progIter->getChannel() << endl;
            out << progIter->getTimeAdded() << endl;
            out << progIter->getWeb() << endl;
            out << progIter->getName() << endl;
            out << progIter->getDescription() << endl;
        }
        file.close();
    }
}

void ProgModel::importFromFile(QFile & file) {
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString pid = "";
            QString episode = "";
            qint32 duration = 0;
            QString channel = "";
            qint64 timeadded = 0;
            QString web = "";
            QString name = "";
            QString desc = "";

            pid = in.readLine();
            episode = in.readLine();
            duration = in.readLine().toLongLong();
            channel = in.readLine();
            timeadded = in.readLine().toLongLong();
            web = in.readLine();
            name = in.readLine();
            desc = in.readLine();

            addProgramme(Programme(pid, name, duration, timeadded, channel, episode, web, desc));
        }
        file.close();
    }
}


