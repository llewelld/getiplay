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
        EpisodeRole,
        DurationRole,
        ChannelRole,
        AvailableRole,
        WebRole,
        NameRole,
        DescRole,
        ImageIdRole
    };

    QHash<int, QByteArray> roleNames() const;

    ProgModel(QString filename = QString(), QObject *parent = 0);
    ~ProgModel();

    void addProgramme(const Programme &programme);
    void replaceAll(const ProgModel &model);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void clear();

signals:
    // General signals
    void programmesChanged();

private:
    void exportToFile(QFile & file);
    void importFromFile(QFile & file);

    QString filename;
    QHash<int, QByteArray> roles;
    QList<Programme> programmes;
};

#endif // PROGMODEL_H
