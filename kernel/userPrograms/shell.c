#include "systemFunc.h"
#include "../kernel/syscalls.h"
#include "../stdlib/stdlib.h"

void print_help();

int start()
{
    unsigned int timer = 0;
    char *line;
    unsigned int temp;

    uprintf("----------System shell-----------\n");
    line = umalloc(256);

    while (1)
    {
        ureadln(line);
        uprintf("Parsing message...\n");

        if ((unsigned int)strstr(line, "help") == (unsigned int)line)
            print_help();

        if ((unsigned int)strstr(line, "exec") == (unsigned int)line)
            uprintf("pid: 0x%x\n", uexec((char*)(line + strlen("exec") + 1)));

        if ((unsigned int)strstr(line, "kill") == (unsigned int)line)
        {
            temp = str_toIntHex(line + strlen("kill") + 1);
            if (temp != 0)
                ukill(temp);
            uprintf("process killed: 0x%x\n", temp);
        }

        if ((unsigned int)strstr(line, "all") == (unsigned int)line)
            print_processes();
    }
    uexit();
}

void print_help()
{
    uprintf("--------------------------------------------\n");
    uprintf("Galimos komandos:\n");
    uprintf("help - parodo si teksta\n");
    uprintf("exec fileName - paleidzia programa\n");
    uprintf("kill pid - sunaikinamas procesas\n");
    uprintf("all - spausdinamas visu procesu sarasas\n");
    uprintf("--------------------------------------------\n");
}