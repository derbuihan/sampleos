#ifndef PROCESS_H
#define PROCESS_H

#include "../config.h"
#include "task.h"

struct process {
  // The process ID
  uint16_t id;

  // The filename of the process
  char filename[SAMPLEOS_MAX_PATH];

  // The main process task
  struct task *task;

  // The memory (malloc) allocations of the process
  void *allocations[SAMPLEOS_MAX_PROGRAM_ALLOCATIONS];

  // The physical pointer to the process memory
  void *ptr;

  // The physical pointer to the stack memory
  void *stack;

  // The size of the data pointed to by ptr
  uint32_t size;
};

int process_load(const char *filename, struct process **process);
int process_load_for_slot(const char *filename, struct process **process,
                          int process_slot);

#endif  // PROCESS_H
