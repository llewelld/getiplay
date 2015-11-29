#include "logfile.h"
#include <QDebug>
#include <QDir>

logfile::logfile()
{
    fileOpen = false;
    file.setFileName(FILE_LOG);
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
    dir.mkdir(DIR_LOG);

    if (file.open(QIODevice::WriteOnly)) {
        fileOpen = true;
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
