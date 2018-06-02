#ifndef PROGRAMME_H
#define PROGRAMME_H

#include <QObject>
#include <QTime>

class Programme
{
public:
    Programme();
    Programme(QString progId, QString name, qint32 duration, qint64 available, QString channel, QString episode, QString web, QString description);

    QString getName () const;
    qint32 getDuration () const;
    QString getProgId () const;
    qint64 getAvailable () const;
    QString getChannel () const;
    QString getEpisode () const;
    QString getWeb () const;
    QString getDescription () const;

    void setName (const QString &value);
    void setDuration (qint32 value);
    void setProgId (unsigned int value);
    void setAvailable(qint64 value);
    void setChannel(QString value);
    void setEpisode(QString value);
    void setWeb(QString value);
    void setDescription(QString value);

private:
    QString name;
    qint32 duration;
    QString progId;
    QString episode;
    qint64 available;
    QString channel;
    QString web;
    QString description;
};


#endif // PROGRAMME_H
