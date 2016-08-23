#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QProcess>
#include "logfile.h"

enum DOWNLOADSTATUS {
    DOWNLOADSTATUS_INVALID = -1,

    DOWNLOADSTATUS_UNINITIALISED,
    DOWNLOADSTATUS_INITIALISING,
    DOWNLOADSTATUS_DOWNLOADING,
    DOWNLOADSTATUS_CONVERTING,
    DOWNLOADSTATUS_CANCEL,
    DOWNLOADSTATUS_DONE,

    DOWNLOADSTATUS_NUM
};


class Download : public QObject
{
    Q_OBJECT

    // General properties
    Q_PROPERTY (float progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY (QString logText READ getLogText WRITE setLogText NOTIFY logTextChanged)
    Q_ENUMS(DOWNLOADSTATUS)

private:
    QProcess * process;
    DOWNLOADSTATUS status;
    QStringList arguments;
    int progId;
    double duration;
    float progress;
    QString logText;
    logfile logToFile;

    // Internal methods
    void setupEnvironment ();
    void collectArguments ();
    void setStatus (DOWNLOADSTATUS newStatus);
    void addArgument (QString key, QString value);
    void addArgument (QString key);
    void addArgumentNonempty (QString key, QString value);
    void addOption (QString key, bool add);
    void addValue (QString key);
    void interpretData(const QString &text);
    void interpretLine(const QString &text);

public:
    explicit Download(QObject *parent = 0);
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
    void startDownload (int progId);
    void cancel ();
    void readData ();
    void started ();
    void finished (int code);
    void readError (QProcess::ProcessError error);
    void setProgress(float value);
    void setLogText(const QString &value);
    void logAppend(const QString &text);
};

#endif // DOWNLOAD_H
