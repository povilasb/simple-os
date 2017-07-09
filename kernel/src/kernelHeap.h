#ifndef _KERNELHEAP_H
#define _KERNELHEAP_H

//heap memory element, free or in use
typedef struct structHeapElement
{
    unsigned char free;
    struct structHeapElement *next;
    struct structHeapElement *prev;
    unsigned int dataSize;
    void *data; //address where data starts
} HeapElement;

typedef struct
{
    unsigned int baseAddress;
    unsigned int freeMemAddress;
    unsigned int size;
    HeapElement *head; //first heap element
    HeapElement *tail; //last heap element
} Heap;

//basic heap functions
void init_heap(Heap *heap, unsigned int baseAddress, unsigned int sizeInFrames);
void heap_insertElement(Heap *heap, HeapElement *heapElement);
void heap_removeElement(Heap *heap);
void *heap_malloc(Heap *heap, unsigned int size);
void heap_free(Heap *heap, void *addr);

//kernel heap specific functions
void init_kernelHeap();
void *kmalloc(unsigned int size);
void kfree(void *addr);


#endif
