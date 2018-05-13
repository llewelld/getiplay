#include "refresh.h"
#include "stdio.h"
#include <QDebug>
#include "GetiPlay.h"

static const QString typeString[] = {"radio", "tv"};

Refresh::Refresh(QList<ProgModel*> model, QObject *parent) :
    QObject(parent),
    process(NULL),
    status(REFRESHSTATUS_INVALID),
    logText(""),
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
            foreach (QString argument, arguments) {
                logToFile.logLine(argument);
            }

            process->start(program, arguments);
            process->closeWriteChannel();
            setStatus(REFRESHSTATUS_INITIALISING);
            setProgress(-1.0);
            periodCount = 0;
            arguments.clear();
        }
    }
}

void Refresh::setupEnvironment() {
    // Set up appropriate environment variables to ensure get_iplayer can see
    // the installed binaries and libraries
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_LIBRARY_PATH", DIR_LIB ":" + env.value("LD_LIBRARY_PATH"));
    env.insert("PERL_MB_OPT", "--install_base \"" DIR_PERLLOCAL "\"");
    env.insert("PERL_MM_OPT", "INSTALL_BASE=" DIR_PERLLOCAL);
    env.insert("PERL5LIB", DIR_PERLLOCAL "/lib/perl5");
    env.insert("PERL_LOCAL_LIB_ROOT", DIR_PERLLOCAL);
    env.insert("PATH", DIR_PERLLOCAL "/bin:" DIR_BIN ":" + env.value("PATH", ""));
    process->setProcessEnvironment(env);
    foreach (QString var, env.toStringList()) {
        qDebug() << var;
    }
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
                setProgress(((float)periodCount / ((float)periodCount + 20.0)) + ((float)periodCount * 0.0005f));
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
        logAppend(text);
        //setStatus(REFRESHSTATUS_DONE);
    }
    else if (text.startsWith("INFO: Indexing")) {
        logAppend(text);
        setStatus(REFRESHSTATUS_REFRESHING);
        qDebug() << "Switching to period check.";
        periodCheck = true;
    }
    else if (text.startsWith("Added: ")) {
        // Do nothing
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
    logToFile.logLine("Finished with code " + QString::number(code));
    logAppend("Finished with code " + QString::number(code));
    logToFile.closeLog();
    if (process != NULL) {
        //delete process;
        process = NULL;
    }
    currentRefresh = REFRESHTYPE_INVALID;
    setStatus(REFRESHSTATUS_DONE);
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
    emit progressChanged(value);
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

