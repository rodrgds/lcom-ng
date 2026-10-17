// Small function that writes to a log file to help debugging

#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define LOGFILE "/home/lcom/repos/projeto/log1.log" // all Log(); messages will be appended to this file

extern int LogCreated; // keeps track whether the log file is created or not

void Log (char *message); // logs a message to LOGFILE

#endif
