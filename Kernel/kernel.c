// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <def.h>
#include <drivers/keyboardDriver.h>
#include <drivers/pitDriver.h>
#include <drivers/videoDriver.h>
#include <interruptHandlers/interrupts.h>
#include <lib.h>
#include <managers/memoryManager.h>
#include <managers/pipesManager.h>
#include <managers/processManager.h>
#include <managers/scheduler.h>
#include <managers/semaphoreManager.h>
#include <moduleLoader.h>
#include <stdint.h>
#include <string.h>

static char managed_memory[MEMORY_MANAGER_MEM_SIZE];

memoryManagerADT the_memory_manager = NULL;
schedulerADT the_scheduler = NULL;
processManagerADT the_process_manager = NULL;
semaphoreManagerADT the_semaphore_manager = NULL;
pipesManagerADT the_pipes_manager = NULL;

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static const uint64_t StackPageCount = 16;	// Stack Size = PageSize * StackPageCount

typedef int (*EntryPoint)();

void load_idt();

void clearBSS(void *bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
	return (void *)((uint64_t)&endOfKernel + PageSize * StackPageCount - sizeof(uint64_t));
}

void *initializeKernelBinary() {
	void *moduleAddresses[] = {(void *)SHELL_CODE_ADDRESS, (void *)SHELL_DATA_ADDRESS};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}
semaphoreManagerADT init_semaphore_manager(memoryManagerADT memory_manager, schedulerADT scheduler, processManagerADT process_manager);
int main() {
	_cli();
	initialize_pit(PIT_FREQUENCY);
	load_idt();

	the_memory_manager = init_memory_manager(managed_memory, MEMORY_MANAGER_MEM_SIZE);
	if (the_memory_manager == NULL) {
		write_to_video_text_buffer("Not enough memory\n", 18, HEX_RED);
		delay(2000);
		return -1;
	}
	the_scheduler = init_scheduler(the_memory_manager);
	if (the_scheduler == NULL) {
		write_to_video_text_buffer("Not enough memory\n", 18, HEX_RED);
		delay(2000);
		return -1;
	}
	the_process_manager = init_process_manager(the_memory_manager, the_scheduler);
	if (the_process_manager == NULL) {
		write_to_video_text_buffer("Not enough memory\n", 18, HEX_RED);
		delay(2000);
		return -1;
	}
	the_semaphore_manager = init_semaphore_manager(the_memory_manager, the_scheduler, the_process_manager);
	if (the_semaphore_manager == NULL) {
		write_to_video_text_buffer("Not enough memory\n", 18, HEX_RED);
		delay(2000);
		return -1;
	}
	the_pipes_manager = init_pipes_manager(the_memory_manager, the_semaphore_manager, the_process_manager, the_scheduler);
	if (the_pipes_manager == NULL) {
		write_to_video_text_buffer("Not enough memory\n", 18, HEX_RED);
		delay(2000);
		return -1;
	}

	if (initialize_keyboard() == -1) {
		write_to_video_text_buffer("Could not initialize keyboard\n", 18, HEX_RED);
		delay(2000);
		return -1;
	}

	_sti();

	write_to_video_text_buffer("Back in kernel...\n", 18, HEX_GRAY);
	delay(2000);
	return 0;
}
