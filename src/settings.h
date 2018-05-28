#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDir>

#include "harbour-getiplay.h"

#define DIR_BIN "/usr/share/" APP_NAME "/bin"
#define DIR_LIB "/usr/share/" APP_NAME "/lib"
#define DIR_PERLLOCAL "/usr/share/" APP_NAME "/lib/perl5"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    static QString getLogDir();
    static QString getConfigDir();
    static QString getMusicDir();
    static QString getVideoDir();
    static QString getDownloadsDir();
    static QString getProfileDir();
    static QString getTempDir();

    static QString & escape(QString &string);
    static QString & unescape(QString &string);
signals:

public slots:
};

#endif // SETTINGS_H
