#include "download.h"
#include "stdio.h"
#include <QDebug>

//#define FAKE_GETIPLAYER (1)
#define MAX_LINE_LENGTH (255)

Download::Download(QObject *parent) :
    QObject(parent),
    process(NULL),
    status(DOWNLOADSTATUS_INVALID),
    progId(0),
    duration(0.0),
    progress(0.0)
{
    arguments.clear();
}

void Download::initialise()
{
    setStatus(DOWNLOADSTATUS_UNINITIALISED);
    setProgress(-1.0f);
}

void Download::setStatus(DOWNLOADSTATUS newStatus)
{
    if (status != newStatus) {
        status = newStatus;
        emit statusChanged(newStatus);
    }
}

void Download::startDownload(int progId) {
    this->progId = progId;

    if (process != NULL) {
        qDebug() << "Process already running.";
    }
    else {
        process = new QProcess();
#ifndef FAKE_GETIPLAYER
        QString program = "/home/nemo/Documents/Development/Projects/get_iplayer/get_iplayer";
#else // !FAKE_GETIPLAYER
        QString program = "cat";
#endif // !FAKE_GETIPLAYER
        collectArguments ();
        process->setProcessChannelMode(QProcess::MergedChannels);
        process->setReadChannel(QProcess::StandardOutput);
        connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(readError(QProcess::ProcessError)));
        connect(process, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(process, SIGNAL(started()), this, SLOT(started()));
        connect(process, SIGNAL(finished(int)), this, SLOT(finished(int)));

        process->start(program, arguments);
        process->closeWriteChannel();
        setStatus(DOWNLOADSTATUS_INITIALISING);
        setProgress(-1.0);
        duration = 0.0;
        arguments.clear();
    }
}

void Download::collectArguments () {
    arguments.clear();

#ifndef FAKE_GETIPLAYER
    addArgument("type=radio");
    addArgument("get", QString("%1").arg(progId));
    addArgument("force");
    addArgument("output", "/home/nemo/Music/iplayer");
#else // !FAKE_GETIPLAYER
    addValue("/opt/sdk/GetiPlay/usr/share/GetiPlay/output02.txt");
#endif // !FAKE_GETIPLAYER
}

void Download::addArgument (QString key, QString value) {
    QString argument;

    argument = "--" + key;
    arguments.append(argument);
    if (!value.isEmpty()) {
        arguments.append(value);
    }
}

void Download::addArgumentNonempty (QString key, QString value) {
    QString argument;

    if (!value.isEmpty()) {
        argument = "--" + key;
        arguments.append(argument);
        arguments.append(value);
    }
}

void Download::addArgument (QString key) {
    QString argument;

    argument = "--" + key;
    arguments.append(argument);
}

void Download::addOption (QString key, bool add) {
    if (add) {
        addArgument (key);
    }
}

void Download::addValue (QString key) {
    arguments.append(key);
}

void Download::cancel() {
    if (process != NULL) {

        process->terminate();
        setStatus(DOWNLOADSTATUS_CANCEL);
    }
}

void Download::readData() {
    // Read any full lines if they're available
    while (process->canReadLine()) {
        QByteArray read = process->readLine();
        //printf ("Output: %s", read.data());

        interpretData(read);
    }

    // Read any lines that full lines but don't register
    qint64 available = process->bytesAvailable();
    while (available > 0) {
        QByteArray read = process->peek(MAX_LINE_LENGTH);
        available = read.indexOf('\n');
        if (available < 0) {
            available = read.indexOf('\r');
        }
        if (available >= 0) {
            process->read(available + 1);
            interpretData(read.left(available));
        }
    }
}

void Download::interpretData(const QString &text) {
    if (!text.isEmpty()) {
        QStringList lines = text.split('\n', QString::SkipEmptyParts);
        for (QStringList::iterator iter = lines.begin(); iter != lines.end(); ++iter) {
            interpretLine(*iter);
        }
    }
}

void Download::interpretLine(const QString &text) {
    int foundPos;
    float percentRead;

    qDebug() << "Line: " << text;
    if (text.startsWith("INFO: Recorded ")) {
        setStatus(DOWNLOADSTATUS_DONE);
    }
    else {
        QRegExp percent("^.*\\((\\d+\\.\\d+)%\\)$");
        foundPos = percent.indexIn(text);
        if (foundPos > -1) {
            setStatus(DOWNLOADSTATUS_DOWNLOADING);
            percentRead = percent.cap(1).toFloat() / 100.0f;
            setProgress(percentRead);
            qDebug() << "Progress " << (getProgress() * 100.0) << "%";
        }
        else {
            QRegExp findDuration("^.*time=(\\d+):(\\d\\d):(\\d\\d\\.\\d+) .*$");
            foundPos = findDuration.indexIn(text);
            if (foundPos > -1) {
                int hours = findDuration.cap(1).toInt();
                int mins = findDuration.cap(2).toInt();
                float secs = findDuration.cap(3).toFloat();
                float durationRead = (hours * 60 * 60) + (mins * 60) + secs;
                if (durationRead > 0.0) {
                    setStatus(DOWNLOADSTATUS_CONVERTING);
                    percentRead = (durationRead / duration);
                    setProgress(percentRead);
                    qDebug() << "Progress " << (getProgress() * 100.0) << "%";
                }
            }
            else {
                QRegExp findDuration("^.*Duration: (\\d+):(\\d\\d):(\\d\\d\\.\\d+), .*$");
                foundPos = findDuration.indexIn(text);
                if (foundPos > -1) {
                    int hours = findDuration.cap(1).toInt();
                    int mins = findDuration.cap(2).toInt();
                    float secs = findDuration.cap(3).toFloat();
                    duration = (hours * 60 * 60) + (mins * 60) + secs;
                    qDebug() << "Duration: " << duration << "s (" << hours << ":" << mins << ":" << secs << ")";
                }
            }
        }
    }
}

void Download::started() {
    setStatus(DOWNLOADSTATUS_INITIALISING);
    //model.clear();
}

void Download::finished(int code) {
    if (process != NULL) {
        //delete process;
        process = NULL;
    }
    setStatus(DOWNLOADSTATUS_DONE);
}

void Download::readError(QProcess::ProcessError error)
{
    qDebug() << "Error: " << error;
    if (process != NULL) {
        QByteArray dataOut = process->readAllStandardOutput();
        QByteArray errorOut = process->readAllStandardError();

        qDebug() << "Output text: " << dataOut.data();
        qDebug() << "Error text: " << errorOut.data();
    }

    // Disconnect
    cancel();
}

float Download::getProgress() const {
    return progress;
}

void Download::setProgress(float value) {
    progress = value;
    emit progressChanged(value);
}
