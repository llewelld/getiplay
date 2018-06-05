#include <QDesktopServices>
#include <QDateTime>
#include <QDebug>

#include "settings.h"

#define DIR_GETIPLAYERCONFIG "/.get_iplayer"
#define LEAF_LOG "/log01.txt"
#define LEAF_LOG_CYCLE "/log02.txt"

Settings * Settings::instance = nullptr;

Settings::Settings(QObject *parent) : QObject(parent),
  settings(this)
{
    audioDir = settings.value("storage/audioDir", QString(QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/" APP_NAME)).toString();
    videoDir = settings.value("storage/videoDir", QString(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/" APP_NAME)).toString();
    proxyUrl = settings.value("download/proxyUrl", "").toString();
    refreshType = (PROGTYPE)settings.value("storage/refreshType", PROGTYPE_NATIONAL).toInt();
}

Settings::~Settings() {
    settings.setValue("storage/audioDir", audioDir);
    settings.setValue("storage/videoDir", videoDir);
    settings.setValue("download/proxyUrl", proxyUrl);
    settings.setValue("storage/refreshType", refreshType);
}

void Settings::instantiate(QObject *parent) {
    if (instance == nullptr) {
        instance = new Settings(parent);
    }
}

Settings & Settings::getInstance() {
    return *instance;
}

QObject * Settings::provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return instance;
}

QString Settings::getLogDir() {
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QString Settings::getConfigDir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
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

QString Settings::getLogFile(unsigned int cycle) {
    QString filename;
    if (cycle == 0) {
        filename = Settings::getLogDir() + LEAF_LOG;
    }
    else {
        filename = Settings::getLogDir() + LEAF_LOG_CYCLE;
    }

    return filename;
}


QString & Settings::escape(QString &string) {
    string.replace("\n", "|");

    return string;
}

QString & Settings::unescape(QString &string) {
    string.replace("|", "\n");

    return string;
}

QString Settings::epochToDate (quint64 epoch) {
    QDateTime date;

    date.setTime_t(epoch);
    return date.toString("yyyy-MM-dd");
}

quint64 Settings::dateToEpoch (QString date) {
    quint64 epoch = QDateTime::fromString(date, Qt::ISODate).toTime_t();

    return epoch;
}

// Configurable values

QString Settings::getAudioDir() {
    return audioDir;
}

QString Settings::getVideoDir() {
    return videoDir;
}

QString Settings::getProxyUrl() {
    return proxyUrl;
}

Settings::PROGTYPE Settings::getRefreshType() {
    return refreshType;
}

void Settings::setAudioDir(QString &value) {
    qDebug() << "Set audio Dir: " << value;
    audioDir = value;
    emit audioDirChanged(audioDir);
}

void Settings::setVideoDir(QString &value) {
    qDebug() << "Set video Dir: " << value;
    videoDir = value;
    emit videoDirChanged(videoDir);
}

void Settings::setProxyUrl(QString &value) {
    qDebug() << "Set proxy URL: " << value;
    proxyUrl = value;
    emit proxyUrlChanged(proxyUrl);
}

void Settings::setRefreshType(PROGTYPE value) {
    qDebug() << "Set refresh type: " << value;
    refreshType = value;
    emit refreshTypeChanged(refreshType);
}

