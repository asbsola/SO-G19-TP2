#include <timer.h>
#include <drivers/keyboardDriver.h>
#include <syscall.h>
#include <stdint.h>

static void (*interrupts[])(void) = {timer_handler, keyboard_handler};

void irqDispatcher(uint64_t irq) {
    if (irq >= sizeof(interrupts) / sizeof(interrupts[0]))
        return;

	interrupts[irq]();
}

uint64_t softIntDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return syscall_handler(rax, rdi, rsi, rdx, r10, r8, r9);
}
