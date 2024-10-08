#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <drivers/videoDriver.h>
#include <drivers/pitDriver.h>
#include <shell_caller.h>
#include <interruptHandlers/interrupts.h>

#include <managers/memoryManager.h>
#include <managers/processManager.h>

#define MEMORY_MANAGER_MEM_SIZE 200000
static char managed_memory[MEMORY_MANAGER_MEM_SIZE];

memoryManagerADT the_memory_manager = NULL;
processManagerADT the_process_manager = NULL;

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static const uint64_t StackPageCount = 64; // Stack Size = PageSize * StackPageCount

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
		SHELL_CODE_ADDRESS,
		SHELL_DATA_ADDRESS
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
	the_process_manager = init_process_manager(the_memory_manager);

    _sti();
	
    set_font_size(1);
    clear_video_text_buffer();
    write_to_video_text_buffer("GRUPO 21\n", 9, 0x006fb5fb);
    write_to_video_text_buffer("Kernel initialized\nRunning user code...\n\n", 41, HEX_GRAY);

    start_shell();

    write_to_video_text_buffer("Back in kernel...\n", 18, HEX_GRAY);

	delay(1000);

	return 0;
}
