#ifndef REFRESH_H
#define REFRESH_H

#include <QObject>
#include <QProcess>
#include <QList>
#include <QTimer>
#include "progmodel.h"
#include "log.h"

class Refresh : public QObject
{
    Q_OBJECT

    // General properties
    Q_PROPERTY(float progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_ENUMS(REFRESHSTATUS REFRESHTYPE)

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
    REFRESHTYPE currentRefresh;
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

public:
    // General methods
    explicit Refresh(QObject *parent = 0, QList<ProgModel*> model = QList<ProgModel *>(), Log *log = 0);
    void initialise();
    float getProgress() const;
    QString getLogText() const;

signals:
    // General signals
    void statusChanged(int status);
    void progressChanged(float progress);

public slots:
    // General methods
    void startRefresh(REFRESHTYPE type);
    void cancel();

private slots:
    void readData();
    void started();
    void finished(int code);
    void readError(QProcess::ProcessError error);
    void setProgress(float value);
    void overflow();
};

#endif // REFRESH_H
