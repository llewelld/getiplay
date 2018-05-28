#ifndef QUEUEMODEL_H
#define QUEUEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QFile>
#include "queue.h"

class QueueItem;

class QueueModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum QueueRoles {
        ProgIdRole = Qt::UserRole + 1,
        NameRole,
        DurationRole,
        StatusRole,
        ProgressRole,
        TypeRole,
        FilenameRole,
        ChannelRole,
        DescriptionRole,
        EpisodeRole,
        TimeaddedRole,
        WebRole
    };

    QHash<int, QByteArray> roleNames() const;

    QueueModel(QObject *parent = 0);

    void addProgramme(QueueItem * programme);
    void removeFirstWithProgId(QString progid);
    QueueItem * findNextRemote();
    QueueItem * findFromId(QString progid);
    void refreshItem(int index);
    void refreshItem(QueueItem * programme);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void clear();

    void exportToFile(QFile & file);
    void importFromFile(QFile & file);
    void pruneQueue();
    bool removePath(const QString &path);
    void monitorPaths(QFileSystemWatcher &filewatcher);

signals:
    // General signals
    void programmesChanged();

private:
    QHash<int, QByteArray> roles;
    QList<QueueItem *> programmes;
};

#endif // PROGMODEL_H
