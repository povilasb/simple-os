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
