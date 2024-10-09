#include <drivers/pitDriver.h>
#include <drivers/keyboardDriver.h>
#include <interruptHandlers/syscall.h>
#include <stdint.h>

extern schedulerADT the_scheduler;

static void (*interrupts[])(const registers64_t *) = {keyboard_handler};

uint64_t irqDispatcher(uint64_t irq, const registers64_t *registers) {
    if (irq > sizeof(interrupts) / sizeof(interrupts[0]))
        return (uint64_t)registers;

    if(irq == 0)
        return timer_handler(the_scheduler, registers);

    interrupts[irq - 1](registers);
    return (uint64_t)registers;
}

uint64_t softIntDispatcher(const registers64_t *registers){
    return syscall_handler(registers);
}
