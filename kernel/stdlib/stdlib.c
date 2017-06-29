#include "stdlib.h"
#include "stdarg.h"
#include "../kernel/kernelHeap.h"

const char HEX_DIGITS[] = "0123456789abcdef";

int memcpy(char *dst, const char* src, int n)
{
    int i;

    for (i = 0; i < n; i++)
        dst[i] = src[i];
}

int memset(char *dst, unsigned char value, unsigned int n)
{
    for (auto i = 0u; i < n; i++)
        dst[i] = value;
}

int memcmp(const char *p1, const char* p2, unsigned int size)
{
    for (auto i = 0u; i < size; i++)
        if (p1[i] != p2[i])
        {
            return 1;
        }

    return 0;
}

void int_toStr(char *str, int num, int base)
{
    switch (base)
    {
        case 2:
            int_toStrBinary(str, num);
            break;

        case 10:
            int_toStrDec(str, num);
            break;

        case 16:
            int_toStrHex(str, num);
            break;

        default:
            int_toStrDec(str, num);
            break;
    }
}

void int_toStrDec(char *str, unsigned int num)
{
    unsigned int temp;
    int digitCount = 0;
    int i;

    if (num == 0)
    {
        digitCount = 1;
        str[0] = '0';
    }
    else
    {
        temp = num;
        while (temp > 0)
        {
            temp /= 10;
            digitCount++;
        }

        for (i = 0; i < digitCount; i++)
        {
            str[digitCount - i - 1] = num % 10 + '0';
            num /= 10;
        }
    }

    str[digitCount] = 0;
}

void int_toStrHex(char *str, unsigned int num)
{
    const char *hex = "0123456789ABCDEF";
    int digitCount = 0;
    int digit;
    unsigned int temp;
    int i;

    if (num == 0)
    {
        digitCount = 1;
        str[0] = '0';
    }
    else
    {
        temp = num;
        while (temp > 0)
        {
            temp >>= 4;
            digitCount++;
        }

        for (i = 0; i < digitCount; i++)
        {
            digit = num & 0xF;
            num >>= 4;
            str[digitCount - i - 1] = hex[digit];
        }
    }

    str[digitCount] = 0;
}

void int_toStrBinary(char *str, unsigned int num)
{
    unsigned int temp;
    int digitCount = 0;
    int i;

    temp = num;
    while (temp > 0)
    {
        temp >>= 1;
        digitCount++;
    }

    if (num == 0)
        digitCount = 8;

    if (digitCount % 8 != 0)
        digitCount += 8 - (digitCount % 8);

    for (i = 0; i < digitCount; i++)
        {
            str[digitCount - i - 1] = (num & 1) + '0';
            num >>= 1;
        }

    str[digitCount] = 0;
}

unsigned int str_toIntHex(char *str)
{
    unsigned int offset = 0;
    int i;
    unsigned int pos;
    unsigned int digit;
    unsigned int res = 0;

    if (memcmp(str, "0x", 2) == 0)
        offset += 2;

    for (i = offset; i < strlen(str); i++)
    {
        pos = (unsigned int) strchr(HEX_DIGITS, str[i]);
        if (pos == NULL)
            return 0;
        digit = pos - (unsigned int) HEX_DIGITS;
        res = (res << 4) | digit;
    }

    return res;
}

int strlen(const char* str)
{
    int i = 0;

    while (str[i] != 0)
        i++;
    return i;
}

int strcmp(const char* p1, const char* p2)
{
    return memcmp(p1, p2, strlen(p1));
}

void strcpy(char *dst, const char* src)
{
    unsigned int length;

    length = strlen(src);
    memcpy(dst, src, length);
    dst[length] = 0;
}

//Returns a pointer to the first occurrence of str2 in str1, or a null pointer if str2 is not part of str1.
const char *strstr(const char* str1, const char* str2)
{
    int i;

    for (i = 0; i < strlen(str1); i++)
        if (memcmp(str1 + i, str2, strlen(str2)) == 0)
            return str1 + i;

    return nullptr;
}

//Returns a pointer to the first occurrence of character in the C string str.
const char* strchr(const char* str, char c)
{
    int i;

    for (i = 0; i < strlen(str); i++)
        if (str[i] == c)
            return (char*) (str + i);

    return (char*) NULL;
}

void va_stringf(char *strDest, const char* strFormat, va_list list)
{
	int i;
	int size = 0;
	char tempstr[1024];
    char *pStr;
	char tempchar;
	int tempint;
	int j;

	for(i = 0; i < strlen(strFormat); i++)
	{
		if(*(strFormat+i) == '%')
		{
			i++;
			switch(*(strFormat+i))
			{
				case 's':
					pStr = va_arg(list, char*);
					for(j = 0; j < strlen(pStr); j++)
					{
						*(strDest + size) = *(pStr + j);
						size++;
					}
					break;
				case 'c':
					tempchar = va_arg(list, int);
					*(strDest + size) = tempchar;
					size++;
					break;
				case 'd':
					tempint = va_arg(list, int);
					int_toStr(tempstr, tempint, 10);
					for(j = 0; j < strlen(tempstr); j++)
                    {
                        *(strDest + size) = *(tempstr + j);
                        size++;
                    }
                    break;
				case 'x':
					tempint = va_arg(list, int);
					int_toStr(tempstr, tempint, 16);
					for(j = 0; j < strlen(tempstr); j++)
                    {
                        *(strDest + size) = *(tempstr + j);
                        size++;
                    }
                    break;
				case 'b':
					tempint = va_arg(list, int);
					int_toStr(tempstr, tempint, 2);
                    for(j = 0; j < strlen(tempstr); j++)
                        {
                            *(strDest + size) = *(tempstr + j);
                            size++;
                        }
                    break;
				default:
					*(strDest + size) = *(strFormat + i);
					size++;
					break;
			}
		}
		else
		{
			*(strDest + size) = *(strFormat + i);
			size++;
		}
	}

    strDest[size] = 0;
}
