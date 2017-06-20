#main make file

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector

all: main.o stdlib/stdio.o stdlib/stdlib.o pmode/descriptorTables.o pmode/interrupts.o pmode/irq.o kernel/processScheduler.o kernel/kernelHeap.o stdlib/queue.o \
     kernel/fileSystem.o pmode/paging.o kernel/resourceManager.o kernel/syscalls.o pmode/keyboard.o stdlib/stack.o
	ld --Ttext 0x6400000 --oformat binary main.o stdlib/stdio.o stdlib/stdlib.o pmode/descriptorTables.o pmode/interrupts.o pmode/irq.o kernel/processScheduler.o \
    kernel/kernelHeap.o stdlib/queue.o kernel/fileSystem.o pmode/paging.o kernel/resourceManager.o kernel/syscalls.o pmode/keyboard.o stdlib/stack.o -o main.bin
	./makeImage.lexe
	make clean

main.o: main.c stdlib/stdio.c stdlib/stdlib.c pmode/descriptorTables.c pmode/irq.c kernel/processScheduler.c kernel/kernelHeap.c stdlib/queue.c kernel/fileSystem.c \
        pmode/paging.c kernel/resourceManager.c kernel/syscalls.c pmode/keyboard.c stdlib/stack.c
	./userPrograms/binToHeader.lexe userPrograms/shell.bin
	./userPrograms/binToHeader.lexe userPrograms/IdleProcess.bin
	./userPrograms/binToHeader.lexe userPrograms/prog1.bin
	./userPrograms/binToHeader.lexe userPrograms/error.bin
	./userPrograms/binToHeader.lexe userPrograms/input.bin
	gcc -fno-builtin -fno-stack-protector -c main.c stdlib/stdio.c stdlib/stdlib.c pmode/descriptorTables.c pmode/irq.c kernel/processScheduler.c \
    kernel/kernelHeap.c stdlib/queue.c kernel/fileSystem.c pmode/paging.c kernel/resourceManager.c kernel/syscalls.c pmode/keyboard.c stdlib/stack.c

pmode/interrupts.o: pmode/interrupts.asm
	nasm -f elf pmode/interrupts.asm

clean:
	rm -f *.o ; \
	rm -f pmode/*.o ; \
	rm -f stdlib/*.o ; \
    rm -f kernel/*.o