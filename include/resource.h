#ifndef RESOURCE_H
#define RESOURCE_H

#include "common.h"

/* All functions return 0 on success, -1 on error. */
int resource_set_rlimit(pid_t pid, int resource, long soft, long hard);
int resource_cgroup_create(const char *name, int cpu_percent,
                           int mem_mb, pid_t pid);
int resource_cgroup_remove(const char *name);

#endif
