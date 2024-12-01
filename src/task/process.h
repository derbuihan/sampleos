#ifndef SAMPLEOS_SRC_TASK_PROCESS_H_
#define SAMPLEOS_SRC_TASK_PROCESS_H_

#include <stdint.h>

#include "../config.h"
#include "../task/task.h"

struct process {
  // The process id
  uint16_t id;

  char filename[SAMPLEOS_MAX_PATH];

  // The main process task
  struct task* task;

  // The memory (malloc) allocations of the process
  void* allocations[SAMPLEOS_MAX_PROGRAM_ALLOCATIONS];

  // The physical pointer to the process memory
  void* ptr;

  // The physical pointer to the stack memory
  void* stack;

  // The size of the data pointed to by "ptr"
  uint32_t size;
};

int process_load_for_slot(const char* filename, struct process** process,
                          int process_slot);

#endif  // SAMPLEOS_SRC_TASK_PROCESS_H_
