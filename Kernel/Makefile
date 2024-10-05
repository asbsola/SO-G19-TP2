include Makefile.inc

KERNEL=kernel.bin

SOURCE_DIRS=. drivers interruptHandlers managers
SOURCES=$(wildcard $(addsuffix /*.c, $(SOURCE_DIRS)))
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES := $(filter-out managers/buddyMemoryManager.c managers/bitmapMemoryManager.c, $(SOURCES))

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)

MEMORY_MANAGER ?= bitmap 
ifeq ($(MEMORY_MANAGER), buddy)
    MEMORY_MANAGER_SRC = managers/buddyMemoryManager.c 
else
    MEMORY_MANAGER_SRC = managers/bitmapMemoryManager.c 
endif

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(MEMORY_MANAGER_SRC:.c=.o) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(MEMORY_MANAGER_SRC:.c=.o) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o drivers/*.o interruptHandlers/*.o managers/*.o *.o *.bin

.PHONY: all clean buddy
