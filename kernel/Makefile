stdlib_srcs := $(shell find stdlib -name '*.cc')
src_files := $(shell find src -name '*.cc')
src_files := $(filter-out src/main.cc,$(src_files))
all_src_files = src/main.cc $(stdlib_srcs) $(pmode_srcs) $(kernel_srcs) $(src_files)
obj_files = $(all_src_files:.cc=.o)

CXXFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-fno-pic -std=c++14 -fno-rtti -fno-exceptions -Wall -Wextra
LDFLAGS = --Ttext 0x6400000 -m elf_i386 -s --oformat binary -e kmain
LD = ld

all: user_programs $(obj_files) src/interrupts.o
	$(LD) $(LDFLAGS) $(obj_files) src/interrupts.o -o main.bin
	cd ../bootloader && make
	cat ../bootloader/boot.bin main.bin /dev/zero | head -c 1474560 > sos.img

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

.PHONY: user_programs
user_programs:
	make -C user-programs

src/interrupts.o: src/interrupts.asm
	nasm -f elf $^

.PHONY: clean
clean:
	make -C user-programs clean
	find -name '*.o' | xargs $(RM)

.PHONY: qemu
qemu:
	qemu-system-i386 -usb -usbdevice disk:sos.img
