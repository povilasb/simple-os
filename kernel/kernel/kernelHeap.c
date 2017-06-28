#include "kernelHeap.h"
#include "../stdlib/stdlib.h"
#include "../stdlib/stdio.h"
#include "../pmode/paging.h"

Heap kernelHeap;

void init_heap(Heap *heap, unsigned int baseAddress, unsigned int sizeInFrames)
{
    heap->baseAddress = baseAddress;
    heap->freeMemAddress = heap->baseAddress;
    heap->size = sizeInFrames * FRAME_SIZE;
    heap->head = NULL;
    heap->tail = NULL;
}

void heap_insertElement(Heap *heap, HeapElement *heapElement)
{
    if (heap->head == NULL) //heap is empty
    {
        heapElement->next = NULL;
        heapElement->prev = NULL;
        heap->head = heapElement;
        heap->tail = heapElement;
    }
    else
    {
        heapElement->prev = heap->tail;
        heap->tail->next = heapElement;
        heap->tail = heapElement;
    }
}

void heap_removeElement(Heap *heap) //removes last element
{
    if (heap->tail == heap->head) //one element left
    {
        heap->head = NULL;
        heap->tail = NULL;
    }
    else
    {
        heap->tail = heap->tail->prev;
        heap->tail->next = NULL;
    }
}

void *heap_malloc(Heap *heap, unsigned int size)
{
    int temp;
    int physicalSize;
    HeapElement *heapElement;

    physicalSize = size + sizeof(HeapElement);
    if (heap->freeMemAddress + physicalSize <= heap->baseAddress + heap->size) //memory is allocated at the end of the heap
    {
        heapElement = (HeapElement*) heap->freeMemAddress;
        heapElement->free = FALSE;
        heapElement->dataSize = size;
        heapElement->data = (void*)((int)heapElement + sizeof(HeapElement));

        heap_insertElement(heap, heapElement);
        heap->freeMemAddress += physicalSize;

        return heapElement->data;
    }
    else //searching for a free memory that is big enough
    {
        heapElement = heap->head;
        while (heapElement != NULL)
        {
            if ((heapElement->free == TRUE) && (heapElement->dataSize >= size))
            {
                heapElement->free = FALSE;
                return heapElement->data;
            }
            else
                heapElement = heapElement->next;
        }
    }

    return NULL; //no free memory found
}

void heap_free(Heap *heap, void *addr)
{
    HeapElement *heapElement;

    if (((unsigned int)addr < heap->baseAddress) || ((unsigned int)addr > (heap->baseAddress + heap->size * FRAME_SIZE))) //is address in heap space?
        return;

    heapElement = (HeapElement*)((int)addr - sizeof(HeapElement));

    if (heapElement == heap->tail) //freeing the last element
    {
        heap_removeElement(heap);
        heap->freeMemAddress = (unsigned int) heapElement;
    }
    else
        heapElement->free = TRUE;
}

void init_kernelHeap()
{
    init_heap(&kernelHeap, KERNEL_HEAP_START_ADDR, KERNEL_HEAP_SIZE);
}

void *kmalloc(unsigned int size)
{
    return heap_malloc(&kernelHeap, size);
}

void kfree(void *addr)
{
    heap_free(&kernelHeap, addr);
}
