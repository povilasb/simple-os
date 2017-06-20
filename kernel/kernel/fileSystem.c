#include "fileSystem.h"
#include "../stdlib/stdio.h"
#include "../stdlib/stdlib.h"

unsigned char programShell[] =
{
    #include "../userPrograms/shell.h"
};

unsigned char programIdleProcess[] =
{
    #include "../userPrograms/IdleProcess.h"
};

unsigned char programProg1[] =
{
    #include "../userPrograms/prog1.h"
};

unsigned char programError[] =
{
    #include "../userPrograms/error.h"
};

unsigned char programInput[] =
{
    #include "../userPrograms/input.h"
};

FileNode fileList[] =
{
    {
        .name = "shell.exe",
        .data = programShell,
        .size = sizeof(programShell)
    },
    {
        .name = "IdleProcess.exe",
        .data = programIdleProcess,
        .size = sizeof(programIdleProcess)
    },
    {
        .name = "prog1.exe",
        .data = programProg1,
        .size = sizeof(programProg1)
    },
    {
        .name = "error.exe",
        .data = programError,
        .size = sizeof(programError)
    },
    {
        .name = "input.exe",
        .data = programInput,
        .size = sizeof(programInput)
    }
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
