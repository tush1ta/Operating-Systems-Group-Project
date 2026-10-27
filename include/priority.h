#ifndef PRIORITY_H
#define PRIORITY_H

#include "common.h"

/* All functions return 0 on success, -1 on error. */
int priority_set_nice(pid_t pid, int nice_value);
int priority_get_nice(pid_t pid, int *nice_value);
int priority_set_policy(pid_t pid, int policy, int rt_priority);
int priority_set_affinity(pid_t pid, int cpu);

#endif
