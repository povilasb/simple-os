#include "systemFunc.h"
#include "../stdlib/stdlib.h"

void print_help();

int start()
{
    char *line;
    unsigned int temp;

    uprintf("----------System shell-----------\n");
    line = (char*)umalloc(256);

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
    uprintf("All commands:\n");
    uprintf("help - show this text\n");
    uprintf("exec file_name - run the specified program\n");
    uprintf("kill pid - kills process\n");
    uprintf("all - list all processes\n");
    uprintf("--------------------------------------------\n");
}
