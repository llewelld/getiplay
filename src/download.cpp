#include "download.h"
#include "stdio.h"
#include <QDebug>

#define FAKE_GETIPLAYER (1)

Download::Download(QObject *parent) :
    QObject(parent),
    process(NULL),
    status(DOWNLOADSTATUS_INVALID),
    progId(0),
    percentRead(0.0f)
{
    arguments.clear();
}

void Download::initialise()
{
    setStatus(DOWNLOADSTATUS_UNINITIALISED);
    percentRead = -1.0f;
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
        qDebug() << "Process already running." << endl;
    }
    else {
        process = new QProcess();
#ifndef FAKE_GETIPLAYER
        QString program = "/home/nemo/Documents/Development/Projects/get_iplayer/get_iplayer";
#else // !FAKE_GETIPLAYER
        QString program = "cat";
#endif // !FAKE_GETIPLAYER
        collectArguments ();
        process->setReadChannel(QProcess::StandardOutput);
        connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(readError(QProcess::ProcessError)));
        connect(process, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(process, SIGNAL(started()), this, SLOT(started()));
        connect(process, SIGNAL(finished(int)), this, SLOT(finished(int)));

        process->start(program, arguments);
        process->closeWriteChannel();
        setStatus(DOWNLOADSTATUS_INITIALISING);
        percentRead = -1.0f;
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
    while (process->canReadLine()) {
        QByteArray read = process->readLine();
        //printf ("Output: %s", read.data());

        interpretData(read);
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
    qDebug() << "Line: " << text;
    if (text.startsWith("INFO: Recorded ")) {
        setStatus(DOWNLOADSTATUS_DONE);
    }
    else {
        QRegExp percent("^.*\\((\\d+\\.\\d+)%\\)$");
        int foundPos = percent.indexIn(text);
        if (foundPos > -1) {
            percentRead = percent.cap(1).toFloat() / 100.0f;
            qDebug() << "Progress " << (progress() * 100.0) << "%" << endl;
        }
    }
}

void Download::started() {
    setStatus(DOWNLOADSTATUS_DOWNLOADING);
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
    qDebug() << "Error: " << error << endl;
    if (process != NULL) {
        QByteArray dataOut = process->readAllStandardOutput();
        QByteArray errorOut = process->readAllStandardError();

        qDebug() << "Output text: " << dataOut.data() << endl;
        qDebug() << "Error text: " << errorOut.data() << endl;
    }

    // Disconnect
    cancel();
}

float Download::progress() const {
    return percentRead;
}
