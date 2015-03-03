#ifndef _SYSCALLS_H 
#define _SYSCALLS_H

#include "../pmode/descriptorTables.h"

#define SYSCALL_EXIT 100 //exit process
#define SYSCALL_PRINT 101 //output string
#define SYSCALL_READLN 102 //read line from console
#define SYSCALL_MALLOC 103 //allocate memory in process heap
#define SYSCALL_FREE 104 //free memory in process heap
#define SYSCALL_EXEC_PROGRAM 105 //execute program
#define SYSCALL_TERMINATE_PROCESS 106 //terminate running process
#define SYSCALL_PRINT_PROCESSES 107

void handle_systemCall(IntRegisters *regs);
void syscall_exit();
void syscall_print(IntRegisters *regs);
void syscall_readln();
void syscall_malloc();
void syscall_free();
void syscall_execProgram();
void syscall_terminateProcess();

#endif