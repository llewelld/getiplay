#include <QDesktopServices>
#include <QDateTime>
#include <QDebug>
#include <sailfishapp.h>
#include <mlite5/MGConfItem>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QCoreApplication>

#include "settings.h"
#include "refresh.h"

#define DIR_GETIPLAYERCONFIG "/.get_iplayer"
#define LEAF_LOG "/log01.txt"
#define LEAF_LOG_CYCLE "/log02.txt"

Settings * Settings::instance = nullptr;

bool Settings::migrate() {
    QString const basePath = getConfigDir() + "/";
    QString const oldBasePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + QCoreApplication::applicationName() + "/";

    QString const settingsPath = basePath + QCoreApplication::applicationName() + ".conf";
    QString const oldSettingsPath = oldBasePath + QCoreApplication::applicationName() + ".conf";
    bool const migrate = !QFile(settingsPath).exists() && QFile(oldSettingsPath).exists();

    if (migrate) {
        qDebug() << "Migrating settings";
        qDebug() << "Migrating from : " << basePath;
        qDebug() << "Migrating to: " << oldBasePath;

        QDir dir;
        dir.rename(oldSettingsPath, settingsPath);

        QString const oldSubPath = oldBasePath + QCoreApplication::applicationName() + "/";
        QStringList const leafNames = {"queue.txt", "radio.txt", "tv.txt"};
        for (QString const &leafName : leafNames) {
            qDebug() << "Moving from: " << (oldSubPath + leafName) << ", to: " << (basePath + leafName);
            dir.rename(oldSubPath + leafName, basePath + leafName);
        }
    }
    return migrate;
}

Settings::Settings(QObject *parent) : QObject(parent) {
    migrate();

    QString const settingsPath = getConfigDir() + "/" + QCoreApplication::applicationName() + ".conf";
    qDebug() << "Settings file: " << settingsPath;
    settings = new QSettings(settingsPath, QSettings::NativeFormat, this);

    audioDir = settings->value("storage/audioDir", QString(QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/" APP_NAME)).toString();
    videoDir = settings->value("storage/videoDir", QString(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/" APP_NAME)).toString();
    proxyUrl = settings->value("download/proxyUrl", "").toString();
    modeTv = (QUALITY)settings->value("download/modeTv", QUALITY_WORST).toInt();
    modeRadio = (QUALITY)settings->value("download/modeRadio", QUALITY_WORST).toInt();
    PROGTYPE refreshType = (PROGTYPE)settings->value("storage/refreshType", PROGTYPE_NATIONAL).toInt();
    refreshTypeTv = (PROGTYPE)settings->value("storage/refreshTypeTv", refreshType).toInt();
    refreshTypeRadio = (PROGTYPE)settings->value("storage/refreshTypeRadio", refreshType).toInt();

    settings->beginReadArray("storage/lastRefresh");
    for (int progType = 0; progType < REFRESHTYPE_NUM; progType++) {
        settings->setArrayIndex(progType);
        lastRefreshType[progType] = static_cast<PROGTYPE>(settings->value("type", PROGTYPE_INVALID).toInt());
    }
    settings->endArray();

    currentTab = settings->value("state/currentTab", 0).toInt();

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

    indexMaxConn = (unsigned int)settings->value("download/indexMaxConn", 0).toInt();
    if (indexMaxConn == 0) {
        // Select default value depending on the device
        bool early = earlyDevice();
        if (early) {
            indexMaxConn = 2;
        }
        else {
            indexMaxConn = 5;
        }
    }

    skipTimeShort = settings->value("player/skipTimeShort", 10).toInt();
    skipTimeLong = settings->value("player/skipTimeLong", 5 * 60).toInt();
}

Settings::~Settings() {
    settings->setValue("storage/audioDir", audioDir);
    settings->setValue("storage/videoDir", videoDir);
    settings->setValue("download/proxyUrl", proxyUrl);
    settings->setValue("download/modeTv", modeTv);
    settings->setValue("download/modeRadio", modeRadio);

    settings->setValue("storage/refreshTypeTv", refreshTypeTv);
    settings->setValue("storage/refreshTypeRadio", refreshTypeRadio);
    settings->setValue("state/currentTab", currentTab);

    settings->beginWriteArray("storage/lastRefresh");
    for (int progType = 0; progType < REFRESHTYPE_NUM; progType++) {
        settings->setArrayIndex(progType);
        settings->setValue("type", static_cast<int>(lastRefreshType[progType]));
    }
    settings->endArray();

    settings->setValue("download/indexMaxConn", indexMaxConn);
    settings->setValue("player/skipTimeShort", skipTimeShort);
    settings->setValue("player/skipTimeLong", skipTimeLong);

    qDebug() << "Settings deleted";
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

QString Settings::getLatestLogFile() const {
    return getLogFile(0);
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
    QString result;

    if (epoch != 0) {
        date.setTime_t(epoch);
        result = date.toString("yyyy-MM-dd");
    }
    else {
        result = "";
    }

    return result;
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

Settings::PROGTYPE Settings::getRefreshTypeTv() {
    return refreshTypeTv;
}

Settings::PROGTYPE Settings::getRefreshTypeRadio() {
    return refreshTypeRadio;
}

unsigned int Settings::getCurrentTab() {
    return currentTab;
}

unsigned int Settings::getIndexMaxConn() {
    return indexMaxConn;
}

unsigned int Settings::getSkipTimeShort() {
    return skipTimeShort;
}

unsigned int Settings::getSkipTimeLong() {
    return skipTimeLong;
}

Settings::QUALITY Settings::getModeTv() {
    return modeTv;
}

Settings::QUALITY Settings::getModeRadio() {
    return modeRadio;
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

void Settings::setRefreshTypeTv(PROGTYPE value) {
    qDebug() << "Set refresh type tv: " << value;
    refreshTypeTv = value;
    emit refreshTypeTvChanged(refreshTypeTv);
}

void Settings::setRefreshTypeRadio(PROGTYPE value) {
    qDebug() << "Set refresh type radio: " << value;
    refreshTypeRadio = value;
    emit refreshTypeRadioChanged(refreshTypeRadio);
}

void Settings::setCurrentTab(unsigned int value) {
    if (value != currentTab) {
        qDebug() << "Set current tab: " << value;
        currentTab = value;
        emit currentTabChanged(currentTab);
    }
}

void Settings::setIndexMaxConn(unsigned int value) {
    qDebug() << "Set index max connections: " << value;
    indexMaxConn = value;
    emit indexMaxConnChanged(indexMaxConn);
}

void Settings::setSkipTimeShort(unsigned int value) {
    qDebug() << "Set skip time short: " << value;
    skipTimeShort = value;
    emit skipTimeShortChanged(skipTimeShort);
}

void Settings::setSkipTimeLong(unsigned int value) {
    qDebug() << "Set skip time long: " << value;
    skipTimeLong = value;
    emit skipTimeLongChanged(skipTimeLong);
}

void Settings::setModeTv(QUALITY value) {
    qDebug() << "Set mode tv: " << value;
    modeTv = value;
    emit modeTvChanged(modeTv);
}

void Settings::setModeRadio(QUALITY value) {
    qDebug() << "Set mode radio: " << value;
    modeRadio = value;
    emit modeRadioChanged(modeRadio);
}

bool Settings::getRebuildCache(int type) {
    bool rebuildCache;
    int typeget = qBound(0, (int)type, (int)REFRESHTYPE_NUM);
    PROGTYPE refreshType;
    switch (typeget) {
    case REFRESHTYPE_RADIO:
        refreshType = refreshTypeRadio;
        break;
    default:
        refreshType = refreshTypeTv;
        break;
    }

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

QString Settings::qualityToStringRadio (QUALITY quality) {
    QString result;
    switch (quality) {
    case QUALITY_BEST:
        result = QStringLiteral("high,std,med,low");
        break;
    case QUALITY_GOOD:
        result = QStringLiteral("std,med,low");
        break;
    case QUALITY_WORST:
    default:
        result = QStringLiteral("low");
        break;
    }

    return result;
}

QString Settings::qualityToStringTv (QUALITY quality) {
    QString result;
    switch (quality) {
    case QUALITY_BEST:
        result = QStringLiteral("fhd,hd,sd,web,mobile");
        break;
    case QUALITY_GOOD:
        result = QStringLiteral("sd,web,mobile");
        break;
    case QUALITY_WORST:
    default:
        result = QStringLiteral("mobile");
        break;
    }

    return result;
}

QString Settings::getImageDir() const {
    return imageDir;
}
QString Settings::getImageUrl(const QString &id) const {
    return imageDir + id + ".png";
}

// See https://github.com/kimmoli/chargemon/blob/d577d5277e97524a298bea3ff3cec9588971e06b/src/cmon.cpp#L54
// Licence MIT
bool Settings::earlyDevice()
{
    bool result = false;
    bool success = true;
    QDBusMessage ssuCallReply;
    QList<QVariant> outArgs;

    if (success) {
        if (!QDBusConnection::systemBus().isConnected()) {
            qWarning() << "Cannot connect to the D-Bus systemBus:" << qPrintable(QDBusConnection::systemBus().lastError().message());
            success = false;
        }
    }

    if (success) {
        QDBusInterface ssuCall("org.nemo.ssu", "/org/nemo/ssu", "org.nemo.ssu", QDBusConnection::systemBus());
        ssuCall.setTimeout(1000);

        QList<QVariant> args;
        args.append(2);

        ssuCallReply = ssuCall.callWithArgumentList(QDBus::Block, "displayName", args);

        if (ssuCallReply.type() == QDBusMessage::ErrorMessage) {
            qWarning() <<  "DBus Error: " << qPrintable(ssuCallReply.errorMessage());
            success = false;
        }
    }

    if (success) {
        outArgs = ssuCallReply.arguments();
        if (outArgs.count() == 0) {
            qWarning() << "Device ssu dbus reply is empty";
            success = false;
        }
    }

    if (success) {
        QString const deviceName = outArgs.at(0).toString();
        qDebug() << "Device name: " << qPrintable(deviceName);

        const QStringList earlyDevices = {
            "JP-1301", // Jolla One
            "JT-1501", // Jolla Tablet
            "onyx", // One Plus X
            "fp2-sibon", // Sibon
            "JP-1601", // Jolla C
            "Aqua Fish", // Aqua Fish
            "L500D", // Aqua Fish
        };

        if (earlyDevices.contains(deviceName)) {
            result = true;
        }
    }

    return result;
}
