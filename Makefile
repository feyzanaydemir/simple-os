C_SOURCES = $(wildcard kernel/*.c drivers/*.c lib/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h lib/*.h)
OBJ = ${C_SOURCES:.c=.o kernel/interrupts.o} 

# Change this to your cross-compiler path
GCC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
LD = /usr/local/i386elfgcc/bin/i386-elf-ld

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32

os-image.bin: boot/bootsector.bin kernel.bin
	cat $^ > os-image.bin

kernel.bin: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${GCC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o lib/*.o