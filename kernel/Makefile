#main make file

src_files = main.c stdlib/stdio.c stdlib/stdlib.c pmode/descriptorTables.c \
	pmode/irq.c kernel/processScheduler.c kernel/kernelHeap.c \
	stdlib/queue.c kernel/fileSystem.c pmode/paging.c \
	kernel/resourceManager.c kernel/syscalls.c pmode/keyboard.c \
	stdlib/stack.c

obj_files = $(src_files:.c=.o)

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector

all: user_programs $(obj_files)
	ld --Ttext 0x6400000 --oformat binary $(obj_files) -o main.bin
	./makeImage.lexe
	make clean

%.o: %.c
	gcc -m32 -fno-builtin -fno-stack-protector -c $^ -o $@

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
