# Automatically generate lists of sources
C_SOURCES = $(wildcard kernel/*.c)
HEADERS = $(wildcard kernel/*.h)

OBJ = ${C_SOURCES:.c=.o}

# Default build target
all: os-image

# Run qemu with our OS
run: all
	qemu-system-i386 -hda os-image

# The disk image created by combining the bootsector and kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

# Build the kernel binary
kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 -m elf_i386 $^ --oformat binary

# Generic rule for compiling C code
%.o: %.c ${HEADERS}
	gcc -ffreestanding -m32 -fno-pie -c $< -o $@

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
