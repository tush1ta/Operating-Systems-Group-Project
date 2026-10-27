# Process Priority and Resource Manager Toolkit

A Linux toolkit that lists processes, changes their priority, limits their CPU and memory, and automatically lowers the priority of processes that hog the CPU.

**Course:** CSEG2060 Operating Systems, B.Tech CSE (AI & ML), Semester III
**Language:** C, with Bash and Python helper scripts
**Platform:** Linux (Ubuntu, WSL2 or a VM)

## What It Does

- **Process monitor:** shows running processes, their state and CPU usage by reading `/proc`.
- **Priority control:** changes nice value, scheduling policy and CPU affinity.
- **Resource limits:** caps CPU and memory using rlimit and cgroup v2.
- **Auto-tuner:** finds CPU hogs and lowers their priority on its own, logging every action to a CSV file.

## OS Concepts Used

Process states and the PCB, CPU scheduling (nice values, CFS, FIFO/RR/BATCH/IDLE), CPU affinity, system calls (`setpriority`, `sched_setscheduler`, `sched_setaffinity`, `prlimit`), resource limits, cgroups v2, and response time as a scheduling metric.

## Project Structure

```text
include/    shared headers
src/        toolkit source code
workloads/  test programs (cpu_hog, io_hog, mem_hog, interactive)
scripts/    experiment, demo and plotting scripts
results/    generated CSV logs and graphs
```

## Build

```bash
sudo apt install build-essential python3-matplotlib
make
```

## Usage

```bash
./bin/toolkit                                  # interactive menu
./bin/toolkit list 10                          # top 10 processes by CPU
./bin/toolkit nice 1234 10                     # set nice value of PID 1234
sudo ./bin/toolkit cgroup grp1 30 200 1234     # 30% CPU, 200 MB for PID 1234
sudo ./bin/toolkit cgroup-rm grp1              # remove the cgroup
./bin/toolkit autotune 20 20 hog results/tuner.csv [mem_mb]
```

The auto-tuner takes: `autotune <seconds> <cpu_threshold%> <name_filter> <log.csv> [mem_mb]`. It only changes processes whose name contains the filter, so system processes are never touched.

## Experiment

Four `cpu_hog` processes and one `interactive` program run on CPU 0. The interactive program measures how late it wakes up (its response time). We compare results with and without the auto-tuner.

```bash
scripts/run_experiment.sh 15 3     # 15 seconds per run, 3 runs
python3 scripts/plot.py            # creates graphs in results/
```

## Resource Limit Demo

```bash
scripts/demo_limits.sh             # rlimit demo
sudo scripts/demo_limits.sh        # rlimit + cgroup demo
```

## Team

| Member | Module |
|---|---|
| Member 1 (lead) | Process monitor, CLI menu, integration, Makefile |
| Member 2 | Priority manager and auto-tuner (CPU part) |
| Member 3 | Resource manager and auto-tuner (memory part) |
| Member 4 | Workloads, logging, experiments and graphs |

Replace "Member N" with real names and GitHub usernames.

## Future Enhancements

- Graphical dashboard
- More scheduling policies and tuner rules
- Deadlock detection demo
- Automatic report generation

## License

Developed for academic purposes as part of the CSEG2060 Operating Systems course.
