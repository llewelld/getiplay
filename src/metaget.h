#ifndef METAGET_H
#define METAGET_H

#include <QObject>
#include <QProcess>
#include "log.h"


class Metaget : public QObject
{
    Q_OBJECT

    // General properties
    Q_ENUMS(STATUS)

public:
    enum STATUS {
        STATUS_INVALID = -1,

        STATUS_UNINITIALISED,
        STATUS_INITIALISING,
        STATUS_DOWNLOADING,
        STATUS_CANCEL,
        STATUS_DONE,
        STATUS_ERROR,

        STATUS_NUM
    };

private:
    QProcess * process;
    STATUS status;
    QStringList arguments;
    QString progId;
    int progType;
    Log * log;
    QString metadata;

    // Internal methods
    void setupEnvironment();
    void collectArguments();
    void setStatus(STATUS newStatus);
    void addArgument(QString key, QString value);
    void addArgument(QString key);
    void addArgumentNonempty(QString key, QString value);
    void addOption(QString key, bool add);
    void addValue(QString key);
    void interpretData(const QString &text);
    void interpretLine(const QString &text);

public:
    explicit Metaget(QObject *parent = 0, Log *log = 0);
    void initialise();
    STATUS getStatus();
    Q_INVOKABLE QString metagot();

signals:
    // General signals
    void statusChanged(int status);

public slots:
    // General methods
    void startDownload(QString progId, int progType);
    void cancel();

private slots:
    void readData();
    void started();
    void finished(int code);
    void readError(QProcess::ProcessError error);
};

#endif // METAGET_H
