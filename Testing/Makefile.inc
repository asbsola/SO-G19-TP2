ASM=nasm
GCC=x86_64-linux-gnu-gcc
LD=x86_64-linux-gnu-ld
AR=x86_64-linux-gnu-ar

GCCFLAGS= -m64 -fno-exceptions -fPIE -fno-asynchronous-unwind-tables -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -mno-red-zone -Wall -ffreestanding -fno-common -std=c99

