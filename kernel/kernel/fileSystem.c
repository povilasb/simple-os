#include "fileSystem.h"
#include "../stdlib/stdlib.h"

#include "../user-programs/shell.bin.h"
#include "../user-programs/IdleProcess.bin.h"
#include "../user-programs/prog1.bin.h"
#include "../user-programs/error.bin.h"
#include "../user-programs/input.bin.h"

FileNode fileList[] =
{
    { "shell.exe", sizeof(shell_bin), shell_bin },
    { "IdleProcess.exe", sizeof(IdleProcess_bin), IdleProcess_bin },
    { "prog1.exe", sizeof(prog1_bin), prog1_bin },
    { "error.exe", sizeof(error_bin), error_bin },
    { "input.exe", sizeof(input_bin), input_bin }
};

unsigned int filesCount = sizeof(fileList) / sizeof(FileNode);

void init_fileSystem()
{

}

const FileNode* find_file(const char* fileName)
{
    for (int i = 0; i < filesCount; i++)
        if (strcmp(fileList[i].name, fileName) == 0)
            return &fileList[i];
    return nullptr;
}
