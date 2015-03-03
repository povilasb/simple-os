#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fin, *fout;
    char fnameRes[64];
    char *extPos;
    unsigned char symb;
    int fpos;
    
    strcpy(fnameRes, argv[1]);
    extPos = strchr(fnameRes, '.');
    strcpy(extPos, ".h");

    fin = fopen(argv[1], "rb");
    fout = fopen(fnameRes, "wb");
    
    fpos = 1;
    symb = fgetc(fin);
    while (!feof(fin))
    {
        fprintf(fout, "0x%.2X, ", (unsigned char)symb);
        if (fpos % 10 == 0)
            fprintf(fout, "\n");
        symb = fgetc(fin);
        fpos++;
    }
    
    fclose(fout);
    fclose(fin);
    return 0;
}