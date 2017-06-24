#main make file

src_files = main.c stdlib/stdio.c stdlib/stdlib.c pmode/descriptorTables.c \
	pmode/irq.c kernel/processScheduler.c kernel/kernelHeap.c \
	stdlib/queue.c kernel/fileSystem.c pmode/paging.c \
	kernel/resourceManager.c kernel/syscalls.c pmode/keyboard.c \
	stdlib/stack.c

obj_files = $(src_files:.c=.o)

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-pic

all: user_programs $(obj_files) pmode/interrupts.o
	ld --Ttext 0x6400000 -m elf_i386 -s --oformat binary $(obj_files) pmode/interrupts.o -o main.bin
	cd ../bootloader && make
	cat ../bootloader/boot.bin main.bin /dev/zero | head -c 1474560 > sos.img

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

.PHONY: user_programs
user_programs:
	./userPrograms/binToHeader.lexe userPrograms/shell.bin
	./userPrograms/binToHeader.lexe userPrograms/IdleProcess.bin
	./userPrograms/binToHeader.lexe userPrograms/prog1.bin
	./userPrograms/binToHeader.lexe userPrograms/error.bin
	./userPrograms/binToHeader.lexe userPrograms/input.bin

pmode/interrupts.o: pmode/interrupts.asm
	nasm -f elf pmode/interrupts.asm

clean:
	rm -f *.o ; \
	rm -f pmode/*.o ; \
	rm -f stdlib/*.o ; \
    rm -f kernel/*.o

.PHONY: qemu
qemu:
	qemu-system-i386 -usb -usbdevice disk:sos.img
