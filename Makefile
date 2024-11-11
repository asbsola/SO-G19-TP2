all: toolchain bootloader kernel userland image 
buddy: toolchain bootloader kernelbuddy userland imagebuddy

buddytest: 
	cd Testing; make buddy

bitmaptest:
	cd Testing; make bitmap

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

kernelbuddy:
	cd Kernel; make all MEMORY_MANAGER=buddy

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

imagebuddy: kernelbuddy bootloader userland
	cd Image; make all

toolchain:
	cd Toolchain; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Testing; make clean
	cd Userland; make clean
	cd Toolchain; make clean

.PHONY: bootloader image collections kernel kernel-buddy userland all clean
