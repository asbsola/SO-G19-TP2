include ../Makefile.inc

MODULE=0000-userCode.bin

SOURCE_DIRS=. tests programs
SOURCES=$(wildcard $(addsuffix /*.c, $(SOURCE_DIRS)))
SOURCES_ASM=$(wildcard asm/*.asm)

OBJECTS=$(SOURCES:.c=.o)	
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T userCode.ld $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)
	$(LD) $(LDFLAGS) -T userCode.ld -no-pie --oformat=elf64-x86-64 $(OBJECTS) $(OBJECTS_ASM) -o ../0000-userCode.elf

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I./../../include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(OBJECTS_ASM) *.elf *.o *.bin

.PHONY: all clean print
