#ifndef REFRESH_H
#define REFRESH_H

#include <QObject>
#include <QProcess>
#include "progmodel.h"

enum REFRESHSTATUS {
    REFRESHSTATUS_INVALID = -1,

    REFRESHSTATUS_UNINITIALISED,
    REFRESHSTATUS_INITIALISING,
    REFRESHSTATUS_REFRESHING,
    REFRESHSTATUS_CANCEL,
    REFRESHSTATUS_DONE,

    REFRESHSTATUS_NUM
};


class Refresh : public QObject
{
    Q_OBJECT

    // General properties
    Q_PROPERTY (float progress READ getProgress WRITE setProgress NOTIFY progressChanged)

private:
    QProcess * process;
    REFRESHSTATUS status;
    QStringList arguments;

    ProgModel &model;
    bool periodCheck;
    int periodCount;
    float progress;

    // Internal methods
    void collectArguments ();
    void setStatus (REFRESHSTATUS newStatus);
    void addArgument (QString key, QString value);
    void addArgument (QString key);
    void addArgumentNonempty (QString key, QString value);
    void addOption (QString key, bool add);
    void addValue (QString key);
    void interpretData(const QString &text);
    void interpretLine(const QString &text);

public:
    // General methods
    explicit Refresh(ProgModel &model, QObject *parent = 0);
    void initialise();
    float getProgress() const;

signals:
    // General signals
    void statusChanged(int status);
    void progressChanged(float progress);

public slots:
    // General methods
    void startRefresh ();
    void cancel ();
    void readData ();
    void started ();
    void finished (int code);
    void readError (QProcess::ProcessError error);
    void setProgress(float value);
};

#endif // REFRESH_H
