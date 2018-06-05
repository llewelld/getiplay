#ifndef REFRESH_H
#define REFRESH_H

#include <QObject>
#include <QProcess>
#include <QList>
#include <QTimer>
#include "progmodel.h"
#include "settings.h"
#include "log.h"

class Refresh : public QObject
{
    Q_OBJECT

    // General properties
    Q_PROPERTY(float progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_ENUMS(REFRESHSTATUS)

public:
    enum REFRESHSTATUS {
        REFRESHSTATUS_INVALID = -1,

        REFRESHSTATUS_UNINITIALISED,
        REFRESHSTATUS_INITIALISING,
        REFRESHSTATUS_DOWNLOADING,
        REFRESHSTATUS_PROCESSING,
        REFRESHSTATUS_OVERFLOW,
        REFRESHSTATUS_CANCEL,
        REFRESHSTATUS_DONE,

        REFRESHSTATUS_NUM
    };

private:
    QProcess * process;
    REFRESHSTATUS status;
    QStringList arguments;
    QList<ProgModel*> model;
    ProgModel temp;
    bool periodCheck;
    int periodCount;
    int addingCount;
    int addingTotal;
    int lineProcessCount;
    float progress;
    QTimer * overflowpoll;
    int finishedcode;
    Settings::REFRESHTYPE currentRefresh;
    Log * log;

    // Internal methods
    void setupEnvironment();
    void collectArguments ();
    void setStatus(REFRESHSTATUS newStatus);
    void addArgument(QString key, QString value);
    void addArgument(QString key);
    void addArgumentNonempty(QString key, QString value);
    void addOption(QString key, bool add);
    void addValue(QString key);
    void interpretData(const QString &text);
    void interpretLine(const QString &text);
    bool interpretProgramme(const QString &text);
    void setProgressCount(int periodCount, int addingCount);
    static const QString & includeTypeToString(Settings::PROGTYPE refreshType);
    static const QString & excludeTypeToString(Settings::PROGTYPE refreshType);

public:
    // General methods
    explicit Refresh(QObject *parent = 0, QList<ProgModel*> model = QList<ProgModel *>(), Log *log = 0);
    void initialise();
    float getProgress() const;
    QString getLogText() const;

    Q_INVOKABLE void startRefresh(int type);
    Q_INVOKABLE void cancel();

signals:
    // General signals
    void statusChanged(int status);
    void progressChanged(float progress);

private slots:
    void readData();
    void started();
    void finished(int code);
    void readError(QProcess::ProcessError error);
    void setProgress(float value);
    void overflow();
};

#endif // REFRESH_H
