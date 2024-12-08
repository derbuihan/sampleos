#include "task.h"

#include "../config.h"
#include "../kernel.h"
#include "../memory/heap/kheap.h"
#include "../memory/memory.h"
#include "../memory/paging/paging.h"
#include "../status.h"

// The current task that is running
struct task* current_task = 0;

int task_init(struct task* task, struct process* process);

struct task* task_new(struct process* process) {
  int res = 0;

  struct task* task = kzalloc(sizeof(struct task));
  if (!task) {
    res = -ENOMEM;
    goto out;
  }

  res = task_init(task, process);
  if (res != SAMPLEOS_ALL_OK) {
    goto out;
  }

out:
  if (ISERR(res)) {
    kfree(task);
    return 0;
  }
  return task;
}

struct task* task_current() { return current_task; }

int task_free(struct task* task) {
  paging_free_4gb(task->page_directory);
  kfree(task);
  return 0;
}

int task_init(struct task* task, struct process* process) {
  memset(task, 0, sizeof(struct task));

  task->page_directory =
      paging_new_4gb(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

  if (!task->page_directory) {
    return -EIO;
  }

  task->registers.ip = SAMPLEOS_PROGRAM_VIRTUAL_ADDRESS;
  task->registers.esp = SAMPLEOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START;
  task->registers.ss = USER_DATA_SEGMENT;
  task->registers.cs = USER_CODE_SEGMENT;

  task->process = process;

  return 0;
}
