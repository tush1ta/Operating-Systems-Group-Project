#ifndef AUTOTUNER_H
#define AUTOTUNER_H

/* Run for `seconds`, lowering priority of matching processes whose CPU
   usage is above cpu_threshold percent. mem_mb of 0 means no memory limit.
   Returns 0 on success, -1 on error. */
int autotune_run(int seconds, double cpu_threshold,
                 const char *name_filter, const char *log_path, int mem_mb);

#endif
