#include "download.h"
#include <QDebug>
#include "harbour-getiplay.h"
#include "settings.h"

#define MAX_LINE_LENGTH (255)

Download::Download(QObject *parent, Log *log) :
    QObject(parent),
    process(NULL),
    status(DOWNLOADSTATUS_INVALID),
    progId(""),
    duration(0.0),
    progress(0.0),
    log(log)
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

void Download::startDownload(QString progId, QString progType) {
    LOGAPPEND("STARTING");

    this->progId = progId;
    this->progType = progType;

    if (process != NULL) {
        LOGAPPEND("Process already running.");
    }
    else {
        process = new QProcess();
        QString program = DIR_BIN "/get_iplayer";
        process->setWorkingDirectory(DIR_BIN);
        setupEnvironment();
        collectArguments ();
        process->setProcessChannelMode(QProcess::MergedChannels);
        process->setReadChannel(QProcess::StandardOutput);
        connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(readError(QProcess::ProcessError)));
        connect(process, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(process, SIGNAL(started()), this, SLOT(started()));
        connect(process, SIGNAL(finished(int)), this, SLOT(finished(int)));
        // Write the get_iplayer command to the log window
        LOGAPPEND(program + " " + arguments.join(" "));
        process->start(program, arguments);
        process->closeWriteChannel();
        setStatus(DOWNLOADSTATUS_INITIALISING);
        setProgress(-1.0);
        duration = 0.0;
        arguments.clear();
    }
}

void Download::setupEnvironment() {
    // Set up appropriate environment variables to ensure get_iplayer can see
    // the installed binaries and libraries
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PERL5LIB", DIR_PERLLOCAL "/lib/perl5");
    env.insert("PERL_UNICODE", "AS");
    process->setProcessEnvironment(env);
}

void Download::collectArguments () {
    arguments.clear();

    if (progType == QString("radio")) {
        addArgument("type=radio");
    } else if (progType == QString("tv")) {
        addArgument("type=tv");
    }

    addArgument("get");
    addArgument("pid", QString("%1").arg(progId));
    addArgument("force");
    addArgument("nocopyright");
    addArgument("atomicparsley", DIR_BIN "/AtomicParsley");
    addArgument("ffmpeg", DIR_BIN "/ffmpeg");
    addArgument("ffmpeg-loglevel", "info");
    addArgument("expiry=99999999");
    addArgument("log-progress");
    addArgument("profile-dir", Settings::getProfileDir());

    if (progType == QString("radio")) {
        addArgument("output", Settings::getMusicDir());
    } else if (progType == QString("tv")) {
        addArgument("output", Settings::getVideoDir());
    } else {
        addArgument("output", Settings::getDownloadsDir());
    }
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
        LOGAPPEND("Terminate signal sent");
    }
    setStatus(DOWNLOADSTATUS_CANCEL);
}

void Download::readData() {
    // Read any full lines if they're available
    while (process->canReadLine()) {
        QByteArray read = process->readLine();
        //printf ("Output: %s", read.data());

        interpretData(read);
    }

    // Read any lines that are full lines but don't register
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
        LOGAPPEND(text);
        //setStatus(DOWNLOADSTATUS_DONE);
    }
    else {
        QRegExp percent("^\\s+(\\d+\\.\\d+)%\\s.*$");
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
                LOGAPPEND(text);
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
    LOGAPPEND("Finished with code " + QString::number(code));
    if (process != NULL) {
        //delete process;
        process = NULL;
    }

    // anything but a zero exit status is an error
    if (!code) {
        setStatus(DOWNLOADSTATUS_DONE);
    } else {
        setStatus(DOWNLOADSTATUS_ERROR);
    }
}

void Download::readError(QProcess::ProcessError error)
{
    LOGAPPEND("Error: " + error);
    if (process != NULL) {
        QByteArray dataOut = process->readAllStandardOutput();
        QByteArray errorOut = process->readAllStandardError();

        LOGAPPEND(QString("Output text: ") + dataOut.data());
        LOGAPPEND(QString("Error text: ") + errorOut.data());
    }
}

float Download::getProgress() const {
    return progress;
}

void Download::setProgress(float value) {
    progress = value;
    emit progressChanged(value);
}

DOWNLOADSTATUS Download::getStatus() {
    return status;
}

