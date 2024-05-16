#include <timer.h>
#include <keyboard.h>
#include <syscall.h>
#include <stdint.h>

static void int_20();
static void int_21();

static uint64_t int_80(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

static void (*interrupts[2])(void) = {int_20, int_21};

void irqDispatcher(uint64_t irq) {
	interrupts[irq]();
	return;
}

uint64_t softIrqDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return int_80(rax, rdi, rsi, rdx, r10, r8, r9);
}

void int_20() {
	timer_handler();
}

void int_21(){
	keyboard_handler();
}

uint64_t int_80(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return syscall_handler(rax, rdi, rsi, rdx, r10, r8, r9);
}