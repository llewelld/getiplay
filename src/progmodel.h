#ifndef PROGMODEL_H
#define PROGMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QFile>

#include "programme.h"

class ProgModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ProgRoles {
        ProgIdRole = Qt::UserRole + 1,
        NameRole,
        LengthRole
    };

    QHash<int, QByteArray> roleNames() const;

    ProgModel(QObject *parent = 0);

    void addProgramme(const Programme &programme);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void clear();

    void exportToFile(QFile & file);
    void importFromFile(QFile & file);

signals:
    // General signals
    void programmesChanged();

private:
    QHash<int, QByteArray> roles;
    QList<Programme> programmes;
};

#endif // PROGMODEL_H
