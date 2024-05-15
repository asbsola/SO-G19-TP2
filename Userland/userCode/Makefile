include ../Makefile.inc

MODULE=0000-userCode.bin
SOURCES=$(wildcard [^_]*.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T userCode.ld _loader.c $(SOURCES) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print
