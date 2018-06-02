#include "metaget.h"
#include <QDebug>
#include "harbour-getiplay.h"
#include "settings.h"

#define MAX_LINE_LENGTH (255)

Metaget::Metaget(QObject *parent, Log *log) :
    QObject(parent),
    process(NULL),
    status(Metaget::STATUS_INVALID),
    progId(""),
    log(log),
    metadata("")
{
    arguments.clear();
}

void Metaget::initialise()
{
    setStatus(Metaget::STATUS_UNINITIALISED);
}

void Metaget::setStatus(Metaget::STATUS newStatus)
{
    if (status != newStatus) {
        status = newStatus;
        emit statusChanged(newStatus);
    }
}

void Metaget::startDownload(QString progId, int progType) {
    LOGAPPEND("\nStarting new process");

    this->progId = progId;
    this->progType = progType;
    this->metadata = "";

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
        setStatus(Metaget::STATUS_INITIALISING);
        arguments.clear();
    }
}

void Metaget::setupEnvironment() {
    // Set up appropriate environment variables to ensure get_iplayer can see
    // the installed binaries and libraries
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PERL5LIB", DIR_PERLLOCAL "/lib/perl5");
    env.insert("PERL_UNICODE", "AS");
    process->setProcessEnvironment(env);
}

void Metaget::collectArguments () {
    arguments.clear();

    // get-iplayer --type=tv --output temp --metadata-only --thumbnail-size 640 7850 --file-prefix "tempn.txt"

    if (progType == 0) {
        addArgument("type=radio");
    } else if (progType == 1) {
        addArgument("type=tv");
    }

    addArgument("nocopyright");
    addArgument("atomicparsley", DIR_BIN "/AtomicParsley");
    addArgument("ffmpeg", DIR_BIN "/ffmpeg");
    addArgument("ffmpeg-loglevel", "info");
    addArgument("profile-dir", Settings::getProfileDir());
    addArgument("output", Settings::getTempDir());
    addArgument("expiry=99999999");
    addArgument("metadata-only");
    addArgument("thumbnail-size", "640");
    addArgument("file-prefix", "metadata");
    addArgument("pid", progId);
}

void Metaget::addArgument (QString key, QString value) {
    QString argument;

    argument = "--" + key;
    arguments.append(argument);
    if (!value.isEmpty()) {
        arguments.append(value);
    }
}

void Metaget::addArgumentNonempty (QString key, QString value) {
    QString argument;

    if (!value.isEmpty()) {
        argument = "--" + key;
        arguments.append(argument);
        arguments.append(value);
    }
}

void Metaget::addArgument (QString key) {
    QString argument;

    argument = "--" + key;
    arguments.append(argument);
}

void Metaget::addOption (QString key, bool add) {
    if (add) {
        addArgument (key);
    }
}

void Metaget::addValue (QString key) {
    arguments.append(key);
}

void Metaget::cancel() {
    if (process != NULL) {
        process->terminate();
        LOGAPPEND("Terminate signal sent");
    }
    setStatus(Metaget::STATUS_CANCEL);
}

void Metaget::readData() {
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

void Metaget::interpretData(const QString &text) {
    if (!text.isEmpty()) {
        QStringList lines = text.split('\n', QString::SkipEmptyParts);
        for (QStringList::iterator iter = lines.begin(); iter != lines.end(); ++iter) {
            interpretLine(*iter);
        }
    }
}

void Metaget::interpretLine(const QString &text) {
    // Just log it
    LOGAPPEND(text);
}

void Metaget::started() {
    setStatus(Metaget::STATUS_INITIALISING);
    //model.clear();
}

void Metaget::finished(int code) {
    LOGAPPEND("Finished with code " + QString::number(code));
    // Load in the metadata
    if (!code) {
        QFile file(Settings::getTempDir() + "/metadata.xml");
        if (file.open(QFileDevice::ReadOnly)) {
            metadata = file.readAll();
            qDebug() << metadata;
        }
    }

    if (process != NULL) {
        //delete process;
        process = NULL;
    }

    // anything but a zero exit status is an error
    if (!code) {
        setStatus(Metaget::STATUS_DONE);
    } else {
        setStatus(Metaget::STATUS_ERROR);
    }
}

void Metaget::readError(QProcess::ProcessError error)
{
    LOGAPPEND("Error: " + error);
    if (process != NULL) {
        QByteArray dataOut = process->readAllStandardOutput();
        QByteArray errorOut = process->readAllStandardError();

        LOGAPPEND(QString("Output text: ") + dataOut.data());
        LOGAPPEND(QString("Error text: ") + errorOut.data());
    }
}

Metaget::STATUS Metaget::getStatus() {
    return status;
}

QString Metaget::metagot() {
    return metadata;
}

