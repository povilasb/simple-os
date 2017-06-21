#include "paging.h"
#include "../stdlib/stdio.h"
#include "../stdlib/stdlib.h"
#include "../kernel/kernelHeap.h"

/*
    max user memory is 1 MB because virtual addr 0x100000 is where
    page directory and page tables start
*/

unsigned char frames[FRAMES_COUNT];
PageDirectory *pageDirectory;

//visus puslapius nustatyti i not present
void init_paging()
{
    int i, j;
    PageTable *pageTable;

    for (i = 0; i < FRAMES_COUNT; i++)
        frames[i] = 0; //unused

    //frames for page directory and page tables are set as in use
    frame_setUsage(PAGE_DIRECTORY_START, 1);
    for (i = 0; i < PAGE_TABLE_COUNT; i++)
        frame_setUsage(PAGE_TABLES_START + i, 1);

    pageDirectory = (PageDirectory*) frame_address(PAGE_DIRECTORY_START);
    //all pages are set as not present
    for (i = 0; i < 1024; i++)
    {
        set_pageTableEntry(&pageDirectory->entry[i], 0, 0, 0, 0); //not present
        pageTable = (PageTable*) frame_address(PAGE_TABLES_START + i);
        //for (j = 0; j < 1024; j++)
            //set_pageTableEntry(&pageTable->entry[j], 0, 0, 0, 0); //not present
    }

    set_pageDirectory(pageDirectory);

    //map kernel source code where virtual addr = physical addr
    for (i = 0; i < KERNEL_SOURCE_SIZE; i++)
    {
        map_page(pageDirectory, KERNEL_START_ADDR + i*FRAME_SIZE, KERNEL_START_ADDR + i*FRAME_SIZE);
    }

    //map kernel stack where virtual addr = physical addr
    for (i = 0; i < 4; i++)
        map_page(pageDirectory, KERNEL_STACK_START_ADDR + i*FRAME_SIZE, KERNEL_STACK_START_ADDR + i*FRAME_SIZE);
    map_page(pageDirectory, VIDEO_MEM_START, 0xB8000); //mapping virtual video memory

    //mapping kernel heap where virtual addr = physical addr
    for (i = 0; i < KERNEL_HEAP_SIZE; i++)
        map_page(pageDirectory, KERNEL_HEAP_START_ADDR + i*FRAME_SIZE, KERNEL_HEAP_START_ADDR + i*FRAME_SIZE);

    //map page directory and page tables
    map_page(pageDirectory, frame_address(PAGE_DIRECTORY_START), frame_address(PAGE_DIRECTORY_START));
    for (i = 0; i < PAGE_TABLE_COUNT; i++)
        map_page(pageDirectory, frame_address(PAGE_TABLES_START + i), frame_address(PAGE_TABLES_START + i));

    paging_enable();
}

//returns number of a new free frame
unsigned int frame_alloc()
{
    int i, j;
    unsigned char temp; //bit number in byte
    unsigned int frameNr;

    for (i = 0; i < FRAMES_COUNT; i++)
        if (frames[i] != 0xFF)
        {
            temp = frames[i];
            for (j = 0; j < 8; j++)
            {
                if ((temp & 1) == 0)
                {
                    frameNr = i * 8 + j;
                    frame_setUsage(frameNr, 1);
                    return frameNr;
                }
                temp >>= 1;
            }
        }
}

//sets frame usage bit to 0
void frame_free(unsigned int frameNr)
{
    frame_setUsage(frameNr, 0);
}

void frame_setUsage(unsigned int frameNr, int usage)
{
    unsigned int byteNr; //byte number in frames buffer
    unsigned int bitNr;
    unsigned char mask;

    byteNr = frameNr / 8;
    bitNr = frameNr % 8;
    mask = 1 << bitNr;
    if (usage == 1)
        frames[byteNr] = frames[byteNr] | mask;
    if (usage == 0)
        frames[byteNr] = frames[byteNr] & ~mask;
}

unsigned int frame_address(unsigned int frameNr)
{
    return FRAMES_START_ADDR + frameNr * FRAME_SIZE;
}

unsigned int frame_number(unsigned int frameAddress)
{
    return (frameAddress - FRAMES_START_ADDR) / FRAME_SIZE;
}

unsigned int size_inFrames(unsigned int size)
{
    unsigned int frameCount;

    frameCount = size / FRAME_SIZE;
    if (size % FRAME_SIZE > 0)
        frameCount++;
    return frameCount;
}

void set_pageTableEntry(PageTableEntry *tableEntry, unsigned int frameAddress, unsigned int present, unsigned int rw, unsigned int userMode)
{
    tableEntry->frameAddress = frameAddress;
    tableEntry->present = present;
    tableEntry->rw = rw;
    tableEntry->userMode = userMode;

    tableEntry->dirty = 0;
    tableEntry->reserved1 = 0;
    tableEntry->reserved2 = 0;
    tableEntry->accessed = 0;
    tableEntry->unused = 0;
}

void paging_enable()
{
    asm volatile ("mov %%cr0, %%eax;"
        "or $0x80000000, %%eax;"
        "mov %%eax, %%cr0"
        : : : "eax"
       );

    set_videoMemorySegment(0x20);
}

void paging_disable()
{
    asm volatile ("mov %%cr0, %%eax;"
        "and $0x7FFFFFFF, %%eax;"
        "mov %%eax, %%cr0"
        : : : "eax"
       );
}

void set_pageDirectory(PageDirectory *pageDirectory)
{
    asm volatile ("mov %0, %%eax;"
        "mov %%eax, %%cr3"
        : : "r" (pageDirectory)
        : "eax"
        );
}

//maps virtual address to physical address
//virtualAddr should be 0x1000 aligned
void map_page(PageDirectory *pageDir, unsigned int virtualAddr, unsigned int physicalAddr)
{
    int pageTableNr;
    int pageNr; //page number inside page table
    PageTable *pageTable;

    pageTableNr = virtualAddr >> 22;
    pageNr = (virtualAddr >> 12) & 1023;
    pageTable = (PageTable*) frame_address(PAGE_TABLES_START + pageTableNr);
    set_pageTableEntry(&pageDir->entry[pageTableNr], (int)pageTable >> 12, 1, 1, 0);
    set_pageTableEntry(&pageTable->entry[pageNr], physicalAddr >> 12, 1, 1, 0);

    frame_setUsage(frame_number(physicalAddr), 1);
}

void unmap_page(unsigned int virtualAddr)
{
    int pageTableNr;
    int pageNr; //page number inside page table
    PageTable *pageTable;

    pageTableNr = virtualAddr >> 22;
    pageNr = (virtualAddr >> 12) & 1023;

    pageTable = (PageTable*) frame_address(PAGE_TABLES_START + pageTableNr);
    set_pageTableEntry(&pageTable->entry[pageNr], 0, 0, 0, 0);
}

void remote_mapPage(unsigned int virtualAddr, unsigned int physicalAddr)
{
    map_page(pageDirectory, virtualAddr, physicalAddr);
}