#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <drivers/videoDriver.h>
#include <drivers/pitDriver.h>
#include <shell_caller.h>

#include <managers/memoryManager.h>

#define MEMORY_MANAGER_MEM_SIZE 10000
memoryManagerADT the_memory_manager = NULL;

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

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
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
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
	initialize_pit(60);
	load_idt();

    char managed_memory[MEMORY_MANAGER_MEM_SIZE];
    the_memory_manager = init_memory_manager(managed_memory, MEMORY_MANAGER_MEM_SIZE);

    set_font_size(1);
    clear_video_text_buffer();
    write_to_video_text_buffer("GRUPO 21\n", 9, 0x006fb5fb);
    write_to_video_text_buffer("Kernel initialized\nRunning user code...\n\n", 41, HEX_GRAY);

    start_shell();

    write_to_video_text_buffer("Back in kernel...\n", 18, HEX_GRAY);

	delay(1000);

	return 0;
}
