#include "syscalls.h"
#include "../pmode/descriptorTables.h"
#include "../stdlib/stdio.h"
#include "../stdlib/stdlib.h"
#include "processScheduler.h"
#include "kernelHeap.h"

void handle_systemCall(IntRegisters *regs)
{   
    switch(regs->eax)
    {
        case SYSCALL_EXIT:
            syscall_exit();
            break;
            
        case SYSCALL_PRINT:
            syscall_print(regs);
            break;
        
        case SYSCALL_READLN:
            syscall_readln();
            break;
            
        case SYSCALL_MALLOC:
            syscall_malloc();
            break;
            
        case SYSCALL_FREE:
            syscall_free();
            break;
            
        case SYSCALL_EXEC_PROGRAM:
            syscall_execProgram();
            break;
            
        case SYSCALL_TERMINATE_PROCESS:
            syscall_terminateProcess();
            break;
            
        case SYSCALL_PRINT_PROCESSES:
            print_allProcesses();
            resume_process(get_runningProcess());
            break;
         
        default:
            kprintf("@----Kernel---- Unknown system call\n");
            break;
    }
}

void syscall_exit()
{
    terminate_runningProcess();
}

//prints string which address is in esi
void syscall_print(IntRegisters *regs)
{
    PID pid;
    
    pid = get_runningProcess();
    kprintf("%s(0x%x): %s", pid->processName, pid->pid, (char*)regs->esi);
    resume_process(get_runningProcess());
}

//reads line from console and copies it in [edi]
void syscall_readln()
{
    keyboard_askResource();
}

//allocates memory in process heap, size is in EBX, returns address in EAX
void syscall_malloc()
{
    PID pid;
    
    pid = get_runningProcess();
    pid->registers.EAX = (unsigned int) heap_malloc(&pid->processHeap, pid->registers.EBX);
    resume_process(pid);
}

//frees memory in process heap, address is in EBX
void syscall_free()
{
    PID pid;
    
    pid = get_runningProcess();
    heap_free(&pid->processHeap, (void*) pid->registers.EBX);
    resume_process(pid);    
}

//creates new process and puts it in ready queue, process name string address is in EBX, process id is returned in EAX
void syscall_execProgram()
{
    PID pid;
    
    pid = get_runningProcess();
    pid->registers.EAX = (unsigned int) create_process((char*) pid->registers.EBX);
    if (pid->registers.EAX != NULL)
        resume_process((PID)pid->registers.EAX);
    resume_process(pid); 
}

//terminates process that's pid is in EBX
void syscall_terminateProcess()
{
    PID pid;
    
    pid = get_runningProcess();
    resume_process(pid);
    terminate_process((PID)pid->registers.EBX); 
}