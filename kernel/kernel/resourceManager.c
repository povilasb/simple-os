#include "resourceManager.h"
#include "kernelHeap.h"

//allocates frames
MemoryResource *create_memoryResource(unsigned int size)
{
    MemoryResource *memRes;

    memRes = kmalloc(sizeof(MemoryResource));

    return memRes;
}