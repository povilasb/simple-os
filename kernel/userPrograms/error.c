#include "systemFunc.h"
#include "../kernel/syscalls.h"

void start()
{
    char *p;

    uprintf("Program to demonstrate page fault\n");
    p = 0xFFFFFFFF; //not accessable address
    p[0] = 12;
    uexit();
}
