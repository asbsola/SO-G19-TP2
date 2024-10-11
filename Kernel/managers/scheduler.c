#include <lib.h>
#include <registers.h>
#include <managers/scheduler.h>
#include <drivers/videoDriver.h>
#include <utils/list.h>
#include <def.h>
#include <utils/utils.h>

struct schedulerCDT {
    memoryManagerADT memory_manager;
    processControlBlockADT current_process;
    ListADT process_list[PRIORITY_LEVELS];
    uint8_t executions_counter;
};

void free_process_list(ListADT process_list[], int size){
    for(int i = 0; i < size; i++){
        free_list(process_list[i]);
    }
}

schedulerADT init_scheduler(memoryManagerADT memory_manager){
    schedulerADT scheduler = mem_alloc(memory_manager, sizeof(struct schedulerCDT));
    if (scheduler == NULL) return NULL;

    
    for(int i = 0; i < PRIORITY_LEVELS; i++){
        
        ListADT process_list = list_init(memory_manager);
        if(process_list == NULL) {
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



pid_t get_current_process(schedulerADT scheduler){
    if(scheduler->current_process == NULL) return -1;
    return scheduler->current_process->pid;
}

int schedule_process(schedulerADT scheduler, processControlBlockADT process){
    return list_add(scheduler->process_list[process->priority], process);
}

int deschedule_process(schedulerADT scheduler, processControlBlockADT process){
    return list_remove(scheduler->process_list[process->priority], process);
}


processControlBlockADT get_next_aux(processControlBlockADT next, schedulerADT scheduler, int priority){
    

    if(next == NULL || scheduler->current_process->priority == priority){
        if(scheduler->executions_counter <= priority ){
            
            next=(processControlBlockADT)list_next(scheduler->process_list[priority]);
        }else{
            scheduler->executions_counter=0;
        }
    }
    return next;
}

processControlBlockADT next_process(schedulerADT scheduler){

    processControlBlockADT current = scheduler->current_process;
    for(int priority = HIGH; priority >= LOW; priority--){
        current = get_next_aux(current, scheduler, priority);
        if(current != scheduler->current_process){
            scheduler->executions_counter++;
            break;
        };
    }
    
    return current;
}

void handle_status(schedulerADT scheduler, processControlBlockADT process){
    switch (process->status) {
    case RUNNING:
        process->status = READY;
        break;
    case BLOCKED:
        list_remove(scheduler->process_list[process->priority], process);
        break;
    case EXITED:
        list_remove(scheduler->process_list[process->priority], process);
        break;
    case KILLED:
        list_remove(scheduler->process_list[process->priority], process);
    default:
        break;
    }
}

uint64_t context_switch(schedulerADT scheduler, uint64_t rsp){
    if (scheduler->current_process == NULL){
        scheduler->current_process = next_process(scheduler); 
        return scheduler->current_process->rsp;
    }

    scheduler->current_process->rsp = rsp;
    handle_status(scheduler, scheduler->current_process);

    scheduler->current_process = next_process(scheduler);
    scheduler->current_process->status = RUNNING;
    return scheduler->current_process->rsp;
}


int change_process_priority(schedulerADT scheduler, processControlBlockADT process, processPriority old_priority, processPriority new_priority){
    deschedule_process(scheduler, process);
    process->priority = new_priority;
    schedule_process(scheduler, process);
    return 0;
}