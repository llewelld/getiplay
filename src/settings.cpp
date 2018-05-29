#include <QDesktopServices>

#include "settings.h"

#define DIR_GETIPLAYERCONFIG "/.get_iplayer"

Settings::Settings(QObject *parent) : QObject(parent)
{

}

QString Settings::getLogDir() {
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QString Settings::getConfigDir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

QString Settings::getMusicDir() {
    return QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/" APP_NAME;
}

QString Settings::getVideoDir() {
    return QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/" APP_NAME;
}

QString Settings::getDownloadsDir() {
    return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" APP_NAME;
}

QString Settings::getProfileDir() {
    return getConfigDir() + DIR_GETIPLAYERCONFIG;
}

QString Settings::getTempDir() {
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QString & Settings::escape(QString &string) {
    string.replace("\n", "|");

    return string;
}

QString & Settings::unescape(QString &string) {
    string.replace("|", "\n");

    return string;
}
