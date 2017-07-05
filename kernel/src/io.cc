#include "io.hh"

namespace sos
{
namespace io
{

// TODO: use templates for all output forms: bytes, words, double words.
void out8(short port, char byte)
{
    asm("mov %1, %%al;"
        "mov %0, %%dx;"
        "out %%al, %%dx"
        : : "r" (port), "r" (byte)
        : "eax", "edx");
}

void out32(short port, int word)
{
    asm("mov %1, %%eax;"
        "mov %0, %%dx;"
        "out %%eax, %%dx"
        : : "r" (port), "r" (word)
        : "eax", "edx");
}

char in8(short port)
{
    char res;
    asm("mov %1, %%dx;"
        "in %%dx, %%al;"
        "mov %%al, %0"
        : "=r" (res)
        : "r" (port)
        : "eax", "edx");
    return res;
}

int in32(short port)
{
    int res;
    asm("mov %1, %%dx;"
        "in %%dx, %%eax;"
        "mov %%eax, %0"
        : "=r" (res)
        : "r" (port)
        : "eax", "edx");
    return res;
}

}
}
