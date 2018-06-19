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

    // Configurable values
    Q_INVOKABLE QString getAudioDir();
    Q_INVOKABLE QString getVideoDir();
    Q_INVOKABLE QString getProxyUrl();
    Q_INVOKABLE PROGTYPE getRefreshType();
    Q_INVOKABLE unsigned int getCurrentTab();

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

public slots:
    // Configurable values
    void setAudioDir(QString &value);
    void setVideoDir(QString &value);
    void setProxyUrl(QString &value);
    void setRefreshType(PROGTYPE value);
    void setCurrentTab(unsigned int value);

private:
    static Settings * instance;
    QSettings settings;

    // Configurable values
    QString audioDir;
    QString videoDir;
    QString proxyUrl;
    PROGTYPE refreshType;
    PROGTYPE lastRefreshType[REFRESHTYPE_NUM];
    unsigned int currentTab;
};

#endif // SETTINGS_H
