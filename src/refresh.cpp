#include "refresh.h"
#include <QDebug>
#include "harbour-getiplay.h"
#include "settings.h"
#include "log.h"

#define LINIEPROCESS_MAX (20)
#define OVERFLOWPOLL_TIMEOUT (10)

static const QString typeString[] = {"radio", "tv"};

#define STRINGSEP "|"

static const QStringList listformat = {"pid", "episode", "duration", "channel", "available", "web", "name", "desc", "thumbnail"};

Refresh::Refresh(QObject *parent, QList<ProgModel*> model, Log *log) :
    QObject(parent),
    process(NULL),
    status(REFRESHSTATUS_INVALID),
    model(model),
    periodCheck(false),
    periodCount(0),
    addingCount(0),
    addingTotal(1),
    lineProcessCount(0),
    progress(0.0),
    overflowpoll(nullptr),
    finishedcode(0),
    currentRefresh(Settings::REFRESHTYPE_INVALID),
    log(log)
{
    arguments.clear();
    temp.clear();
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

void Refresh::startRefresh(int type) {
    LOGAPPEND("\n");
    LOGAPPENDTIMESTAMP("Starting new process");

    if ((process != NULL) || (currentRefresh != Settings::REFRESHTYPE_INVALID)) {
        LOGAPPEND("Process already running.");
    }
    else {
        if ((type > Settings::REFRESHTYPE_INVALID) && (type < Settings::REFRESHTYPE_NUM)) {
            currentRefresh = (Settings::REFRESHTYPE)type;
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
            setStatus(REFRESHSTATUS_INITIALISING);
            periodCount = 0;
            addingCount = 0;
            addingTotal = model[currentRefresh]->rowCount();
            if (addingTotal == 0) {
                addingTotal = (currentRefresh == Settings::REFRESHTYPE_RADIO ? 13000 : 7500);
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
    env.insert("PERL_UNICODE", "AS");

    process->setProcessEnvironment(env);
}

const QString & Refresh::includeTypeToString(Settings::PROGTYPE refreshType) {
    static const QString types[Settings::PROGTYPE_NUM] = {"national", "regional", "local", "national,regional,local"};

    return types[qBound((int)0, (int)refreshType, (int)(Settings::PROGTYPE_NUM - 1))];
}

const QString & Refresh::excludeTypeToString(Settings::PROGTYPE refreshType) {
    static const QString types[Settings::PROGTYPE_NUM] = {"regional,local", "national,local", "national,regional", ""};

    return types[qBound((int)0, (int)refreshType, (int)(Settings::PROGTYPE_NUM - 1))];
}

void Refresh::collectArguments () {
    Settings::PROGTYPE refreshType = Settings::getInstance().getRefreshType();
    QString proxy = Settings::getInstance().getProxyUrl();
    bool rebuildCache = Settings::getInstance().getRebuildCache(currentRefresh);
    QString include = includeTypeToString(refreshType);
    QString exclude = excludeTypeToString(refreshType);
    arguments.clear();

    addArgument("type=" + typeString[currentRefresh]);
    addArgument("refresh");
    addArgument("force");
    addArgument("nocopyright");
    addArgument("nopurge");
    if (rebuildCache) {
        addArgument("cache-rebuild");
    }
    if (include != "") {
        addArgument("refresh-include-groups", include);
    }
    if (exclude != "") {
        addArgument("refresh-exclude-groups", exclude);
    }
    addArgument("index-maxconn", QString("%1").arg(Settings::getInstance().getIndexMaxConn()));
    addArgument("atomicparsley", DIR_BIN "/AtomicParsley");
    addArgument("ffmpeg", DIR_BIN "/ffmpeg");
    addArgument("ffmpeg-loglevel", "info");
    addArgument("log-progress");
    addArgument("profile-dir", Settings::getProfileDir());
    (proxy != "") ? addArgument("proxy", proxy): addArgument("no-proxy");
    addArgument("listformat", "<" + listformat.join(">" STRINGSEP "<") + ">");
    addValue(".*");
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
    currentRefresh = Settings::REFRESHTYPE_INVALID;
    if (process != NULL) {
        process->terminate();
        LOGAPPEND("Terminate signal sent");
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
    bool found;

    //qDebug() << "Line: " << text;
    if (text.endsWith("Matching Programmes\n")) {
        LOGAPPEND(text);
    }
    else if (text.startsWith("INFO: Indexing")) {
        LOGAPPEND(text);
        setStatus(REFRESHSTATUS_DOWNLOADING);
        qDebug() << "Switching to period check.";
        periodCheck = true;
    }
    else if (text.startsWith("INFO: Added ")) {
        setStatus(REFRESHSTATUS_PROCESSING);
        LOGAPPEND(text);
    }
    else {
        found = interpretProgramme(text);
        if (found) {
            addingCount++;
            setProgressCount(periodCount, addingCount);
        }
        else {
            LOGAPPEND(text);
        }
    }
}

bool Refresh::interpretProgramme(const QString &text) {
    QStringList split;
    bool success;
    QString pid = "";
    QString episode = "";
    qint32 duration = 0;
    QString channel = "";
    qint64 available = 0;
    QString web = "";
    QString name = "";
    QString desc = "";
    QString imageId = "";

    split = text.split(STRINGSEP, QString::KeepEmptyParts, Qt::CaseSensitive);
    success = (split.size() == listformat.size());

    if (success) {
        for (int property = 0; property < split.size(); property++) {
            switch (property) {
                case 0: // "pid"
                pid = split[property];
                break;
                case 1: // "episode"
                episode = split[property];
                break;
                case 2: // "duration"
                duration = split[property].toLongLong();
                break;
                case 3: // "channel"
                channel = split[property];
                break;
                case 4: // "available"
                available = Settings::dateToEpoch(split[property]);
                break;
                case 5: // "web"
                web = split[property];
                break;
                case 6: // "name"
                name = split[property];
                break;
                case 7: // "desc"
                desc = split[property];
                break;
                case 8: // "imageId"
                QString url = split[property];
                if ((url.endsWith(".jpg")) || (url.endsWith(".png"))) {
                    imageId = "";
                    int start = url.lastIndexOf('/');
                    if (start >= 0) {
                        imageId = url.mid(start + 1);
                    }
                }
                break;
            }
        }

        temp.addProgramme(Programme(pid, name, duration, available, channel, episode, web, desc, imageId));
    }

    return success;
}

void Refresh::started() {
    setStatus(REFRESHSTATUS_INITIALISING);
    temp.clear();
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
        LOGAPPEND("Download periods: " + QString::number(periodCount));
        LOGAPPEND("Processed entries: " + QString::number(addingCount));
        LOGAPPENDTIMESTAMP("Finished with code " + QString::number(finishedcode));
        if (process != NULL) {
            //delete process;
            process = NULL;
        }

        // Transfer the model over
        model[currentRefresh]->replaceAll(temp);
        temp.clear();

        // Clean up
        currentRefresh = Settings::REFRESHTYPE_INVALID;
        setStatus(REFRESHSTATUS_DONE);
        disconnect(overflowpoll, SIGNAL(timeout()), this, SLOT(overflow()));
        overflowpoll->stop();
        delete overflowpoll;
        overflowpoll = nullptr;
    }
}

void Refresh::readError(QProcess::ProcessError error)
{
    LOGAPPEND("Error: " + error);
    if (process != NULL) {
        QByteArray dataOut = process->readAllStandardOutput();
        QByteArray errorOut = process->readAllStandardError();

        LOGAPPEND(QString("Output text: ") + dataOut.data());
        LOGAPPEND(QString("Error text: ") + errorOut.data());
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
