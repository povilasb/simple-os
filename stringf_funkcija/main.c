#include <stdio.h>
#include "funkcija.h"

int main()
{
    char *str;

    str = stringf("skaicius: %d %s", 16, "liux");
    printf("%s", str);
    getchar();
    return 0;
}
