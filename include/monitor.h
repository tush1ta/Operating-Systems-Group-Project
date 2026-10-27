#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

/* Fill out[] with up to max processes. Returns count, or -1 on error. */
int monitor_list(ProcInfo *out, int max);

/* Fill info for one PID. Returns 0 or -1. */
int monitor_get(pid_t pid, ProcInfo *info);

/* Print the top n processes by CPU usage. */
void monitor_print_top(int n);

#endif
