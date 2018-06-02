#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QProcess>
#include <QList>
#include <QTimer>
#include "progmodel.h"
#include "logfile.h"

#define LOGAPPEND(TEXT) if (log != nullptr) {log->logAppend( TEXT );}

class Log : public QObject
{
    Q_OBJECT

    // General properties
    Q_PROPERTY(QString logText READ getLogText WRITE setLogText NOTIFY logTextChanged)

public:

private:
    QString logText;
    logfile logToFile;

    // Internal methods

public:
    // General methods
    explicit Log(QObject *parent = 0);
    ~Log();
    void initialise();
    QString getLogText() const;
    void exportToFile(QFile & file);
    void importFromFile(QFile & file);

    Q_INVOKABLE void logAppend(const QString &text);
    Q_INVOKABLE void clear();

signals:
    // General signals
    void logTextChanged(QString &logText);

public slots:
    // General methods
    void setLogText(const QString &value);
};

#endif // LOG_H
