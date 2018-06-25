#include <QDesktopServices>
#include <QDateTime>
#include <QDebug>
#include <sailfishapp.h>
#include <mlite5/MGConfItem>
#include <QDBusConnection>
#include <QDBusInterface>

#include "settings.h"
#include "refresh.h"

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

    settings.beginReadArray("storage/lastRefresh");
    for (int progType = 0; progType < REFRESHTYPE_NUM; progType++) {
        settings.setArrayIndex(progType);
        lastRefreshType[progType] = static_cast<PROGTYPE>(settings.value("type", PROGTYPE_INVALID).toInt());
    }
    settings.endArray();

    currentTab = settings.value("state/currentTab", 0).toInt();

    QScopedPointer<MGConfItem> ratioItem(new MGConfItem("/desktop/sailfish/silica/theme_pixel_ratio"));
    pixelRatio = ratioItem->value(1.0).toDouble();
    QString dir;
    if (pixelRatio > 1.75) {
        dir = "2.0";
    }
    else if (pixelRatio > 1.5) {
        dir = "1.75";
    }
    else if (pixelRatio > 1.25) {
        dir = "1.5";
    }
    else if (pixelRatio > 1.0) {
        dir = "1.25";
    }
    else {
        dir = "1.0";
    }

    imageDir = SailfishApp::pathTo("qml/images/z" + dir).toString(QUrl::RemoveScheme) + "/";
    qDebug() << "Image folder: " << imageDir;

    indexMaxConn = (unsigned int)settings.value("download/indexMaxConn", 0).toInt();
    if (indexMaxConn == 0) {
        // Select default value depending on the device
        DEVICE device = getDevice();
        switch (device) {
        case DEVICE_XPERIA_X:
        case DEVICE_XPERIA_X_DUALSIM:
        case DEVICE_XPERIA_X_COMPACT:
            indexMaxConn = 5;
            break;
        default:
            indexMaxConn = 2;
            break;
        }
    }
}

Settings::~Settings() {
    settings.setValue("storage/audioDir", audioDir);
    settings.setValue("storage/videoDir", videoDir);
    settings.setValue("download/proxyUrl", proxyUrl);
    settings.setValue("storage/refreshType", refreshType);
    settings.setValue("state/currentTab", currentTab);

    settings.beginWriteArray("storage/lastRefresh");
    for (int progType = 0; progType < REFRESHTYPE_NUM; progType++) {
        settings.setArrayIndex(progType);
        settings.setValue("type", static_cast<int>(lastRefreshType[progType]));
    }
    settings.endArray();

    settings.setValue("download/indexMaxConn", indexMaxConn);
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

unsigned int Settings::getCurrentTab() {
    return currentTab;
}

unsigned int Settings::getIndexMaxConn() {
    return indexMaxConn;
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

void Settings::setCurrentTab(unsigned int value) {
    qDebug() << "Set current tab: " << value;
    currentTab = value;
    emit currentTabChanged(currentTab);
}

void Settings::setIndexMaxConn(unsigned int value) {
    qDebug() << "Set index max connections: " << value;
    indexMaxConn = value;
    emit indexMaxConnChanged(indexMaxConn);
}


bool Settings::getRebuildCache(int type) {
    bool rebuildCache;
    int typeget = qBound(0, (int)type, (int)REFRESHTYPE_NUM);

    // If the refresh type changed since the last time we refreshed
    // we need to rebuild the cache
    if (refreshType != lastRefreshType[typeget]) {
        rebuildCache = true;
    }
    else {
        rebuildCache = false;
    }
    lastRefreshType[typeget] = refreshType;

    return rebuildCache;
}

QString Settings::millisecondsToTime (quint32 milliseconds) {
    int remaining = milliseconds / 1000;
    int hours = remaining / 3600;
    int minutes = (remaining / 60) % 60;
    int seconds = (remaining % 60);

    return QString("%1:%2:%3").arg(hours).arg(minutes, 2, 'f', 0, '0').arg(seconds, 2, 'f', 0, '0');
}

QString Settings::getImageDir() const {
    return imageDir;
}
QString Settings::getImageUrl(const QString &id) const {
    return imageDir + id + ".png";
}

// See https://github.com/kimmoli/chargemon/blob/d577d5277e97524a298bea3ff3cec9588971e06b/src/cmon.cpp#L54
// Licence MIT
Settings::DEVICE Settings::getDevice()
{
    Settings::DEVICE result = DEVICE_INVALID;
    QDBusMessage ssuCallReply;
    QList<QVariant> outArgs;

    if (result != DEVICE_FAILURE) {
        if (!QDBusConnection::systemBus().isConnected()) {
            printf("Cannot connect to the D-Bus systemBus\n%s\n", qPrintable(QDBusConnection::systemBus().lastError().message()));
            result = DEVICE_FAILURE;
        }
    }

    if (result != DEVICE_FAILURE) {
        QDBusInterface ssuCall("org.nemo.ssu", "/org/nemo/ssu", "org.nemo.ssu", QDBusConnection::systemBus());
        ssuCall.setTimeout(1000);

        QList<QVariant> args;
        args.append(2);

        ssuCallReply = ssuCall.callWithArgumentList(QDBus::Block, "displayName", args);

        if (ssuCallReply.type() == QDBusMessage::ErrorMessage) {
            printf("Error: %s\n", qPrintable(ssuCallReply.errorMessage()));
            result = DEVICE_FAILURE;
        }
    }

    if (result != DEVICE_FAILURE) {
        outArgs = ssuCallReply.arguments();
        if (outArgs.count() == 0) {
            printf("Reply is empty\n");
            result = DEVICE_FAILURE;
        }
    }

    if (result != DEVICE_FAILURE) {
        QString deviceName = outArgs.at(0).toString();

        printf("device name is %s\n", qPrintable(deviceName));

        result = DEVICE_UNKNOWN;
        if (outArgs.at(0).toString() == "JP-1301") {
            // The one and only original Jolla phone
            result = DEVICE_JOLLA_ONE;
        }
        else if (outArgs.at(0).toString() == "JT-1501") {
            // The one and only original Jolla tablet
            result = DEVICE_JOLLA_TABLET;
        }
        else if (outArgs.at(0).toString() == "onyx") {
            // OneplusX
            result = DEVICE_ONE_PLUS_X;
        }
        else if (outArgs.at(0).toString() == "fp2-sibon") {
            result = DEVICE_SIBON;
        }
        else if (outArgs.at(0).toString() == "JP-1601") {
             // Jolla C
             result = DEVICE_JOLLA_C;
        }
        else if (outArgs.at(0).toString() == "Aqua Fish") {
            // Aquafish
            result = DEVICE_AQUAFISH;
        }
        else if (outArgs.at(0).toString() == "L500D") {
            // This is also Aquafish
            result = DEVICE_AQUAFISH;
        }
        else if (outArgs.at(0).toString() == "f5121") {
            // Sony Xperia X
            result = DEVICE_XPERIA_X;
        }
        else if (outArgs.at(0).toString() == "f5122") {
            // Sony Xperia X dual SIM
            result = DEVICE_XPERIA_X_DUALSIM;
        }
        else if (outArgs.at(0).toString() == "f5321") {
            // Sony Xperia X Compact
            result = DEVICE_XPERIA_X_COMPACT;
        }
    }

    return result;
}
