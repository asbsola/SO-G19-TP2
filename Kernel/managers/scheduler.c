// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <managers/scheduler.h>

struct schedulerCDT {
  memoryManagerADT memory_manager;
  processControlBlockADT current_process;
  ListADT process_list[PRIORITY_LEVELS];
  uint8_t executions_counter;
};

void free_process_list(ListADT process_list[], int size) {
  for (int i = 0; i < size; i++) {
    free_list(process_list[i]);
  }
}

schedulerADT init_scheduler(memoryManagerADT memory_manager) {
  schedulerADT scheduler =
      mem_alloc(memory_manager, sizeof(struct schedulerCDT));
  if (scheduler == NULL)
    return NULL;

  for (int i = 0; i < PRIORITY_LEVELS; i++) {

    ListADT process_list = list_init(memory_manager);
    if (process_list == NULL) {
      free_process_list(scheduler->process_list, i);
      return NULL;
    }
    scheduler->process_list[i] = process_list;
  }
  scheduler->current_process = NULL;
  scheduler->memory_manager = memory_manager;
  scheduler->executions_counter = 0;

  return scheduler;
}

pid_t get_current_pid(schedulerADT scheduler) {
  if (scheduler->current_process == NULL)
    return -1;
  return scheduler->current_process->pid;
}

int schedule_process(schedulerADT scheduler, processControlBlockADT process) {
  return list_add(scheduler->process_list[process->priority], process);
}

int deschedule_process(schedulerADT scheduler, processControlBlockADT process) {
  return list_remove(scheduler->process_list[process->priority], process);
}

int denominator(schedulerADT scheduler) {
  return 4 * list_size(scheduler->process_list[HIGH]) +
         2 * list_size(scheduler->process_list[MEDIUM]) +
         list_size(scheduler->process_list[LOW]);
}

int get_probability_low(schedulerADT scheduler) {
  if (denominator(scheduler) == 0)
    return 0;
  return list_size(scheduler->process_list[LOW]) * 10000 /
         denominator(scheduler);
}

int get_probability_medium(schedulerADT scheduler) {
  if (denominator(scheduler) == 0)
    return 0;
  return list_size(scheduler->process_list[MEDIUM]) * 20000 /
         denominator(scheduler);
}
int get_probability_high(schedulerADT scheduler) {
  if (denominator(scheduler) == 0)
    return 0;
  return list_size(scheduler->process_list[HIGH]) * 40000 /
         denominator(scheduler);
}

int get_next_priority(schedulerADT scheduler) {

  uint32_t random = GetUniform(10000);
  if (random < get_probability_high(scheduler) &&
      !list_is_empty(scheduler->process_list[HIGH]))
    return HIGH;
  if (random <
          get_probability_high(scheduler) + get_probability_medium(scheduler) &&
      !list_is_empty(scheduler->process_list[MEDIUM]))
    return MEDIUM;
  return LOW;
}

processControlBlockADT next_process(schedulerADT scheduler) {
  return list_next(scheduler->process_list[get_next_priority(scheduler)]);
}

void handle_status(schedulerADT scheduler, processControlBlockADT process) {
  switch (process->status) {
  case RUNNING:
    process->status = READY;
    break;
  case BLOCKED:
  case EXITED:
  case KILLED:
    list_remove(scheduler->process_list[process->priority], process);
  default:
    break;
  }
}

uint64_t context_switch(schedulerADT scheduler, uint64_t rsp) {
  if (scheduler->current_process == NULL) {
    scheduler->current_process = next_process(scheduler);
    return scheduler->current_process->rsp;
  }

  scheduler->current_process->rsp = rsp;
  handle_status(scheduler, scheduler->current_process);

  scheduler->current_process = next_process(scheduler);
  scheduler->current_process->status = RUNNING;
  return scheduler->current_process->rsp;
}

int change_process_priority(schedulerADT scheduler,
                            processControlBlockADT process,
                            processPriority old_priority,
                            processPriority new_priority) {
  deschedule_process(scheduler, process);
  process->priority = new_priority;
  schedule_process(scheduler, process);
  return 0;
}