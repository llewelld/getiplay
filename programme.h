#ifndef PROGRAMME_H
#define PROGRAMME_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

class Programme
{
public:
    Programme(QObject *parent = 0);
    Programme(unsigned int progId, QString name, float length);

    QString getName () const;
    float getLength () const;
    unsigned int getProgId () const;

    void setName (const QString &value);
    void setLength (float value);
    void setProgId (unsigned int value);

private:
    QString name;
    float length;
    unsigned int progId;
};


//////////////// ProgModel ////////////////

class ProgModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ProgRoles {
        ProIdRole = Qt::UserRole + 1,
        NameRole,
        LengthRole
    };

    QHash<int, QByteArray> roleNames() const;

    ProgModel(QObject *parent = 0);

    void addProgramme(const Programme &programme);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

private:
    QHash<int, QByteArray> roles;
    QList<Programme> programmes;
};

#endif // PROGRAMME_H
