#ifndef _STDLIB_H
#define _STDLIB_H

#include "stdarg.h"

#define NULL 0
#define TRUE 1
#define FALSE 0

int memcpy(char *dst, const char* src, int n);
void memset(char *dst, unsigned char value, unsigned int n);
int memcmp(const char* p1, const char* p2, unsigned int size);

void int_toStr(char *str, int num, int base);
void int_toStrDec(char *str, unsigned int num);
void int_toStrHex(char *str, unsigned int num);
void int_toStrBinary(char *str, unsigned int num);
unsigned int str_toIntHex(char *str);
void va_stringf(char *strDest, const char* strFormat, va_list list);

int strlen(const char* str);
int strcmp(const char* p1, const char* p2);
void strcpy(char *dst, const char* src);
const char *strstr(const char* str1, const char* str2);
const char* strchr(const char *str, char c);

#endif
