#include "download.h"
#include "stdio.h"

Download::Download(QObject *parent) :
    QObject(parent),
    process(NULL),
    status(DOWNLOADSTATUS_INVALID)
{
    arguments.clear();
}

void Download::initialise()
{
    setStatus(DOWNLOADSTATUS_UNINITIALISED);
}

void Download::setStatus(DOWNLOADSTATUS newStatus)
{
    if (status != newStatus) {
        status = newStatus;
        emit statusChanged(newStatus);
    }
}

void Download::startDownload() {
    if (process != NULL) {
        printf ("Process already running.\n");
    }
    else {
        process = new QProcess();
        QString program = "cat";
        collectArguments ();
        process->setReadChannel(QProcess::StandardOutput);
        connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(readError(QProcess::ProcessError)));
        connect(process, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(process, SIGNAL(started()), this, SLOT(started()));
        connect(process, SIGNAL(finished(int)), this, SLOT(finished(int)));

        process->start(program, arguments);
        process->closeWriteChannel();
        setStatus(DOWNLOADSTATUS_INITIALISING);
        arguments.clear();
    }
}

void Download::collectArguments () {
    arguments.clear();

    addValue("/opt/sdk/GetiPlay/usr/share/GetiPlay/output01.txt");
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

        if (read.endsWith("Matching Programmes\n")) {
            setStatus(DOWNLOADSTATUS_DONE);
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
    if (text.endsWith("Matching Programmes\n")) {
        setStatus(DOWNLOADSTATUS_DONE);
    }
    else {
        QRegExp progInfo("^(\\d+):\\t(.*)$");
        int foundPos = progInfo.indexIn(text);
        if (foundPos > -1) {
            unsigned int progId = progInfo.cap(1).toUInt();
            QString title = progInfo.cap(2);

            //printf ("Programme: %u, %s\n", progId, title.toStdString().c_str());
            //model.addProgramme(Programme(progId, title, 0.0));
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
    printf ("Error: %d\n", error);
    if (process != NULL) {
        QByteArray dataOut = process->readAllStandardOutput();
        QByteArray errorOut = process->readAllStandardError();

        printf ("Output text: %s\n", dataOut.data());
        printf ("Error text: %s\n", errorOut.data());
    }

    // Disconnect
    cancel();
}
