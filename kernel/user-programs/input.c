#include "systemFunc.h"
#include "../kernel/syscalls.h"
#include "../stdlib/stdlib.h"

void start()
{
    uprintf("Program to demonstrate input\n");
    auto line = static_cast<char*>(umalloc(256));
    uprintf("Hex number: ");
    ureadln(line);
    uprintf("Dec number: %d\n", str_toIntHex(line));
    uexit();
}
