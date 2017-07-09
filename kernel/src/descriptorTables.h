#ifndef _DESCRIPTOR_TABLES_H
#define _DESCRIPTOR_TABLES_H

#include "../stdlib/stdtypes.h"

typedef struct
{
   ushort limitLow;
   ushort baseLow;
   uchar  baseMiddle;
   uchar  access;
   uchar  granularity;
   uchar  baseHigh;
} __attribute__((packed)) GDTEntry;

typedef struct
{
    unsigned short size;
    unsigned int startAddress; //address of first gdt entry
} __attribute__((packed)) GDTDescriptor;

typedef struct
{
   ushort offsetLow;
   ushort selector;
   uchar zero;
   uchar typeAttr;
   ushort offsetHigh;
} __attribute__((packed)) IDTEntry;

typedef struct
{
    ushort limit;
    uint base;
} __attribute__((packed)) IDTDescriptor;

typedef struct
{
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int eip, cs, eflags;
} IntRegisters;

typedef struct
{
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int errorCode;
    unsigned int eip, cs, eflags;
} IntWithErrorRegisters;

void init_gdt();
void load_gdt(GDTDescriptor *gdtd);

void idt_setEntry(uint number, uint isrAddr, ushort segment, uchar present, uchar dpl, uchar storageSegment, uchar gateType);
void init_idt();
void load_idt(IDTDescriptor *idtd);

//interrupt handlers
extern "C"
{

void int0xUnknown_handler(IntRegisters regs);
void int0x80_handler(IntRegisters regs);
void int0x20_handler(IntRegisters regs);
void int0x21_handler(IntRegisters regs);
void int0x8_handler();
void int0xB_handler(IntWithErrorRegisters regs);
void int0xC_handler();
void int0xD_handler();
void int0xE_handler(unsigned int faultingAddr);
void int0xF_handler();

}

#endif
