// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <drivers/videoDriver.h>
#include <drivers/pitDriver.h>
#include <interruptHandlers/interrupts.h>
#include <def.h>

#include <shell_caller.h>

#include <managers/memoryManager.h>
#include <managers/processManager.h>
#include <managers/scheduler.h>
#include <managers/semaphoreManager.h>


static char managed_memory[MEMORY_MANAGER_MEM_SIZE];

memoryManagerADT the_memory_manager = NULL;
processManagerADT the_process_manager = NULL;
schedulerADT the_scheduler = NULL;
semaphoreManagerADT the_semaphore_manager = NULL;

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static const uint64_t StackPageCount = 16; // Stack Size = PageSize * StackPageCount

typedef int (*EntryPoint)();

void load_idt();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * StackPageCount
		- sizeof(uint64_t)		
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		(void*)SHELL_CODE_ADDRESS,
		(void*)SHELL_DATA_ADDRESS
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{	
    _cli();
	initialize_pit(60);
	load_idt();

    the_memory_manager = init_memory_manager(managed_memory, MEMORY_MANAGER_MEM_SIZE);
	the_scheduler = init_scheduler(the_memory_manager);
	the_process_manager = init_process_manager(the_memory_manager, the_scheduler);
	the_semaphore_manager = init_semaphore_manager(the_memory_manager, the_process_manager, the_scheduler);

    _sti();

    write_to_video_text_buffer("Back in kernel...\n", 18, HEX_GRAY);

	delay(2000);

	return 0;
}
