#include "systemFunc.h"
#include "../stdlib/stdarg.h"
#include "../stdlib/stdlib.h"
#include "../kernel/syscalls.h"

void uprintf(char *str, ...)
{
    char formatedStr[2048];
    va_list list;
     
    va_start(list, str);
    va_stringf(formatedStr, str, list);
    asm("mov %0, %%esi;"
        "mov %1, %%eax;"
        "int $0x80;"
        : : "r" (formatedStr), "r" (SYSCALL_PRINT)
        : "esi", "eax"
        );
}

void ureadln(char *dest)
{
    asm("mov %0, %%edi;"
        "mov %1, %%eax;"
        "int $0x80;"
        : : "r" (dest), "r" (SYSCALL_READLN)
        : "eax", "edi"
        );
}

void uexit()
{
    asm("mov %0, %%eax;"
        "int $0x80;"
        : : "r" (SYSCALL_EXIT)
        : "eax"
        );
}

void *umalloc(unsigned int size)
{
    unsigned int res;
    
    asm("mov %1, %%eax;"
        "mov %2, %%ebx;"
        "int $0x80;"
        "mov %%eax, %0;"
        : "=r" (res) 
        : "r" (SYSCALL_MALLOC), "r" (size)
        : "eax", "ebx"
        );
        
    return (void*) res;
}

void ufree(void *addr)
{
    asm("mov %0, %%eax;"
        "mov %1, %%ebx;"
        "int $0x80"
        : : "r" (SYSCALL_FREE), "r" (addr)
        : "eax", "ebx"
        );
}

unsigned int uexec(char *program)
{
    unsigned int res;
    
    asm("mov %1, %%eax;"
        "mov %2, %%ebx;"
        "int $0x80;"
        "mov %%eax, %0;"
        : "=r" (res) 
        : "r" (SYSCALL_EXEC_PROGRAM), "r" ((unsigned int) program)
        : "eax", "ebx"
        );
        
    return res;
}

void ukill(unsigned int pid)
{
    asm("mov %0, %%eax;"
        "mov %1, %%ebx;"
        "int $0x80"
        : : "r" (SYSCALL_TERMINATE_PROCESS), "r" (pid)
        : "eax", "ebx"
        );
}

void print_processes()
{
    asm("mov %0, %%eax;"
        "int $0x80"
        : : "r" (SYSCALL_PRINT_PROCESSES)
        : "eax"
        );
}