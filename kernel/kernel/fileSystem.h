#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

typedef struct
{
    char name[32];
    unsigned int size;
    unsigned char *data;
} FileNode;

const FileNode* find_file(const char* fileName);

void init_fileSystem();

#endif
