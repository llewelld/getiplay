#include "refresh.h"
#include "stdio.h"
#include <QDebug>
#include "GetiPlay.h"

//#define FAKE_GETIPLAYER (1)

static const QString typeString[] = {"radio", "tv"};

Refresh::Refresh(QList<ProgModel*> model, QObject *parent) :
    QObject(parent),
    process(NULL),
    status(REFRESHSTATUS_INVALID),
    model(model),
    periodCheck(false),
    periodCount(0),
    progress(0.0),
    currentRefresh(REFRESHTYPE_INVALID)
{
    arguments.clear();
}

void Refresh::initialise()
{
    setStatus(REFRESHSTATUS_UNINITIALISED);
    periodCount = 0;
    setProgress(-1.0f);
}

void Refresh::setStatus(REFRESHSTATUS newStatus)
{
    if (status != newStatus) {
        status = newStatus;
        emit statusChanged(newStatus);
    }
}

void Refresh::startRefresh(REFRESHTYPE type) {
    qDebug() << "Process";

    if ((process != NULL) || (currentRefresh != REFRESHTYPE_INVALID)) {
        qDebug() << "Process already running.";
    }
    else {
        if ((type > REFRESHTYPE_INVALID) && (type < REFRESHTYPE_NUM)) {
            currentRefresh = type;
            process = new QProcess();
    #ifndef FAKE_GETIPLAYER
            QString program = DIR_BIN "/get_iplayer";
    #else // !FAKE_GETIPLAYER
            QString program = "cat";
    #endif // !FAKE_GETIPLAYER
            qDebug() << program;
            collectArguments ();
            process->setReadChannel(QProcess::StandardOutput);
            connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(readError(QProcess::ProcessError)));
            connect(process, SIGNAL(readyRead()), this, SLOT(readData()));
            connect(process, SIGNAL(started()), this, SLOT(started()));
            connect(process, SIGNAL(finished(int)), this, SLOT(finished(int)));

            process->start(program, arguments);
            process->closeWriteChannel();
            setStatus(REFRESHSTATUS_INITIALISING);
            setProgress(-1.0);
            periodCount = 0;
            arguments.clear();
        }
    }
}

void Refresh::collectArguments () {
    arguments.clear();

#ifndef FAKE_GETIPLAYER
    addArgument("type=" + typeString[currentRefresh]);
    addArgument("refresh");
    addArgument("force");
#else // !FAKE_GETIPLAYER
    addValue("../share/" APP_NAME "/output01.txt");
#endif // !FAKE_GETIPLAYER
}

void Refresh::addArgument (QString key, QString value) {
    QString argument;

    argument = "--" + key;
    arguments.append(argument);
    if (!value.isEmpty()) {
        arguments.append(value);
    }
}

void Refresh::addArgumentNonempty (QString key, QString value) {
    QString argument;

    if (!value.isEmpty()) {
        argument = "--" + key;
        arguments.append(argument);
        arguments.append(value);
    }
}

void Refresh::addArgument (QString key) {
    QString argument;

    argument = "--" + key;
    arguments.append(argument);
}

void Refresh::addOption (QString key, bool add) {
    if (add) {
        addArgument (key);
    }
}

void Refresh::addValue (QString key) {
    arguments.append(key);
}

void Refresh::cancel() {
    currentRefresh = REFRESHTYPE_INVALID;
    if (process != NULL) {

        process->terminate();
    }
    setStatus(REFRESHSTATUS_CANCEL);
}

void Refresh::readData() {
    char data[1];
    while ((periodCheck == true) && (process->bytesAvailable() > 0)) {
        int read = process->peek(data, 1);
        if (read > 0) {
            if (data[0] == '.') {
                // Remove the character
                process->read(data, 1);
                periodCount++;
                setProgress(((float)periodCount / ((float)periodCount + 10.0)) + ((float)periodCount * 0.002f));
                qDebug() << "Progress " << (getProgress() * 100.0) << "%";
            }
            else {
                qDebug() << "Switching to line check.";
                periodCheck = false;
            }
        }
    }
    while ((periodCheck == false) && (process->canReadLine())) {
        QByteArray read = process->readLine();

        interpretData(read);
    }
}

void Refresh::interpretData(const QString &text) {
    if (!text.isEmpty()) {
        QStringList lines = text.split('\n', QString::SkipEmptyParts);
        for (QStringList::iterator iter = lines.begin(); iter != lines.end(); ++iter) {
            interpretLine(*iter);
        }
    }
}

void Refresh::interpretLine(const QString &text) {
    //qDebug() << "Line: " << text;
    if (text.endsWith("Matching Programmes\n")) {
        setStatus(REFRESHSTATUS_DONE);
    }
    else if (text.endsWith("(this may take a few minutes)")) {
        setStatus(REFRESHSTATUS_REFRESHING);
        qDebug() << "Switching to period check.";
        periodCheck = true;
    }
    else {
        QRegExp progInfo("^(\\d+):\\t(.*)$");
        int foundPos = progInfo.indexIn(text);
        if (foundPos > -1) {
            unsigned int progId = progInfo.cap(1).toUInt();
            QString title = progInfo.cap(2);

            //qDebug() << "Programme: " << progId << ", " << title;
            model[currentRefresh]->addProgramme(Programme(progId, title, 0.0));
        }
    }
}

void Refresh::started() {
    setStatus(REFRESHSTATUS_INITIALISING);
    model[currentRefresh]->clear();
}

void Refresh::finished(int code) {
    qDebug() << "Finished with code " << code;
    if (process != NULL) {
        //delete process;
        process = NULL;
    }
    currentRefresh = REFRESHTYPE_INVALID;
    setStatus(REFRESHSTATUS_DONE);
}

void Refresh::readError(QProcess::ProcessError error)
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

float Refresh::getProgress() const {
    return progress;
}

void Refresh::setProgress(float value) {
    progress = value;
    emit progressChanged(value);
}

