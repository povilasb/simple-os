Compiling the Kernel


First, download the kernel source. You will also need an assembler(NASM), C compiler(DJGPP or gcc), and a linker(LD).



Now, up near the top of the linker file, you will see this line:
.text  0x100000
The hex number needs to be set to where the kernel will be loaded into memory. In this case, that is at the 1MB mark(0x100000 in hex).


Let's compile our "boiler plate" assembly code file first:

nasm -f aout kernel_start.asm -o ks.o


This compiles kernel_start.asm to ks.o in aout format. Now for our C file:

gcc -c kernel.c -o kernel.o


The next and last step is to link ks.o and kernel.o into one file. In this case, we are going to link them together into a flat binary file with the linker script link.ld. We link the two files together with this command:

ld -T link.ld -o kernel.bin ks.o kernel.o

It is important that ks.o is linked first or the kernel will not work. The kernel is called kernel.bin and is ready to be run by a bootsector/loader that sets up Protected Mode and enables the A20(John Fine's bootf02 bootsector does this). If you would like to have GRUB be able to load this kernel, you can download the GRUB version here(you compile it and link it the same way).



Conclusion


There! A basic kernel. You probably will want to write a better k_printf function, as the one used in this example is rather simple and doesn't handle things like %s, %d, %c, etc. Still, this should be enough to get you on the track towards making a better one.



This tutorial was written by Joachim Nock and K.J.

Updated September 13, 2002 by K.J.




&copy All Rights Reserved Bona Fide OS development 2001-2006. We Disclaim responsibility for all bad things, good things okay.

