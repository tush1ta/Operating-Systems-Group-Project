#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include "monitor.h"

#define SAMPLE_US 500000   /* time between the two CPU samples (0.5 s) */

/* Read /proc/<pid>/stat into p. ticks gets utime+stime (CPU time used). */
static int read_stat(pid_t pid, ProcInfo *p, unsigned long *ticks)
{
    char path[64], buf[1024];
    snprintf(path, sizeof path, "/proc/%d/stat", (int)pid);

    FILE *f = fopen(path, "r");
    if (!f) return -1;
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    fclose(f);
    if (n == 0) return -1;
    buf[n] = '\0';

    /* The name is in brackets and may contain spaces, so find the LAST ')' */
    char *lp = strchr(buf, '(');
    char *rp = strrchr(buf, ')');
    if (!lp || !rp || rp < lp) return -1;

    size_t len = (size_t)(rp - lp - 1);
    if (len >= MAX_NAME) len = MAX_NAME - 1;
    memcpy(p->name, lp + 1, len);
    p->name[len] = '\0';

    char state;
    unsigned long utime, stime;
    long prio, nice, rss;
    int got = sscanf(rp + 2,
        "%c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u "
        "%lu %lu %*ld %*ld %ld %ld %*ld %*ld %*llu %*lu %ld",
        &state, &utime, &stime, &prio, &nice, &rss);
    if (got != 6) return -1;

    p->pid = pid;
    p->state = state;
    p->priority = (int)prio;
    p->nice = (int)nice;
    p->rss_kb = rss * (sysconf(_SC_PAGESIZE) / 1024);
    p->cpu_percent = 0.0;
    *ticks = utime + stime;
    return 0;
}

static double cpu_from_ticks(unsigned long before, unsigned long after)
{
    double hz = (double)sysconf(_SC_CLK_TCK);
    double seconds = SAMPLE_US / 1000000.0;
    return ((double)(after - before) / hz) / seconds * 100.0;
}

int monitor_get(pid_t pid, ProcInfo *info)
{
    unsigned long a, b;
    ProcInfo now;

    if (read_stat(pid, info, &a) != 0) return -1;
    usleep(SAMPLE_US);
    if (read_stat(pid, &now, &b) != 0) return -1;

    now.cpu_percent = cpu_from_ticks(a, b);
    *info = now;
    return 0;
}

int monitor_list(ProcInfo *out, int max)
{
    static unsigned long before[MAX_PROCS];
    DIR *d = opendir("/proc");
    if (!d) return -1;

    int n = 0;
    struct dirent *e;
    while ((e = readdir(d)) != NULL && n < max && n < MAX_PROCS) {
        if (!isdigit((unsigned char)e->d_name[0])) continue;
        pid_t pid = (pid_t)atoi(e->d_name);
        if (read_stat(pid, &out[n], &before[n]) == 0) n++;
    }
    closedir(d);

    usleep(SAMPLE_US);

    for (int i = 0; i < n; i++) {
        ProcInfo now;
        unsigned long after;
        if (read_stat(out[i].pid, &now, &after) == 0) {
            now.cpu_percent = cpu_from_ticks(before[i], after);
            out[i] = now;
        } else {
            out[i].cpu_percent = 0.0;   /* process ended meanwhile */
        }
    }
    return n;
}

static int by_cpu_desc(const void *x, const void *y)
{
    const ProcInfo *a = x, *b = y;
    if (b->cpu_percent > a->cpu_percent) return 1;
    if (b->cpu_percent < a->cpu_percent) return -1;
    return 0;
}

void monitor_print_top(int n)
{
    static ProcInfo list[MAX_PROCS];
    int count = monitor_list(list, MAX_PROCS);
    if (count < 0) {
        fprintf(stderr, "cannot read /proc\n");
        return;
    }
    qsort(list, (size_t)count, sizeof list[0], by_cpu_desc);
    if (n > count) n = count;

    printf("%-7s %-20s %-5s %7s %9s %4s %4s\n",
           "PID", "NAME", "STATE", "CPU%", "MEM(MB)", "NI", "PRI");
    for (int i = 0; i < n; i++) {
        printf("%-7d %-20.20s %-5c %7.1f %9.1f %4d %4d\n",
               (int)list[i].pid, list[i].name, list[i].state,
               list[i].cpu_percent, list[i].rss_kb / 1024.0,
               list[i].nice, list[i].priority);
    }
}
