#include "programme.h"

Programme::Programme(QObject *parent) {
    name = "";
    length = 0.0f;
    progId = 0u;
}

Programme::Programme(unsigned int progId, QString name, float length) :
    name(name), length(length), progId(progId)
{
}

QString Programme::getName () const {
    return name;
}

float Programme::getLength () const {
    return length;
}

unsigned int Programme::getProgId () const {
    return progId;
}

void Programme::setName (const QString &value) {
    name = value;
}


void Programme::setLength (float value) {
    length = value;
}

void Programme::setProgId (unsigned int value) {
    progId = value;
}


//////////////// ProgModel ////////////////

ProgModel::ProgModel(QObject *parent) : QAbstractListModel(parent) {
    roles[ProIdRole] = "progId";
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
    if (role == ProIdRole)
        return programme.getProgId();
    else if (role == NameRole)
        return programme.getName();
    else if (role == LengthRole)
        return programme.getLength();
    return QVariant();
}

