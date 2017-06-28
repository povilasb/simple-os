#include "processScheduler.h"
#include "../stdlib/stdlib.h"
#include "../stdlib/queue.h"
#include "../stdlib/stdio.h"
#include "fileSystem.h"
#include "../pmode/paging.h"
#include "../pmode/descriptorTables.h"
#include "../stdlib/stack.h"
#include "kernelHeap.h"

Queue allProcesses;
Queue readyProcesses;
Queue waitingProcesses;
PID runningProcess;

Stack waitingKeyboardProcesses;

PID idleProcess;
unsigned int kernelESP;

void print_pids(Queue *q)
{
    QueueElement *e;
    PCB *pcb;

    kprintf("----------Processes---------\n");
    e = q->front;
    while (e != NULL)
    {
        pcb = (PCB*) e->data;
        kprintf("%s %x\n", pcb->processName, (unsigned int)pcb);
        e = e->next;
    }
    kprintf("-----------------------------\n");
}

void print_allProcesses()
{
    print_pids(&allProcesses);
}

void init_processScheduler()
{
    init_queue(&allProcesses);
    init_queue(&readyProcesses);
    init_queue(&waitingProcesses);
    init_stack(&waitingKeyboardProcesses);

    idleProcess = create_process("IdleProcess.exe");
}

void start_processScheduler()
{
    runningProcess = (PID)queue_removeFirst(&readyProcesses);
    while (runningProcess == idleProcess) //search if there are any processes other than idle
        runningProcess = (PID)queue_removeFirst(&readyProcesses);
    if (runningProcess == NULL) //no ready processes
        runningProcess = idleProcess;

    //kprintf("sched: %s\n", runningProcess->processName);

    process_loadContext(runningProcess);
}

unsigned int load_process(unsigned int *pages, char *processName)
{
    const FileNode* program;
    int pageCount;
    int i, j;
    char *programText;
    unsigned int bytesToCopy;
    unsigned int bytesCopied = 0;

    program = find_file(processName);
    if (program == NULL)
        return 0;

    pageCount = size_inFrames(program->size);
    for (i = 0; i < pageCount; i++)
    {
        pages[i] = frame_address(frame_alloc());
        remote_mapPage(pages[i], pages[i]);
        programText = (char*) pages[i];

        bytesToCopy  = program->size - bytesCopied;
        if (bytesToCopy > FRAME_SIZE)
            bytesToCopy = FRAME_SIZE;

        for (j = 0; j < bytesToCopy; j++)
            programText[j] = program->data[i*FRAME_SIZE + j];

        bytesCopied += bytesToCopy;
        unmap_page(pages[i]);
    }

    return pageCount;
}

PID create_process(char *processName)
{
    PCB *pcb;
    int i;
    int progPageCount = 0; //pages for program text

    pcb = (PCB*)kmalloc(sizeof(PCB));
    if (pcb == NULL)
        return 0;

    strcpy(pcb->processName, processName);
    pcb->processState = PROC_STATE_NEW;
    pcb->pid = (unsigned int) pcb;
    pcb->priority = PROC_PRIORITY_USER;

    for (i = 0; i < PROC_MAX_MEMORY_PAGES; i++)
        pcb->memoryPages[i] = PROC_UNUSED_PAGE;

    progPageCount = load_process(pcb->memoryPages, processName); //load program text
    if (progPageCount == 0)
        return NULL;

    //initializing process stack
    pcb->memoryPages[PROC_MAX_MEMORY_PAGES - 2] = frame_address(frame_alloc());

    //initializing heap
    for (i = progPageCount; i < PROC_MAX_MEMORY_PAGES  - 3; i++)
        pcb->memoryPages[i] = frame_address(frame_alloc());
    init_heap(&pcb->processHeap, progPageCount * FRAME_SIZE, (i - progPageCount));

    //initializing registers
    pcb->registers.EAX = 0;
    pcb->registers.EBX = 0;
    pcb->registers.ECX = 0;
    pcb->registers.EDX = 0;
    pcb->registers.ESP = (PROC_MAX_MEMORY_PAGES - 1) * FRAME_SIZE - 4;
    pcb->registers.EBP = 0;
    pcb->registers.ESI = 0;
    pcb->registers.EDI = 0;
    pcb->registers.EFLAGS = 1 << 9; //IT (interrupt flag) set
    pcb->registers.EIP = 0;
    pcb->registers.CS = 0x8;
    pcb->registers.SS = 0x10;
    pcb->registers.DS = 0x10;
    pcb->registers.ES = 0x10;
    pcb->registers.FS = 0x10;
    pcb->registers.GS = 0x10;

    queue_add(&allProcesses, (void*)pcb->pid);
    return pcb;
}

void resume_process(PID pid)
{
    queue_add(&readyProcesses, pid);
    pid->processState = PROC_STATE_READY;
}

void process_loadContext(PID pid)
{
    int i;

    pid->processState = PROC_STATE_RUNNING;

    //memory switch
    for (i = 0; i < PROC_MAX_MEMORY_PAGES; i++)
        if (pid->memoryPages[i] != PROC_UNUSED_PAGE)
        {
            unmap_page(i * FRAME_SIZE);
            remote_mapPage(i * FRAME_SIZE, pid->memoryPages[i]);
        }

    pages_refresh();
    //kprintf("load: %s %x\n", pid->processName, pid->pid);

    kernelESP = 0x6504FE0;
    asm("push %0;"
        "pop %%esp;"
        "push %1;"
        "push %2;"
        "push %3;"
        : : "g" (pid->registers.ESP), "g" (pid->registers.EFLAGS), "g" (pid->registers.CS),
            "g" (pid->registers.EIP)
        );
    asm("push %0;"
        "push %1;"
        "push %2;"
        "push %3;"
        : :  "g" (pid->registers.DS), "g" (pid->registers.ES), "g" (pid->registers.FS),
             "g" (pid->registers.GS)
        );
    asm("push %0;"
        "push %1;"
        "push %2;"
        "push %3;"
        "push %4;"
        "push %5;"
        "push %6;"
        "push %7;"
        : : "g" (pid->registers.EAX), "g" (pid->registers.ECX), "g" (pid->registers.EDX),
            "g" (pid->registers.EBX), "g" (pid->registers.ESP), "g" (pid->registers.EBP),
            "g" (pid->registers.ESI), "g" (pid->registers.EDI)
        );

    asm("popa;"
        "pop %%gs;"
        "pop %%fs;"
        "pop %%es;"
        "pop %%ds;"
        "iret;"
        : :
        );
}

//adds 12 to ESP because when interrupt is fired 12 bytes are pushed
void process_saveContext(PID pid, IntRegisters *regs)
{
    pid->registers.EAX = regs->eax;
    pid->registers.EBX = regs->ebx;
    pid->registers.ECX = regs->ecx;
    pid->registers.EDX = regs->edx;
    pid->registers.ESI = regs->esi;
    pid->registers.EDI = regs->edi;
    pid->registers.ESP = regs->esp + 12;
    pid->registers.EBP = regs->ebp;
    pid->registers.EFLAGS = regs->eflags;
    pid->registers.EIP = regs->eip;
    pid->registers.CS = regs->cs;
}

void terminate_process(PID pid)
{
    int i;

    if (queue_removeElement(&allProcesses, (void*)pid) == FALSE) //no such process
        return;

    //freeing memory used by the process
    for (i = 0; i < PROC_MAX_MEMORY_PAGES; i++)
        if (pid->memoryPages[i] != PROC_UNUSED_PAGE)
        {
            frame_free(frame_number(pid->memoryPages[i]));
            unmap_page(i * FRAME_SIZE);
        }

    //removing PID from all process queues
    while (queue_removeElement(&allProcesses, (void*)pid->pid)){}
    while (queue_removeElement(&readyProcesses, (void*)pid->pid)){}
    while (queue_removeElement(&waitingProcesses, (void*)pid->pid)){}
    while (stack_removeElement(&waitingKeyboardProcesses, (void*)pid->pid)){}

    //freeing process PCB
    kfree(pid);
}

void terminate_runningProcess()
{
    terminate_process(runningProcess);
}

PID get_runningProcess()
{
    return runningProcess;
}

unsigned int get_kernelESP()
{
    return kernelESP;
}

void keyboard_askResource()
{
    runningProcess->processState = PROC_STATE_WAITING;

    while (queue_removeElement(&readyProcesses, (void*)runningProcess->pid)){}
    queue_add(&waitingProcesses, (void*)runningProcess->pid);
    stack_push(&waitingKeyboardProcesses, (void*)runningProcess->pid);
}

void keyboard_createResource(char *line)
{
    PID pid;
    char *p;
    int i;

    //remove process from waiting queues
    pid = (PID)stack_pop(&waitingKeyboardProcesses);
    if (pid != NULL)
    {
        while (queue_removeElement(&waitingProcesses, (void*)pid->pid)){}

        //copy input buffer to process memory, address is in EDI
        for (i = 0; i < PROC_MAX_MEMORY_PAGES; i++)
            if (pid->memoryPages[i] != PROC_UNUSED_PAGE)
                remote_mapPage(i * FRAME_SIZE, pid->memoryPages[i]);
        strcpy((char*)pid->registers.EDI, line);

        //add processes to ready queue
        queue_add(&readyProcesses, (void*)pid->pid);
    }
}
