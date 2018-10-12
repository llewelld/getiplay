#include "log.h"
#include <QDebug>
#include "harbour-getiplay.h"
#include "settings.h"

Log::Log(QObject *parent) :
    QObject(parent),
    logText("")
{
    logToFile.openLog();
    qDebug() << "LOG FILE OPEN";
}

void Log::initialise()
{
}

Log::~Log() {
    logToFile.closeLog();
    qDebug() << "LOG FILE CLOSED";
}

QString Log::getLogText() const
{
    return logText;
}

void Log::setLogText(const QString &value)
{
    logText = value;
    emit logTextChanged(logText);
}

void Log::logAppend(const QString &text)
{
    qDebug() << "LOGAPPEND: " << text;
    if (!text.isEmpty()) {
        QString append = text;
        logToFile.logLine(append);
        // Ensure we end with a newline
        if (!append.endsWith('\n')) {
            append += '\n';
        }
        // How many lines to add
        int newLines = append.count('\n');
        int currentLines = logText.count('\n');
        int removeLines = currentLines + newLines - LOG_LINES;

        // Remove excess lines from the top
        while (removeLines > 0) {
            int nextLine = logText.indexOf('\n');
            if (nextLine >= 0) {
                logText = logText.mid(nextLine + 1);
            }
            removeLines--;
        }

        // Add new lines
        logText.append(append);
        emit logTextChanged(logText);
    }
}

void Log::logAppendTimestamp(const QString &text) {
    QDateTime time(QDateTime::currentDateTime());
    QDateTime UTC(time.toUTC());
    logAppend(UTC.toString() + ": " + text);
}

void Log::exportToFile(QFile & file) {
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << logText;
        file.close();
    }
}

void Log::importFromFile(QFile & file) {
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        logText = in.readAll();
        file.close();
    }
    qDebug() << "Log lines: " << logText.count('\n');
    trim();
    qDebug() << "Trimmed to: " << logText.count('\n');
}

void Log::clear() {
    logToFile.cycle();
    logText = "";
    emit logTextChanged(logText);
}

void Log::trim() {
    int newline;
    int lines;

    lines = 0;
    newline = logText.length() - 1;
    while ((lines <= LOG_LINES) && (newline >= 0)) {
        newline = logText.lastIndexOf('\n', newline) - 1;
        lines++;
    }

    if (newline >= 0) {
        logText = logText.mid(newline + 2);
    }
}

