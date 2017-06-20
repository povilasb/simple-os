#ifndef _STDLIB_H
#define _STDLIB_H

#include "stdarg.h"

#define NULL 0
#define TRUE 1
#define FALSE 0

int memcpy(char *dst, char *src, int n);
int memset(char *dst, unsigned char value, unsigned int n);
int memcmp(char *p1, char *p2, unsigned int size);

void int_toStr(char *str, int num, int base);
void int_toStrDec(char *str, unsigned int num);
void int_toStrHex(char *str, unsigned int num);
void int_toStrBinary(char *str, unsigned int num);
void va_stringf(char *strDest, char *strFormat, va_list list);

int strlen(char *str);
int strcmp(char *p1, char *p2);
void strcpy(char *dst, char *src);
char *strstr(char *str1, char *str2);
char *strchr(char *str, char c);

#endif