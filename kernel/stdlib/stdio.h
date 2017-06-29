#ifndef _STDIO_H
#define _STDIO_H

#define SCREEN_LINE_COUNT 25
#define SCREEN_LINE_LENGTH 80
#define SCREEN_BYTES_PER_SYMBOL 2

void set_videoMemorySegment(unsigned short segment);

void kprint_symbol(char symb, int pos);
int kprint_str(const char* message, unsigned int line);
void kprint(const char *str);
void kprintf(const char* str, ...);

void console_scroll(int lineCount);
void clear_screen();

void outb(short port, char byte);
void outw(short port, short word);
void outd(short port, int dword);

char inb(short port);

#endif
