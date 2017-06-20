#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

typedef struct
{
    char name[32];
    unsigned int size;
    unsigned char *data; 
} FileNode;

FileNode *find_file(char *fileName);

#endif