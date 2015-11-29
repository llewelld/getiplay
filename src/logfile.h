#ifndef LOGFILE_H
#define LOGFILE_H

#include "GetiPlay.h"
#include "qfile.h"
#include "logfile.h"

class logfile
{
public:
    logfile();
    ~logfile();

    void openLog();
    void logLine(const QString &text);
    void closeLog();

private:
    QFile file;
    bool fileOpen;
};

#endif // LOGFILE_H
