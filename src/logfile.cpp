#include "logfile.h"
#include <QDebug>
#include <QDir>
#include <QTime>
#include <QTimeZone>
#include "settings.h"

logfile::logfile()
{
    fileOpen = false;
    file.setFileName(Settings::getLogFile(0));
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
        // Add a header to the log file
        if (file.size() == 0) {
            logLine("GetiPlay version: " VERSION);
            QDateTime time(QDateTime::currentDateTime());
            QDateTime UTC(time.toUTC());
            logLine("Log started: " + UTC.toString());
            logLine("");
        }
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
    logLine("Log file: " + Settings::getLogFile(0));
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

void logfile::cycle() {
    // If the file only contains the header information, don't cycle
    if (file.size() > 70) {
        closeLog();
        // Delete the secondary log
        QFile cycle(Settings::getLogFile(1));
        cycle.remove();
        // Move the main log to become the secondary log
        QFile move(Settings::getLogFile(0));
        move.rename(Settings::getLogFile(1));
        openLog();
        status();
    }
}
