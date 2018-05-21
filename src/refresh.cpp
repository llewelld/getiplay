#include "refresh.h"
#include <QDebug>
#include "harbour-getiplay.h"
#include "settings.h"

#define LINIEPROCESS_MAX (300)
#define OVERFLOWPOLL_TIMEOUT (10)

static const QString typeString[] = {"radio", "tv"};

Refresh::Refresh(QList<ProgModel*> model, QObject *parent) :
    QObject(parent),
    process(NULL),
    status(REFRESHSTATUS_INVALID),
    logText(""),
    model(model),
    periodCheck(false),
    periodCount(0),
    addingCount(0),
    addingTotal(1),
    lineProcessCount(0),
    progress(0.0),
    overflowpoll(nullptr),
    finishedcode(0),
    currentRefresh(Refresh::REFRESHTYPE_INVALID)
{
    arguments.clear();
}

void Refresh::initialise()
{
    setStatus(REFRESHSTATUS_UNINITIALISED);
    setProgress(-1.0f);
}

void Refresh::setStatus(REFRESHSTATUS newStatus)
{
    if (status != newStatus) {
        qDebug() << "Status changed from " << status << " to: " << newStatus;
        status = newStatus;
        emit statusChanged(newStatus);
    }
}

void Refresh::startRefresh(REFRESHTYPE type) {
    setLogText("");
    logToFile.openLog();
    logToFile.logLine("Process");

    if ((process != NULL) || (currentRefresh != REFRESHTYPE_INVALID)) {
        logToFile.logLine("Process already running.");
    }
    else {
        if ((type > REFRESHTYPE_INVALID) && (type < REFRESHTYPE_NUM)) {
            currentRefresh = type;
            process = new QProcess();
            process->setProcessChannelMode(QProcess::MergedChannels);
            process->setWorkingDirectory(DIR_BIN);
            process->setReadChannel(QProcess::StandardOutput);

            setupEnvironment();

#ifndef FAKE_GETIPLAYER
            QString program = DIR_BIN "/get_iplayer";
#else // !FAKE_GETIPLAYER
            QString program = "cat";
#endif // !FAKE_GETIPLAYER
            logToFile.logLine(program);

            collectArguments ();
            connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(readError(QProcess::ProcessError)));
            connect(process, SIGNAL(readyRead()), this, SLOT(readData()));
            connect(process, SIGNAL(started()), this, SLOT(started()));
            connect(process, SIGNAL(finished(int)), this, SLOT(finished(int)));
            logAppend(program + " " + arguments.join(" ")); // write the get_iplayer command to the log window
            process->start(program, arguments);
            process->closeWriteChannel();
            setStatus(REFRESHSTATUS_INITIALISING);
            periodCount = 0;
            addingCount = 0;
            addingTotal = model[currentRefresh]->rowCount();
            if (addingTotal == 0) {
                addingTotal = (currentRefresh == REFRESHTYPE_RADIO ? 13000 : 7500);
            }
            lineProcessCount = 0;
            setProgress(-1.0);
            arguments.clear();
            finishedcode = 0;
        }
    }
}

void Refresh::setupEnvironment() {
    // Set up appropriate environment variables to ensure get_iplayer can see
    // the installed binaries and libraries
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PERL5LIB", DIR_PERLLOCAL "/lib/perl5");
    env.insert("PATH", DIR_BIN ":" + env.value("PATH", ""));
    env.insert("PERL_UNICODE", "AS");
    process->setProcessEnvironment(env);
}

void Refresh::collectArguments () {
    arguments.clear();

#ifndef FAKE_GETIPLAYER
    addArgument("type=" + typeString[currentRefresh]);
    addArgument("refresh");
    addArgument("force");
    addArgument("nocopyright");
    addArgument("atomicparsley", "/usr/share/GetiPlay/bin/AtomicParsley");
    addArgument("ffmpeg", "/usr/share/GetiPlay/bin/ffmpeg");
    addArgument("ffmpeg-loglevel", "info");
    addArgument("log-progress");
    addArgument("profile-dir", Settings::getProfileDir());
    addValue(".*");
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
        logAppend("Terminate signal sent");
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
                setProgressCount(periodCount, addingCount);
            }
            else {
                qDebug() << "Switching to line check.";
                periodCheck = false;
            }
        }
    }
    lineProcessCount = 0;
    while ((periodCheck == false) && (process->canReadLine()) && (lineProcessCount < LINIEPROCESS_MAX)) {
        QByteArray read = process->readLine();

        interpretData(read);
        lineProcessCount++;
    }
    //qDebug() << "Progress " << (getProgress() * 100.0) << "%";
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
        logAppend(text);
    }
    else if (text.startsWith("INFO: Indexing")) {
        logAppend(text);
        setStatus(REFRESHSTATUS_DOWNLOADING);
        qDebug() << "Switching to period check.";
        periodCheck = true;
    }
    else if (text.startsWith("INFO: Added ")) {
        setStatus(REFRESHSTATUS_PROCESSING);
        logAppend(text);
    }
    else {
        QRegExp progInfo("^(\\d+):\\t(.*)$");
        int foundPos = progInfo.indexIn(text);
        if (foundPos > -1) {
            unsigned int progId = progInfo.cap(1).toUInt();
            QString title = progInfo.cap(2);

            //qDebug() << "Programme: " << progId << ", " << title;
            model[currentRefresh]->addProgramme(Programme(progId, title, 0.0));
            addingCount++;
            setProgressCount(periodCount, addingCount);
        }
        else {
            logAppend(text);
        }
    }
}

void Refresh::started() {
    setStatus(REFRESHSTATUS_INITIALISING);
    model[currentRefresh]->clear();
}

void Refresh::finished(int code) {
    finishedcode = code;
    setStatus(REFRESHSTATUS_OVERFLOW);

    overflowpoll = new QTimer(this);
    overflowpoll->setSingleShot(true);
    overflowpoll->setInterval(OVERFLOWPOLL_TIMEOUT);
    connect(overflowpoll, SIGNAL(timeout()), this, SLOT(overflow()));
    overflowpoll->start();
}

void Refresh::overflow() {
    if (process->bytesAvailable()) {
        readData();
        overflowpoll->start();
    }
    else {
        logAppend("Download periods: " + QString::number(periodCount));
        logAppend("Processed entries: " + QString::number(addingCount));
        logAppend("Finished with code " + QString::number(finishedcode));
        logToFile.closeLog();
        if (process != NULL) {
            //delete process;
            process = NULL;
        }
        currentRefresh = REFRESHTYPE_INVALID;
        setStatus(REFRESHSTATUS_DONE);
        disconnect(overflowpoll, SIGNAL(timeout()), this, SLOT(overflow()));
        overflowpoll->stop();
        delete overflowpoll;
        overflowpoll = nullptr;
    }
}

void Refresh::readError(QProcess::ProcessError error)
{
    logToFile.logLine("Error: " + error);
    if (process != NULL) {
        QByteArray dataOut = process->readAllStandardOutput();
        QByteArray errorOut = process->readAllStandardError();

        logToFile.logLine(QString("Output text: ") + dataOut.data());
        logToFile.logLine(QString("Error text: ") + errorOut.data());
    }
}

float Refresh::getProgress() const {
    return progress;
}

void Refresh::setProgress(float value) {
    progress = value;
    emit progressChanged(progress);
}

void Refresh::setProgressCount(int periodCount, int addingCount) {
    float progress;
    float periodProgress;
    float addingProgress;

    if (status > REFRESHSTATUS_INITIALISING) {
        periodProgress = 0.75 * (((float)periodCount / ((float)periodCount + 30.0)) + ((float)periodCount * 0.003f));
        addingProgress = ((float)addingCount / (float)(addingTotal));

        progress = periodProgress + qMax((1.0 - periodProgress), 0.0) * addingProgress;

        if (progress > this->progress) {
            this->progress = progress;
            emit progressChanged(progress);
        }
    }
}

QString Refresh::getLogText() const
{
    return logText;
}

void Refresh::setLogText(const QString &value)
{
    logText = value;
    emit logTextChanged(logText);
}

void Refresh::logAppend(const QString &text)
{
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
            if (nextLine > 0) {
                logText = logText.mid(nextLine + 1);
            }
            removeLines--;
        }

        // Add new lines
        logText.append(append);
        emit logTextChanged(logText);
    }
}

