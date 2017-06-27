#include "fileSystem.h"
#include "../stdlib/stdio.h"
#include "../stdlib/stdlib.h"

unsigned char programShell[] =
{
    #include "../user-programs/shell.h"
};

unsigned char programIdleProcess[] =
{
    #include "../user-programs/IdleProcess.h"
};

unsigned char programProg1[] =
{
    #include "../user-programs/prog1.h"
};

unsigned char programError[] =
{
    #include "../user-programs/error.h"
};

unsigned char programInput[] =
{
    #include "../user-programs/input.h"
};

FileNode fileList[] =
{
    { "shell.exe", sizeof(programShell), programShell },
    { "IdleProcess.exe", sizeof(programIdleProcess), programIdleProcess },
    { "prog1.exe", sizeof(programProg1), programProg1 },
    { "error.exe", sizeof(programError), programError },
    { "input.exe", sizeof(programInput), programInput }
};

unsigned int filesCount = sizeof(fileList) / sizeof(FileNode);

void init_fileSystem()
{

}

FileNode *find_file(char *fileName)
{
    int i;

    for (i = 0; i < filesCount; i++)
        if (strcmp(fileList[i].name, fileName) == 0)
            return &fileList[i];

    return (FileNode*)NULL;
}
