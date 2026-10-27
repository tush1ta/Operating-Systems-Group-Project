#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"

int  logger_open(const char *csv_path);
void logger_log(const char *event, pid_t pid, const char *detail);
void logger_close(void);

#endif
