#include "stdlib/stdio.h"
#include "stdlib/stdlib.h"
#include "pmode/descriptorTables.h"
#include "pmode/irq.h"
#include "kernel/processScheduler.h"
#include "kernel/kernelHeap.h"
#include "pmode/paging.h"
#include "kernel/fileSystem.h"

int main()
{     
    PID pidShell;
    
    clear_screen();
    kprintf("------------Simple OS kernel------------\n");
    kprintf("Initializing...\n");
    
    init_gdt();
    kprintf("GDT: OK\n");
    
    init_idt();
    kprintf("IDT: OK\n");
    
    remap_irq(); //IRQ 0..7 - 0x20..0x27, 8..15 - 0x28-0x2F
    kprintf("IRQs remapping: OK\n");
    
    init_timer(20); //20 Hz
    kprintf("Timer (20 Hz): OK\n");
    
    init_paging();
    kprintf("Paging: OK\n");
    
    init_kernelHeap();
    kprintf("Kernel heap: OK\n");
    
    init_fileSystem();
    kprintf("Virtual file system: OK\n");
    
    init_processScheduler();
    kprintf("Process scheduler: OK\n");
    
    init_keyboard();
    kprintf("Keyboard: OK\n");
    
    pidShell = create_process("shell.exe");
    resume_process(pidShell);
    
    start_processScheduler();
    
    while (1)
    {
    }
    
    return 0;
}
