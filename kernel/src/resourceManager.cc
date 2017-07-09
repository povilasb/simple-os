#include "resourceManager.h"
#include "kernelHeap.h"

//allocates frames
// TODO: remove size parameter?
MemoryResource *create_memoryResource(unsigned int size)
{
    (void)size;
    return static_cast<MemoryResource*>(kmalloc(sizeof(MemoryResource)));
}
