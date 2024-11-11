#ifndef __IDTLOADER__H__
#define __IDTLOADER__H__

#include <defs.h>
#include <interruptHandlers/interrupts.h>
#include <stdint.h>

void setup_IDT_entry(int index, uint64_t offset);
void load_idt();

#endif
