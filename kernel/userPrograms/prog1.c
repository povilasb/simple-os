#include "systemFunc.h"
#include "../kernel/syscalls.h"

void start()
{
    unsigned int timer = 0;

    while (1)
    {
        timer++;
        if (timer == 500000000)
        {
            uprintf("dummy prog1\n");
            timer = 0;
        }
    }
    uexit();
}