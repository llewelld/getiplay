#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDir>
#include <QQmlEngine>
#include <QSettings>

#include "harbour-getiplay.h"

#define DIR_BIN "/usr/share/" APP_NAME "/bin"
#define DIR_LIB "/usr/share/" APP_NAME "/lib"
#define DIR_PERLLOCAL "/usr/share/" APP_NAME "/lib/perl5"

class Settings : public QObject
{
    Q_OBJECT
    Q_ENUMS(REFRESHTYPE PROGTYPE)

    // General properties
    Q_PROPERTY(QString audioDir READ getAudioDir WRITE setAudioDir NOTIFY audioDirChanged)
    Q_PROPERTY(QString videoDir READ getVideoDir WRITE setVideoDir NOTIFY videoDirChanged)
    Q_PROPERTY(QString proxyUrl READ getProxyUrl WRITE setProxyUrl NOTIFY proxyUrlChanged)
    Q_PROPERTY(PROGTYPE refreshType READ getRefreshType WRITE setRefreshType NOTIFY refreshTypeChanged)
    Q_PROPERTY(unsigned int currentTab READ getCurrentTab WRITE setCurrentTab NOTIFY currentTabChanged)
    Q_PROPERTY(unsigned int indexMaxConn READ getIndexMaxConn WRITE setIndexMaxConn NOTIFY indexMaxConnChanged)
    Q_PROPERTY(unsigned int skipTimeShort READ getSkipTimeShort WRITE setSkipTimeShort NOTIFY skipTimeShortChanged)
    Q_PROPERTY(unsigned int skipTimeLong READ getSkipTimeLong WRITE setSkipTimeLong NOTIFY skipTimeLongChanged)

public:
    enum PROGTYPE {
        PROGTYPE_INVALID = -1,

        PROGTYPE_NATIONAL,
        PROGTYPE_REGIONAL,
        PROGTYPE_LOCAL,
        PROGTYPE_ALL,

        PROGTYPE_NUM
    };

    enum REFRESHTYPE {
        REFRESHTYPE_INVALID = -1,

        REFRESHTYPE_RADIO = 0,
        REFRESHTYPE_TV = 1,

        REFRESHTYPE_NUM
    };

    explicit Settings(QObject *parent = nullptr);
    ~Settings();

    static void instantiate(QObject *parent = nullptr);
    static Settings & getInstance();
    static QObject * provider(QQmlEngine *engine, QJSEngine *scriptEngine);

    // Non-configurable values
    Q_INVOKABLE static QString getLogDir();
    Q_INVOKABLE static QString getConfigDir();
    Q_INVOKABLE static QString getDownloadsDir();
    Q_INVOKABLE static QString getProfileDir();
    Q_INVOKABLE static QString getTempDir();
    Q_INVOKABLE static QString getLogFile(unsigned int cycle);
    bool getRebuildCache(int type);
    Q_INVOKABLE QString getImageDir() const;
    Q_INVOKABLE QString getImageUrl(const QString &id) const;

    // Configurable values
    Q_INVOKABLE QString getAudioDir();
    Q_INVOKABLE QString getVideoDir();
    Q_INVOKABLE QString getProxyUrl();
    Q_INVOKABLE PROGTYPE getRefreshType();
    Q_INVOKABLE unsigned int getCurrentTab();
    Q_INVOKABLE unsigned int getIndexMaxConn();
    Q_INVOKABLE unsigned int getSkipTimeShort();
    Q_INVOKABLE unsigned int getSkipTimeLong();

    Q_INVOKABLE static QString epochToDate (quint64 epoch);
    Q_INVOKABLE static quint64 dateToEpoch (QString date);
    Q_INVOKABLE static QString millisecondsToTime (quint32 milliseconds);

    static QString & escape(QString &string);
    static QString & unescape(QString &string);

signals:
    // Configurable values
    void audioDirChanged(QString &audioDir);
    void videoDirChanged(QString &videoDir);
    void proxyUrlChanged(QString &proxyUrl);
    void refreshTypeChanged(PROGTYPE refreshType);
    void currentTabChanged(unsigned int currentTab);
    void indexMaxConnChanged(unsigned int indexMaxConn);
    void skipTimeShortChanged(unsigned int skipTimeShort);
    void skipTimeLongChanged(unsigned int skipTimeLong);

public slots:
    // Configurable values
    void setAudioDir(QString &value);
    void setVideoDir(QString &value);
    void setProxyUrl(QString &value);
    void setRefreshType(PROGTYPE value);
    void setCurrentTab(unsigned int value);
    void setIndexMaxConn(unsigned int value);
    void setSkipTimeShort(unsigned int value);
    void setSkipTimeLong(unsigned int value);

private:
    typedef enum _DEVICE {
        DEVICE_INVALID = -1,
        DEVICE_FAILURE,
        DEVICE_UNKNOWN,

        DEVICE_JOLLA_ONE,
        DEVICE_JOLLA_TABLET,
        DEVICE_ONE_PLUS_X,
        DEVICE_SIBON,
        DEVICE_JOLLA_C,
        DEVICE_AQUAFISH,
        DEVICE_XPERIA_X,
        DEVICE_XPERIA_X_DUALSIM,
        DEVICE_XPERIA_X_COMPACT,

        DEVICe_NUM
    } DEVICE;

    static DEVICE getDevice();

    static Settings * instance;
    QSettings settings;
    double pixelRatio;
    QString imageDir;

    // Configurable values
    QString audioDir;
    QString videoDir;
    QString proxyUrl;
    PROGTYPE refreshType;
    PROGTYPE lastRefreshType[REFRESHTYPE_NUM];
    unsigned int currentTab;
    unsigned int indexMaxConn;
    unsigned int skipTimeShort;
    unsigned int skipTimeLong;
};

#endif // SETTINGS_H
