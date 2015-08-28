#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QProcess>

enum DOWNLOADSTATUS {
    DOWNLOADSTATUS_INVALID = -1,

    DOWNLOADSTATUS_UNINITIALISED,
    DOWNLOADSTATUS_INITIALISING,
    DOWNLOADSTATUS_DOWNLOADING,
    DOWNLOADSTATUS_CANCEL,
    DOWNLOADSTATUS_DONE,

    DOWNLOADSTATUS_NUM
};


class Download : public QObject
{
    Q_OBJECT
private:
    QProcess * process;
    DOWNLOADSTATUS status;
    QStringList arguments;

    // Internal methods
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

signals:
    // General signals
    void statusChanged(int status);

public slots:
    // General methods
    void startDownload ();
    void cancel ();
    void readData ();
    void started ();
    void finished (int code);
    void readError (QProcess::ProcessError error);
};

#endif // DOWNLOAD_H
