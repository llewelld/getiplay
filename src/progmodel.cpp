#include "progmodel.h"
#include <QDebug>

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
    Q_UNUSED(parent)
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

void ProgModel::exportToFile(QFile & file) {
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        for (QList<Programme>::iterator progIter = programmes.begin(); progIter != programmes.end(); progIter++) {
            out << progIter->getName() << endl;
            out << progIter->getProgId() << endl;
            out << progIter->getLength() << endl;
        }
        file.close();
    }
}

void ProgModel::importFromFile(QFile & file) {
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString title;
            unsigned int progId;
            float length;
            title = in.readLine();
            progId = in.readLine().toInt();
            length = in.readLine().toFloat();
            addProgramme(Programme(progId, title, length));
        }
        file.close();
    }
}


