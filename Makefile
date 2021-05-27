# Automatically generate lists of sources
C_SOURCES = $(wildcard kernel/*.c kernel/arch/i386/*.c)
HEADERS = $(wildcard kernel/*.h kernel/arch/i386/*.h)

OBJ = ${C_SOURCES:.c=.o}

# Cross-compiler location
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions

# Default build target
all: os-image

# Run qemu with our OS
run: all
	qemu-system-i386 -fda os-image

# The disk image created by combining the bootsector and kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

# Build the kernel binary
kernel.bin: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

# Generic rule for compiling C code
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

# Assemble kernel_entry
%.o: %.asm
	nasm $< -f elf -o $@

# Assemble the bootsector
%.bin: %.asm
	nasm $< -f bin -o $@

# Disassemble our kernel - might be useful for debugging .
kernel.dis : kernel.bin
	ndisasm -b 32 $< > $@

clean:
	rm -rf *.bin *.dis *.o os-image
	rm -rf kernel/*.o boot/*.bin
