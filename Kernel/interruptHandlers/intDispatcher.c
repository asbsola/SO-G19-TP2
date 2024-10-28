// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <drivers/pitDriver.h>
#include <drivers/keyboardDriver.h>
#include <interruptHandlers/syscall.h>
#include <stdint.h>

extern schedulerADT the_scheduler;
extern processManagerADT the_process_manager;

uint64_t irqDispatcher(uint64_t irq, const registers64_t *registers) {
    if(irq == 0)
        return timer_handler(the_scheduler, registers);
    if(irq == 1)
        keyboard_handler(the_process_manager, registers);
    return (uint64_t)registers;
}

uint64_t softIntDispatcher(const registers64_t *registers){
    return syscall_handler(registers);
}
