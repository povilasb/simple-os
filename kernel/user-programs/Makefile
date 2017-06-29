CXXFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-fno-pic -std=c++14 -fno-rtti -fno-exceptions -Wall -Wextra
LDFLAGS = --Ttext 0x0 --oformat binary -m elf_i386
LD = ld

programs = shell IdleProcess prog1 error input
header_files = $(programs:=.bin.h)
bin_files = $(programs:=.bin)

all: $(header_files)

.PHONY: clean
clean:
	$(RM) $(header_files) $(bin_files)

%.bin.h: %.bin
	xxd -i $< $@

%.bin: %.o systemFunc.o ../stdlib/stdlib.o
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@
