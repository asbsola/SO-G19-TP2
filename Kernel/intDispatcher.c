#include <timer.h>
#include <drivers/keyboardDriver.h>
#include <syscall.h>
#include <stdint.h>

static void (*interrupts[])(const registers64_t *) = {timer_handler, keyboard_handler};

void irqDispatcher(uint64_t irq, const registers64_t *registers) {
    if (irq >= sizeof(interrupts) / sizeof(interrupts[0]))
        return;

    interrupts[irq](registers);
}

uint64_t softIntDispatcher(const registers64_t *registers){
    return syscall_handler(registers);
}
