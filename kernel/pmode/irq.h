#ifndef _IRQ_H
#define _IRQ_H

#define PIC1_PORT 0x20 //master PIC command port
#define PIC2_PORT 0xA0 //slave PIC command port

#define PIT_CMD_PORT 0x43
#define PIT0_PORT 0x40 //1 channel port

void remap_irq();
void init_timer(unsigned int frequency);

#endif