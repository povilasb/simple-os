#include "irq.h"
#include "../stdlib/stdio.h"

void remap_irq()
{
    outb(PIC1_PORT, 0x11);
    outb(PIC2_PORT, 0x11);

    //new interrupt numbers
    outb(PIC1_PORT + 1, 0x20);
    outb(PIC2_PORT + 1, 0x28);

    outb(PIC1_PORT + 1, 0x04);
    outb(PIC2_PORT + 1, 0x02);

    outb(PIC1_PORT + 1, 0x01);
    outb(PIC2_PORT + 1, 0x01);

    //outb(PIC1_PORT + 1, 0xFD); //only keaboard
    outb(PIC1_PORT + 1, 0xFC); //keayboard and PIT
    outb(PIC2_PORT + 1, 0xFF);
}

void init_timer(unsigned int frequency)
{
    unsigned int divisor;
    unsigned char lByte;
    unsigned char hByte;

    divisor = 1193180 / frequency;
    lByte = divisor & 0xFF;
    hByte = (divisor >> 8) & 0xFF;

    outb(PIT_CMD_PORT, 0x36);
    outb(PIT0_PORT, lByte);
    outb(PIT0_PORT, hByte);
}