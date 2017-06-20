#ifndef _PROCESSSCHEDULER_H
#define _PROCESSSCHEDULER_H

#include "../stdlib/queue.h"
#include "../pmode/descriptorTables.h"
#include "kernelHeap.h"

#define PROC_STATE_NEW 1
#define PROC_STATE_RUNNING 2
#define PROC_STATE_WAITING 3
#define PROC_STATE_READY 4

#define PROC_PRIORITY_SYSTEM 0
#define PROC_PRIORITY_USER 1

#define PROC_UNUSED_PAGE 0xFFFFFFFF

#define PROC_MAX_MEMORY_PAGES 20

typedef struct
{
    unsigned int EAX, EBX, ECX, EDX, ESP, EBP, ESI, EDI; //general registers
    unsigned int EFLAGS;
    unsigned int EIP;
    unsigned int CS, SS, ES, DS, FS, GS; //segment registers
} __attribute__((packed)) RegisterList;

typedef struct
{
    unsigned char processName[32];
    unsigned char processState;
    unsigned int pid;
    unsigned char priority;
    RegisterList registers;
    unsigned int memoryPages[PROC_MAX_MEMORY_PAGES]; //addresses of process memory pages
    Heap processHeap; //user process heap
}  __attribute__((packed)) PCB;

typedef PCB* PID;

void print_allProcesses();

void init_processScheduler();
void start_processScheduler();
unsigned int load_process(unsigned int *pages, char *processName);
PID create_process(char *processName);
void resume_process(PID pid);
void process_loadContext(PID pid);
void process_saveContext(PID pid, IntRegisters *regs);
void terminate_process(PID pid);
void terminate_runningProcess();
PID get_runningProcess();
unsigned int get_kernelESP();

void keyboard_askResource();
void keyboard_createResource(char *line);

#endif