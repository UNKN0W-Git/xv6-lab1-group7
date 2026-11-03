#ifndef XV6_SYSINFO_H
#define XV6_SYSINFO_H

#include "types.h"
#include "param.h"

// One open file object (one occupied slot of ftable.file[]).
struct sysinfo_file {
  int file_index;       // Index in ftable.file[]: 0 .. NFILE-1.
  int type;             // FD_PIPE, FD_INODE, FD_DEVICE, etc.
  int readable;         // 1 if readable, otherwise 0.
  int writable;         // 1 if writable, otherwise 0.
  int ref;              // Reference count of this file object.
  int process_count;    // Number of distinct referencing PIDs.
  int pids[NPROC];      // PIDs, each included once.
};

// One open descriptor in a process.
struct sysinfo_fd {
  int fd;               // Local file descriptor: 0 .. NOFILE-1.
  int file_index;       // Index of its file object in ftable.file[].
};

// One process whose state is not UNUSED.
struct sysinfo_process {
  int pid;
  int state;           // enum procstate value.
  int parent_pid;      // 0 when there is no parent.
  uint64 scheduling_count; // Number of selections by scheduler().
  int open_file_count; // Number of occupied descriptor slots.
  struct sysinfo_fd open_files[NOFILE];
};

// Snapshot returned by sysinfo(struct sysinfo *).
struct sysinfo {
  int open_file_count;  // Number of file objects with ref > 0.
  struct sysinfo_file files[NFILE];
  int process_count;    // Number of processes with state != UNUSED.
  struct sysinfo_process processes[NPROC];
};

#endif // XV6_SYSINFO_H
