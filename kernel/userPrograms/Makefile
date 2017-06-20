#shell.exe make file

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector

all: shell.c IdleProcess.c systemFunc.c ../stdlib/stdlib.c
	make shell
	make IdleProcess
	make prog1
	make error
	make input

shell: shell.o systemFunc.o ../stdlib/stdlib.o
	ld --Ttext 0x0 --oformat binary shell.o systemFunc.o ../stdlib/stdlib.o -o shell.bin
	rm -f *.o

shell.o: shell.c systemFunc.c ../stdlib/stdlib.c
	gcc -fno-builtin -fno-stack-protector -c shell.c systemFunc.c  ../stdlib/stdlib.c

IdleProcess: IdleProcess.o
	ld --Ttext 0x0 --oformat binary IdleProcess.o -o IdleProcess.bin
	rm -f *.o

IdleProcess.o: IdleProcess.c
	gcc -fno-builtin -fno-stack-protector -c IdleProcess.c

prog1: prog1.o systemFunc.o ../stdlib/stdlib.o
	ld --Ttext 0x0 --oformat binary prog1.o systemFunc.o ../stdlib/stdlib.o -o prog1.bin
	rm -f *.o

prog1.o: prog1.c systemFunc.c ../stdlib/stdlib.c
	gcc -fno-builtin -fno-stack-protector -c prog1.c systemFunc.c ../stdlib/stdlib.c

error: error.o systemFunc.o ../stdlib/stdlib.o
	ld --Ttext 0x0 --oformat binary error.o systemFunc.o ../stdlib/stdlib.o -o error.bin
	rm -f *.o

error.o: error.c systemFunc.c ../stdlib/stdlib.c
	gcc -fno-builtin -fno-stack-protector -c error.c systemFunc.c ../stdlib/stdlib.c

input: input.o systemFunc.o ../stdlib/stdlib.o
	ld --Ttext 0x0 --oformat binary input.o systemFunc.o ../stdlib/stdlib.o -o input.bin
	rm -f *.o

input.o: input.c systemFunc.c ../stdlib/stdlib.c
	gcc -fno-builtin -fno-stack-protector -c input.c systemFunc.c ../stdlib/stdlib.c
