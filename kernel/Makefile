#main make file

src_files = main.c stdlib/stdio.c stdlib/stdlib.c pmode/descriptorTables.c \
	pmode/irq.c kernel/processScheduler.c kernel/kernelHeap.c \
	stdlib/queue.c kernel/fileSystem.c pmode/paging.c \
	kernel/resourceManager.c kernel/syscalls.c pmode/keyboard.c \
	stdlib/stack.c
obj_files = $(src_files:.c=.o)

CXXFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-pic

all: user_programs $(obj_files) pmode/interrupts.o
	ld --Ttext 0x6400000 -m elf_i386 -s --oformat binary $(obj_files) pmode/interrupts.o -o main.bin
	cd ../bootloader && make
	cat ../bootloader/boot.bin main.bin /dev/zero | head -c 1474560 > sos.img

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $^ -o $@

.PHONY: user_programs
user_programs:
	make -C user-programs
	./user-programs/binToHeader.lexe user-programs/shell.bin
	./user-programs/binToHeader.lexe user-programs/IdleProcess.bin
	./user-programs/binToHeader.lexe user-programs/prog1.bin
	./user-programs/binToHeader.lexe user-programs/error.bin
	./user-programs/binToHeader.lexe user-programs/input.bin

pmode/interrupts.o: pmode/interrupts.asm
	nasm -f elf pmode/interrupts.asm

.PHONY: clean
clean:
	find -name '*.o' | xargs $(RM)

.PHONY: qemu
qemu:
	qemu-system-i386 -usb -usbdevice disk:sos.img
