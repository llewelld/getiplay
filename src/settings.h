#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDir>
#include <QQmlEngine>

#include "harbour-getiplay.h"

#define DIR_BIN "/usr/share/" APP_NAME "/bin"
#define DIR_LIB "/usr/share/" APP_NAME "/lib"
#define DIR_PERLLOCAL "/usr/share/" APP_NAME "/lib/perl5"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    static void instantiate(QObject *parent = nullptr);
    static Settings & getInstance();
    static QObject * provider(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE static QString getLogDir();
    Q_INVOKABLE static QString getConfigDir();
    Q_INVOKABLE static QString getMusicDir();
    Q_INVOKABLE static QString getVideoDir();
    Q_INVOKABLE static QString getDownloadsDir();
    Q_INVOKABLE static QString getProfileDir();
    Q_INVOKABLE static QString getTempDir();
    Q_INVOKABLE static QString getLogFile(unsigned int cycle);

    static QString & escape(QString &string);
    static QString & unescape(QString &string);
signals:

public slots:

private:
    static Settings * instance;
};

#endif // SETTINGS_H
