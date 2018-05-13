#ifndef REFRESH_H
#define REFRESH_H

#include <QObject>
#include <QProcess>
#include <QList>
#include "progmodel.h"
#include "logfile.h"

class Refresh : public QObject
{
    Q_OBJECT

    // General properties
    Q_PROPERTY (float progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY (QString logText READ getLogText WRITE setLogText NOTIFY logTextChanged)
    Q_ENUMS(REFRESHSTATUS REFRESHTYPE)

public:
    enum REFRESHSTATUS {
        REFRESHSTATUS_INVALID = -1,

        REFRESHSTATUS_UNINITIALISED,
        REFRESHSTATUS_INITIALISING,
        REFRESHSTATUS_REFRESHING,
        REFRESHSTATUS_CANCEL,
        REFRESHSTATUS_DONE,

        REFRESHSTATUS_NUM
    };

    enum REFRESHTYPE {
        REFRESHTYPE_INVALID = -1,

        REFRESHTYPE_RADIO = 0,
        REFRESHTYPE_TV = 1,

        REFRESHTYPE_NUM
    };

private:
    QProcess * process;
    REFRESHSTATUS status;
    QStringList arguments;
    QString logText;
    logfile logToFile;

    QList<ProgModel*> model;
    bool periodCheck;
    int periodCount;
    float progress;

    REFRESHTYPE currentRefresh;

    // Internal methods
    void setupEnvironment();
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
    explicit Refresh(QList<ProgModel*> model, QObject *parent = 0);
    void initialise();
    float getProgress() const;
    QString getLogText() const;

signals:
    // General signals
    void statusChanged(int status);
    void progressChanged(float progress);
    void logTextChanged (QString &logText);

public slots:
    // General methods
    void startRefresh (REFRESHTYPE type);
    void cancel ();
    void readData ();
    void started ();
    void finished (int code);
    void readError (QProcess::ProcessError error);
    void setProgress(float value);
    void setLogText(const QString &value);
    void logAppend(const QString &text);
};

#endif // REFRESH_H
