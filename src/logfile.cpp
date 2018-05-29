#include "logfile.h"
#include <QDebug>
#include <QDir>
#include "settings.h"

#define LEAF_LOG "/log.txt"

logfile::logfile()
{
    fileOpen = false;
    file.setFileName(Settings::getLogDir() + LEAF_LOG);
}

logfile::~logfile()
{
    if (fileOpen == true) {
        // Close thee file
        closeLog();
    }
}

void logfile::openLog() {
    QDir dir;
    dir.mkpath(Settings::getLogDir());

    if (file.open(QIODevice::Append)) {
        fileOpen = true;
    }
    else {
        qDebug() << "WARNING: Failed to open log file: " << file.errorString();
    }
}

void logfile::logLine(const QString &text) {
    qDebug() << text;
    if (fileOpen == true) {
        QTextStream out(&file);
        out << text << endl;
    }
}

void logfile::closeLog() {
    if (fileOpen == true) {
        file.close();
        fileOpen = false;
    }
}

void logfile::status() {
    logLine("----------------------------------");
    logLine(APP_NAME " Configuration Status");
    logLine("Log dir: " + Settings::getLogDir());
    logLine("Log file: " + Settings::getLogDir() + LEAF_LOG);
    logLine("Config dir: " + Settings::getConfigDir());
    logLine("Music dir: " + Settings::getMusicDir());
    logLine("Video dir: " + Settings::getVideoDir());
    logLine("Downloads dir: " + Settings::getDownloadsDir());
    logLine("Profile dir: " + Settings::getProfileDir());
    logLine("Bin: " DIR_BIN);
    logLine("Lib: " DIR_LIB);
    logLine("Perl lib: " DIR_PERLLOCAL);
    logLine("----------------------------------");
}
