#ifndef LOGFILE_H
#define LOGFILE_H

#include "harbour-getiplay.h"
#include "qfile.h"
#include "logfile.h"

#define LOG_LINES (50)

class logfile
{
public:
    logfile();
    ~logfile();

    void openLog();
    void logLine(const QString &text);
    void closeLog();
    void status();

private:
    QFile file;
    bool fileOpen;
};

#endif // LOGFILE_H
