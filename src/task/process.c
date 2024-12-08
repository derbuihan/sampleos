#include "process.h"

#include "../fs/file.h"
#include "../kernel.h"
#include "../memory/heap/kheap.h"
#include "../memory/memory.h"
#include "../status.h"
#include "string/string.h"

struct process* current_process = 0;

static void process_init(struct process* process) {
  memset(process, 0, sizeof(struct process));
}

static int process_load_binary(const char* filename, struct process* process) {
  int res = 0;
  int fd = fopen(filename, "r");
  if (!fd) {
    res = -EIO;
    goto out;
  }

  struct file_stat stat;
  res = fstat(fd, &stat);
  if (res != SAMPLEOS_ALL_OK) goto out;

  void* program_data_ptr = kzalloc(stat.filesize);
  if (!program_data_ptr) {
    res = -ENOMEM;
    goto out;
  }

  if (fread(program_data_ptr, stat.filesize, 1, fd) != 1) {
    res = -EIO;
    goto out;
  }

  process->ptr = program_data_ptr;
  process->size = stat.filesize;

out:
  fclose(fd);
  return res;
}

int process_load_data(const char* filename, struct process* process) {
  int res = 0;
  res = process_load_binary(filename, process);
  return res;
}

int process_map_binary(struct process* process) {
  int res = 0;
  res = paging_map_to(
      process->task->page_directory->directory_entry,
      (void*)SAMPLEOS_PROGRAM_VIRTUAL_ADDRESS, process->ptr,
      paging_align_address(process->ptr + process->size),
      PAGING_IS_PRESENT | PAGING_IS_WRITEABLE | PAGING_ACCESS_FROM_ALL);
  return res;
}

int process_map_memory(struct process* process) {
  int res = 0;
  res = process_map_binary(process);
  return res;
}

int process_load_for_slot(const char* filename, struct process** process,
                          int process_slot) {
  int res = 0;
  struct task* task = 0;
  struct process* _process;
  void* program_stack_ptr = 0;

  _process = kzalloc(sizeof(struct process));
  if (!_process) {
    res = -ENOMEM;
    goto out;
  }

  process_init(_process);
  res = process_load_data(filename, _process);
  if (res < 0) goto out;

  program_stack_ptr = kzalloc(SAMPLEOS_USER_PROGRAM_STACK_SIZE);
  if (!program_stack_ptr) {
    res = -ENOMEM;
    goto out;
  }

  strncpy(_process->filename, filename, sizeof(_process->filename));
  _process->stack = program_stack_ptr;
  _process->id = process_slot;

  task = task_new(_process);
  if (ERROR_I(task) == 0) {
    res = ERROR_I(task);
  }

  _process->task = task;
  res = process_map_memory(_process);
  if (res < 0) goto out;
  *process = _process;
out:
  if (ISERR(res)) {
    if (_process && _process->ptr) {
      kfree(_process->ptr);
    }
  }
  return res;
}

int process_load(const char* filename, struct process** process) {
  int res = 0;
  res = process_load_for_slot(filename, process, 0);
out:
  return res;
}
