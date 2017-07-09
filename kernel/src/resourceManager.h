#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

typedef struct
{
    void *p; //memory block address
    unsigned int size; //size of memory block
} MemoryResource;


MemoryResource *create_memoryResource(unsigned int size);

#endif