#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void int_toStrDec(char *str, int num)
{
    int temp;
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

void int_toStrHex(char *str, int num)
{
    char *hex = "0123456789ABCDEF";
    int digitCount = 0;
    int digit;
    int temp;
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

void int_toStrBinary(char *str, int num)
{
    int temp;
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

char* va_stringf(char* str, va_list list)
{
	int i;
	int size = 0;
	char* newstr = malloc(0);
	char* tempstr = malloc(256);
	char tempchar;
	int tempint;
	int j;

	for(i = 0; i < strlen(str); i++)
	{
		if(*(str+i) == '%')
		{
			i++;
			switch(*(str+i))
			{
				case 's':
					tempstr = va_arg(list, char*);
					for(j = 0; j < strlen(tempstr); j++)
					{
						newstr = (char*)realloc(newstr, (size+1)*sizeof(char));
						*(newstr + size) = *(tempstr + j);
						size++;
					}
					break;
				case 'c':
					tempchar = va_arg(list, int);
					newstr = (char*)realloc(newstr, (size+1)*sizeof(char));
					*(newstr + size) = tempchar;
					size++;
					break;
				case 'd':
					tempint = va_arg(list, int);
					int_toStr(tempstr, tempint, 10);
					for(j = 0; j < strlen(tempstr); j++)
                                        {
                                                newstr = (char*)realloc(newstr, (size+1)*sizeof(char));
                                                *(newstr + size) = *(tempstr + j);
                                                size++;
                                        }
                                        break;
				case 'x':
					tempint = va_arg(list, int);
					int_toStr(tempstr, tempint, 16);
					for(j = 0; j < strlen(tempstr); j++)
                                        {
                                                newstr = (char*)realloc(newstr, (size+1)*sizeof(char));
                                                *(newstr + size) = *(tempstr + j);
                                                size++;
                                        }
                                        break;
				case 'b':
					tempint = va_arg(list, int);
					int_toStr(tempstr, tempint, 2);
                    for(j = 0; j < strlen(tempstr); j++)
                        {
                            newstr = (char*)realloc(newstr, (size+1)*sizeof(char));
                            *(newstr + size) = *(tempstr + j);
                            size++;
                        }
                    break;
				default:
					newstr = (char*)realloc(newstr, (size+1)*sizeof(char));
					*(newstr + size) = *(str + i);
					size++;
					break;
			}
		}
		else
		{
			newstr = (char*)realloc(newstr, (size+1)*sizeof(char));
			*(newstr + size) = *(str + i);
			size++;
		}
	}

    newstr[size] = 0;
	return newstr;
}

char *stringf(char *str, ...)
{
     char *res;
     va_list list;

     va_start(list, str);
     res = va_stringf(str, list);
     return res;
}
