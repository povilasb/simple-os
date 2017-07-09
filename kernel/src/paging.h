#ifndef _PAGING_H
#define _PAGING_H

#include "../src/kernelHeap.h"

#define FRAMES_COUNT 1024*128
#define FRAMES_START_ADDR 0x100000
#define FRAME_SIZE 4096 //4 kB

//all numbers are in frames
#define PAGE_DIRECTORY_START 0
#define PAGE_TABLES_START 1
#define PAGE_TABLE_COUNT 1024

#define USER_PAGES_START PAGE_TABLES_START + PAGE_TABLE_COUNT //frame number where user pages start
#define KERNEL_START_ADDR 0x6400000 //100 MB
#define KERNEL_SOURCE_SIZE 256 //1 MB = 256 frames
#define KERNEL_STACK_START_ADDR KERNEL_START_ADDR + 0x100000 + FRAME_SIZE //kernel + 1MB + 4kB
#define KERNEL_STACK_SIZE 4

#define VIDEO_MEM_START KERNEL_STACK_START_ADDR + (KERNEL_STACK_SIZE + 1) * FRAME_SIZE //kernel stack + kernel stack size + 4kB

#define KERNEL_HEAP_START_ADDR VIDEO_MEM_START + FRAME_SIZE //video mem + 4kB
#define KERNEL_HEAP_SIZE 1024 * 3 //1024 * 3 frames = 12 MB

typedef struct
{
    unsigned int present : 1;
    unsigned int rw : 1; //set - r/w, unset - read-only
    unsigned int userMode : 1; //set - user mode, unset - kernel mode
    unsigned int reserved1 : 2;
    unsigned int accessed : 1;
    unsigned int dirty : 1; //Set if the page has been written to (dirty)
    unsigned int reserved2 : 2;
    unsigned int unused : 3;
    unsigned int frameAddress : 20; //physical frame address

} __attribute__((packed)) PageTableEntry;

typedef struct
{
    PageTableEntry entry[1024];
} PageTable;

typedef struct
{
    PageTableEntry entry[1024];
} PageDirectory;

void paging_test();

unsigned int frame_alloc();
void frame_free(unsigned int frameNr);
void frame_setUsage(unsigned int frameNr, int usage);
unsigned int frame_address(unsigned int frameNr);
unsigned int frame_number(unsigned int frameAddress);
unsigned int size_inFrames(unsigned int size);

void init_paging();
void paging_enable();
void paging_disable();
void set_pageDirectory(PageDirectory *pageDirectory);
void set_pageTableEntry(PageTableEntry *tableEntry, unsigned int frameAddress, unsigned int present, unsigned int rw, unsigned int userMode);
void map_page(PageDirectory *pageDir, unsigned int virtualAddr, unsigned int physicalAddr);
void unmap_page(unsigned int virtualAddr);
void remote_mapPage(unsigned int virtualAddr, unsigned int physicalAddr);

/**
 * Flush page table cache.
 */
void pages_refresh();

#endif
