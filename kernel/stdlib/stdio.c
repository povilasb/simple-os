#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"

#include "../pmode/paging.h"

int currentLine = 0; //unused
int consoleCursorPos = 0;
unsigned short videoMemorySegment = 0x18; //video memory segment selector

void set_videoMemorySegment(unsigned short segment)
{
    videoMemorySegment = segment;
}

void kprint_symbol(char symb, int pos)
{
    asm("mov %2, %%ax;"
        "mov %%ax, %%es;"
        "movb %0, %%es:(%1);"
        "movb $0x07, %%es:1(%1);"
        : : "r" (symb), "r" (pos), "r" (videoMemorySegment)
        : "eax"
        );
}

int kprint_str(char *message, unsigned int cursor)
{
	unsigned int i = 0;
    unsigned int j = 0;
    char symb;
    unsigned int line;

	i = cursor;
    line = cursor / SCREEN_BYTES_PER_SYMBOL / SCREEN_LINE_LENGTH;

	while(message[j] != 0)
	{
		if(message[j] =='\n') // check for a new line
		{
			line++;
            if (line > SCREEN_LINE_COUNT - 1)
            {
                console_scroll(1);
                line--;
            }

			i = line * 80 * 2;
			j++;
		}
        else
        {
			kprint_symbol(message[j], i);
			j++;
			i = i + 2;
		}
	}

    return i;
}

void kprint(char *str)
{
    consoleCursorPos = kprint_str(str, consoleCursorPos);
}

/*
int kprint_str2(char *message, unsigned int line)
{
	unsigned int i = 0;
    unsigned int j = 0;
    char symb;

	i = line * 80 * 2;

	while(message[j] != 0)
	{
		if(message[j] =='\n') // check for a new line
		{
			line++;
            if (line > SCREEN_LINE_COUNT - 1)
            {
                console_scroll(1);
                line--;
            }

			i = line * 80 * 2;
			j++;
		}
        else
        {
			kprint_symbol(message[j], i);
			j++;
			i = i + 2;
		}
	}

    return line;
}

void kprint2(char *str)
{
    currentLine = kprint_str(str, currentLine);
}
*/

void kprintf(char *str, ...)
{
     char formatedStr[2048];
     va_list list;

     va_start(list, str);
     va_stringf(formatedStr, str, list);
     kprint(formatedStr);
}

void console_scroll(int lineCount)
{
    int bytesToCopy;
    int spacesToFill;
    int copyStart; //offset from where to start copying
    int spacesStart;
    unsigned short a;

    bytesToCopy =  (SCREEN_LINE_COUNT - lineCount) * SCREEN_LINE_LENGTH * SCREEN_BYTES_PER_SYMBOL;
    spacesToFill = lineCount * SCREEN_LINE_LENGTH * SCREEN_BYTES_PER_SYMBOL;
    copyStart = SCREEN_LINE_LENGTH * lineCount * SCREEN_BYTES_PER_SYMBOL;
    spacesStart = (SCREEN_LINE_COUNT - lineCount) * SCREEN_LINE_LENGTH * SCREEN_BYTES_PER_SYMBOL;

    asm("mov %2, %%ax;"
        "mov %%ax, %%es;"
        "mov %0, %%ecx;"
        "mov %1, %%esi;"
        "xor %%edi, %%edi;"
        "rep movsb %%es:(%%esi), %%es:(%%edi)"
        : : "g" (bytesToCopy), "g" (copyStart), "m" (videoMemorySegment)
        : "eax", "ecx", "edi", "esi"
        );

    asm("mov $0x00, %%al;"
        "mov %0, %%ecx;"
        "mov %1, %%edi;"
        "rep stosb"
        : : "r" (spacesToFill), "r" (spacesStart)
        : "eax", "ecx", "edi"
        );
}

void clear_screen()
{
    console_scroll(SCREEN_LINE_COUNT);
    currentLine = 0;
}

void outb(short port, char byte)
{
    asm("mov %1, %%al;"
        "mov %0, %%dx;"
        "out %%al, %%dx"
        : : "r" (port), "r" (byte)
        : "eax", "edx");
}

void outw(short port, short word)
{
    asm("mov %1, %%ax;"
        "mov %0, %%dx;"
        "out %%ax, %%dx"
        : : "r" (port), "r" (word)
        : "eax", "edx");
}

void outd(short port, int dword)
{
    asm("mov %1, %%eax;"
        "mov %0, %%dx;"
        "out %%eax, %%dx"
        : : "r" (port), "r" (dword)
        : "eax", "edx");
}

char inb(short port)
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
