#include <timer.h>
#include <keyboard.h>
#include <syscall.h>
#include <stdint.h>

static void int_20();
static void int_21();
static void int_80();

void (*interrupts[])() = {int_20, int_21};

void irqDispatcher(uint64_t irq) {
	if(irq == 0x80){
		int_80();
		return;
	}
	interrupts[irq]();
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
	keyboard_handler();
}

void int_80(){
	syscall_handler();
}