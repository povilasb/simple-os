#include "systemFunc.h"

void start()
{
    char *p;

    uprintf("Program to demonstrate page fault\n");
    p = (char*)0xFFFFFFFF; //not accessable address
    p[0] = 12;
    uexit();
}
