CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-pic
LDFLAGS = --Ttext 0x0 --oformat binary -m elf_i386

all: shell.bin IdleProcess.bin prog1.bin error.bin input.bin

%.bin: %.o systemFunc.o ../stdlib/stdlib.o
	ld $(LDFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@
