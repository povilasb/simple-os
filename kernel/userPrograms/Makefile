#shell.exe make file

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-pic
LDFLAGS = --Ttext 0x0 --oformat binary -m elf_i386

all: shell.c IdleProcess.c systemFunc.c
	make shell
	make IdleProcess
	make prog1
	make error
	make input

shell: shell.o systemFunc.o ../stdlib/stdlib.o
	ld $(LDFLAGS) shell.o systemFunc.o ../stdlib/stdlib.o -o shell.bin
	rm -f *.o

shell.o: shell.c systemFunc.c
	gcc $(CFLAGS) -c shell.c systemFunc.c

IdleProcess: IdleProcess.o
	ld $(LDFLAGS) IdleProcess.o -o IdleProcess.bin
	rm -f *.o

IdleProcess.o: IdleProcess.c
	gcc $(CFLAGS) -c IdleProcess.c

prog1: prog1.o systemFunc.o ../stdlib/stdlib.o
	ld $(LDFLAGS) prog1.o systemFunc.o ../stdlib/stdlib.o -o prog1.bin
	rm -f *.o

prog1.o: prog1.c systemFunc.c
	gcc $(CFLAGS) -c prog1.c systemFunc.c

error: error.o systemFunc.o ../stdlib/stdlib.o
	ld $(LDFLAGS) error.o systemFunc.o ../stdlib/stdlib.o -o error.bin
	rm -f *.o

error.o: error.c systemFunc.c
	gcc $(CFLAGS) -c error.c systemFunc.c

input: input.o systemFunc.o ../stdlib/stdlib.o
	ld $(LDFLAGS) input.o systemFunc.o ../stdlib/stdlib.o -o input.bin
	rm -f *.o

input.o: input.c systemFunc.c
	gcc $(CFLAGS) -c input.c systemFunc.c
