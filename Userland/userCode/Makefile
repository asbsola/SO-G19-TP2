include ../Makefile.inc

MODULE=0000-userCode.bin
SOURCES=$(wildcard [^_]*.c)
OBJECTS=$(SOURCES:.c=.o)

SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -T userCode.ld _loader.c $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -T userCode.ld -no-pie -Wl,--oformat=elf64-x86-64 _loader.c $(OBJECTS) $(OBJECTS_ASM) -o ../0000-userCode.elf

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I./../../include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o asm/*.o

.PHONY: all clean print
