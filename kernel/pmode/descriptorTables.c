#include "descriptorTables.h"
#include "../stdlib/stdio.h"
#include "../stdlib/stdlib.h"
#include "irq.h"
#include "../kernel/syscalls.h"
#include "../kernel/processScheduler.h"
#include "keyboard.h"

extern void isr0xUnknown();
extern void isr0x80(void);
extern void isr0x20(void);
extern void isr0x21(void);
extern void isr0x8(void); //double fault
extern void isr0xB(void); //segment not present
extern void isr0xC(void); //stack fault
extern void isr0xD(void); //general protection fault
extern void isr0xE(void); //page fault
extern void isr0xF(void); //unknown interrupt

GDTDescriptor gdtDesc;
GDTEntry GDT[5];

IDTDescriptor idtDesc;
IDTEntry IDT[256];

int timer = 0;

void init_gdt()
{
    memset((char*)&GDT[0], 0, sizeof(GDTEntry)); //0x0

    //code 0x8
    GDT[1].limitLow = 0xFFFF;
    GDT[1].baseLow = 0x0;
    GDT[1].baseMiddle = 0x0;
    GDT[1].access = 0x9A; //10011010b
    GDT[1].granularity = 0xCF; //11001111b
    GDT[1].baseHigh = 0;

    //data 0x10
    GDT[2].limitLow = 0xFFFF;
    GDT[2].baseLow = 0x0;
    GDT[2].baseMiddle = 0x0;
    GDT[2].access = 0x92; //10010010b
    GDT[2].granularity = 0xCF; //11001111b
    GDT[2].baseHigh = 0;

    //video memory 0x18
    GDT[3].limitLow = 0xFFFF;
    GDT[3].baseLow = 0x8000;
    GDT[3].baseMiddle = 0x0B;
    GDT[3].access = 0x92; //10010010b
    GDT[3].granularity = 0xCF; //11001111b
    GDT[3].baseHigh = 0;

    //virtual video memory 0x20
    GDT[4].limitLow = 0xFFFF;
    GDT[4].baseLow = 0x6000;
    GDT[4].baseMiddle = 0x50;
    GDT[4].access = 0x92; //10010010b
    GDT[4].granularity = 0xCF; //11001111b
    GDT[4].baseHigh = 0x6;

    gdtDesc.size = sizeof(GDT);
    gdtDesc.startAddress = (unsigned int) &GDT[0];
    load_gdt(&gdtDesc);
}

void load_gdt(GDTDescriptor *gdtd)
{
    asm volatile ("lgdt (%0)" : : "r"(gdtd));
}

void idt_setEntry(uint number, uint isrAddr, ushort segment, uchar present, uchar dpl, uchar storageSegment, uchar gateType)
{
    IDT[number].offsetLow = isrAddr & 0xFFFF;
    IDT[number].offsetHigh = isrAddr >> 16;
    IDT[number].selector = segment;
    IDT[number].zero = 0;
    IDT[number].typeAttr = ((present & 0x1) << 7) | ((dpl & 0x3) << 5) | ((storageSegment & 0x1) << 4) | (gateType & 0xF);
}

void init_idt()
{
    int i;
    char str[256];

    for (i = 0; i < 255; i++)
        idt_setEntry(i, 0, 0, 0, 0, 0, 0xE); //not present

    //for (i = 38; i < 40; i++)
       idt_setEntry(39, (uint)isr0xUnknown, 0x8, 1, 0, 0, 0xE);

    idt_setEntry(0x8, (unsigned int) isr0x8, 0x8, 1, 0, 0, 0xE);
    idt_setEntry(0xB, (unsigned int) isr0xB, 0x8, 1, 0, 0, 0xE);
    idt_setEntry(0xC, (unsigned int) isr0xC, 0x8, 1, 0, 0, 0xE);
    idt_setEntry(0xD, (unsigned int) isr0xD, 0x8, 1, 0, 0, 0xE);
    idt_setEntry(0xF, (unsigned int) isr0xF, 0x8, 1, 0, 0, 0xE);
    idt_setEntry(0xE, (unsigned int) isr0xE, 0x8, 1, 0, 0, 0xE);
    idt_setEntry(0x80, (unsigned int) isr0x80, 0x8, 1, 0, 0, 0xE);
    idt_setEntry(0x20, (uint)isr0x20, 0x8, 1, 0, 0, 0xE); //PIT
    idt_setEntry(0x21, (uint)isr0x21, 0x8, 1, 0, 0, 0xE); //keyboard

    idtDesc.limit = sizeof(IDT);
    idtDesc.base = (uint) &IDT;
    load_idt(&idtDesc);
}

void load_idt(IDTDescriptor *idtd)
{
    asm volatile ("lidt (%0)" : : "r"(idtd));
}

void int0xUnknown_handler(IntRegisters regs)
{
    PID runningProcess;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    runningProcess = get_runningProcess();
    process_saveContext(runningProcess, &regs);
    resume_process(runningProcess);

    kprintf("Unknown interrupt\n");
    kprintf("eax: %x, ebx: %x, ecx: %x, edx: %x\n", regs.eax, regs.ebx, regs.ecx, regs.edx);
    kprintf("esi: %x, edi: %x, esp: %x, ebp: %x\n", regs.esi, regs.edi, regs.esp, regs.ebp);
    kprintf("EFLAGS: %x, cs: %x, eip: %x\n", regs.eflags, regs.cs, regs.eip);


    start_processScheduler();
}

void int0x80_handler(IntRegisters regs)
{
    PID runningProcess;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    runningProcess = get_runningProcess();
    process_saveContext(runningProcess, &regs);

    //kprintf("%s\n", runningProcess->processName);
    //kprintf("eax: %x, ebx: %x, ecx: %x, edx: %x\n", regs.eax, regs.ebx, regs.ecx, regs.edx);
    //kprintf("esi: %x, edi: %x, esp: %x, ebp: %x\n", regs.esi, regs.edi, regs.esp, regs.ebp);
    //kprintf("EFLAGS: %x, cs: %x, eip: %x\n", regs.eflags, regs.cs, regs.eip);

    handle_systemCall(&regs);
    start_processScheduler();
}

void int0x20_handler(IntRegisters regs)
{
    PID runningProcess;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    runningProcess = get_runningProcess();
    process_saveContext(runningProcess, &regs);
    resume_process(runningProcess);

    timer++;
    outb(PIC1_PORT, 0x20); //PIC ack
    start_processScheduler();
}

void int0x21_handler(IntRegisters regs)
{
    PID runningProcess;
    unsigned char scanCode;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    runningProcess = get_runningProcess();
    process_saveContext(runningProcess, &regs);
    resume_process(runningProcess);

    scanCode = inb(0x60);
    if (scanCode < 0x80) //key pressed, if scanCode > 0x80 - key released
        key_pressed(scanCode);
    outb(PIC1_PORT, 0x20); //PIC ack
    start_processScheduler();
}

void int0x8_handler()
{
    PID pid;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    pid = get_runningProcess();
    kprintf("#----Error---- Double fault: %s pid=0x%x\n", pid->processName, pid->pid);
    terminate_process(pid);
    start_processScheduler();
}

void int0xB_handler(IntWithErrorRegisters regs)
{
    PID pid;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    pid = get_runningProcess();
    kprintf("#----Error---- Segment not present: %s pid=0x%x\n", pid->processName, pid->pid);

    terminate_process(pid);
    start_processScheduler();
}

void int0xC_handler()
{
    PID pid;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    pid = get_runningProcess();
    kprintf("#----Error---- Stack fault: %s pid=0x%x\n", pid->processName, pid->pid);
    terminate_process(pid);
    start_processScheduler();
}

void int0xD_handler()
{
    PID pid;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    pid = get_runningProcess();
    kprintf("#----Error---- General protection fault: %s pid=0x%x\n", pid->processName, pid->pid);
    terminate_process(pid);
    start_processScheduler();
}

void int0xE_handler(unsigned int faultingAddr)
{
    PID pid;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    pid = get_runningProcess();
    kprintf("#----Error---- Page fault: 0x%x : %s pid=0x%x\n", faultingAddr, pid->processName, pid->pid);
    terminate_process(pid);
    start_processScheduler();
}

void int0xF_handler()
{
    PID pid;

    asm("mov %0, %%esp" : : "r" (get_kernelESP()));
    pid = get_runningProcess();
    kprintf("#----Error---- Unknown interrupt: %s pid=0x%x\n", pid->processName, pid->pid);
    terminate_process(pid);
    start_processScheduler();
}
