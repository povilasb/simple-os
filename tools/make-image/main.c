#include <stdio.h>

int append_bin(char *fname, FILE *fout)
{
    FILE *fin;
    int bytesWritten = 0;
    int symb;

    fin = fopen(fname, "rb");

    symb = fgetc(fin);
    while (!feof(fin))
    {
        fputc(symb, fout);
        bytesWritten++;
        symb = fgetc(fin);
    }
    fclose(fin);

    return bytesWritten;
}

int main()
{
    FILE *fout;
    int bytesWritten = 0;
    int i;
    int floppySize = 1474560;

    fout = fopen("../bootloader/boot.img", "wb");
    bytesWritten += append_bin("../bootloader/boot.bin", fout); //bootloader
    bytesWritten += append_bin("main.bin", fout); //kernel

    for (i = bytesWritten; i < floppySize; i++)
        fputc(0, fout);
    fclose(fout);
    printf("Floppy image ready!\n");
    return 0;
}
