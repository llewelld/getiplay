#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QProcess>
#include "log.h"

enum DOWNLOADSTATUS {
    DOWNLOADSTATUS_INVALID = -1,

    DOWNLOADSTATUS_UNINITIALISED,
    DOWNLOADSTATUS_INITIALISING,
    DOWNLOADSTATUS_DOWNLOADING,
    DOWNLOADSTATUS_CONVERTING,
    DOWNLOADSTATUS_CANCEL,
    DOWNLOADSTATUS_DONE,
    DOWNLOADSTATUS_ERROR,

    DOWNLOADSTATUS_NUM
};


class Download : public QObject
{
    Q_OBJECT

    // General properties
    Q_PROPERTY(float progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_ENUMS(DOWNLOADSTATUS)

private:
    QProcess * process;
    DOWNLOADSTATUS status;
    QStringList arguments;
    QString progId;
    QString progType;
    quint32 duration;
    float progress;
    Log * log;

    // Internal methods
    void setupEnvironment();
    void collectArguments();
    void setStatus(DOWNLOADSTATUS newStatus);
    void addArgument(QString key, QString value);
    void addArgument(QString key);
    void addArgumentNonempty(QString key, QString value);
    void addOption(QString key, bool add);
    void addValue(QString key);
    void interpretData(const QString &text);
    void interpretLine(const QString &text);

public:
    explicit Download(QObject *parent = 0, Log *log = 0);
    void initialise();
    float getProgress() const;
    DOWNLOADSTATUS getStatus();

signals:
    // General signals
    void statusChanged(int status);
    void progressChanged(float progress);

public slots:
    // General methods
    void startDownload(QString progId, QString progType);
    void cancel();

private slots:
    void readData();
    void started();
    void finished(int code);
    void readError(QProcess::ProcessError error);
    void setProgress(float value);
};

#endif // DOWNLOAD_H
