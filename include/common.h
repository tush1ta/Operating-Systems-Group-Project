#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>

#define MAX_NAME  256
#define MAX_PROCS 4096

/* One process, as read from /proc/<pid>/stat */
typedef struct {
    pid_t  pid;
    char   name[MAX_NAME];
    char   state;          /* R, S, D, Z, T ... */
    double cpu_percent;
    long   rss_kb;         /* resident memory in KB */
    int    nice;
    int    priority;
} ProcInfo;

#endif
