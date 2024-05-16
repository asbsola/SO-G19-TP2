#include <timer.h>
#include <keyboard.h>
#include <syscall.h>
#include <stdint.h>

static void int_20();
static void int_21();

static void int_80(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

void (*interrupts[])() = {int_20, int_21};

void irqDispatcher(uint64_t irq) {
	interrupts[irq]();
	return;
}

void softIrqDispatcher(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	int_80(rax, rdi, rsi, rdx, r10, r8, r9);
}

void int_20() {
	timer_handler();
}

void int_21(){
	keyboard_handler();
}

void int_80(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	syscall_handler(rax, rdi, rsi, rdx, r10, r8, r9);
}