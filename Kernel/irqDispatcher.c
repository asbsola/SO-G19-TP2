#include <timer.h>
#include <keyboard.h>
#include <stdint.h>

static void int_20();
static void int_21();

void (*interrupts[])() = {int_20, int_21};

void irqDispatcher(uint64_t irq) {
	interrupts[irq]();
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
	keyboard_handler();
}