#include "progmodel.h"

ProgModel::ProgModel(QObject *parent) : QAbstractListModel(parent) {
    roles[ProgIdRole] = "progId";
    roles[NameRole] = "name";
    roles[LengthRole] = "size";
}

QHash<int, QByteArray> ProgModel::roleNames() const {
    return roles;
}

void ProgModel::addProgramme(const Programme &programme)
{
    //programmes.append(programme);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    programmes << programme;
    endInsertRows();
}

int ProgModel::rowCount(const QModelIndex & parent) const {
    return programmes.count();
}

QVariant ProgModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > programmes.count())
        return QVariant();

    const Programme &programme = programmes[index.row()];
    if (role == ProgIdRole)
        return programme.getProgId();
    else if (role == NameRole)
        return programme.getName();
    else if (role == LengthRole)
        return programme.getLength();
    return QVariant();
}

void ProgModel::clear() {
    programmes.clear();
}
