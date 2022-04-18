
#include <stdlib.h>
#include "pagetable.h"
#include "phypages.h"

#define MAX_PAGE_TABLE_SIZE 32
#define OFFSET_SIZE 7
#define BYTES_PER_PAGE 128

//initializes the page table with an array of structs and sets all valid bits to 0
page_table* initPageTable(){
    
    int i;
    page_table* pageTablePtr = (page_table*) malloc(MAX_PAGE_TABLE_SIZE * sizeof(page_table));

    for (i = 0; i < MAX_PAGE_TABLE_SIZE; i++){
        pageTablePtr[i].validBit = 0;
    }
    return pageTablePtr;
}

int processPageEntry(unsigned long input, page_table* pageTable, unsigned int* referenceCount, FILE* outFile){

    unsigned long pageNumber, offset, output, physicalFrame;
    //int pageFaults = 0;
    int i, frameNumber, pageFault = 0, stolenPage, stolenFrame, min = 10000000;
    pageNumber = input >> OFFSET_SIZE;                //isolate page number
    offset = input & (BYTES_PER_PAGE - 1);            //drop all values >= 2^7

    if (pageTable[(int) pageNumber].validBit == 1){

        //page hit, get frame number and set reference counter
        physicalFrame = ((unsigned long) pageTable[pageNumber].frameNumber) << OFFSET_SIZE;
        pageTable[pageNumber].referenceCount = (*referenceCount)++;

    }else{
        //page fault
        pageFault = 1;   //function increments var in caller to output page faults


        if ((frameNumber = getFreeFrames()) > 0){
            //during cold start

            //set valid bit and get frameNumber as next freeframe
            pageTable[pageNumber].validBit = 1;
            pageTable[pageNumber].frameNumber = frameNumber;
            pageTable[pageNumber].referenceCount = (*referenceCount)++;
            physicalFrame = ((unsigned long) frameNumber) << OFFSET_SIZE;
            removeFreeFrame(); //mark another free frame as used
        }else{
            // use LRU
            pageTable[pageNumber].validBit = 1;

            //traverse to find minimum reference (LRU) and index/position of the stolen frame
            for (i = 0; i < MAX_PAGE_TABLE_SIZE; i++){
                if (pageTable[i].validBit == 1 && pageTable[i].referenceCount < min){
                    min = pageTable[i].referenceCount;
                    stolenPage = i;
                }
            }
            stolenFrame = min;
            pageTable[stolenPage].validBit = 0;
            pageTable[pageNumber].frameNumber = stolenFrame;
            pageTable[pageNumber].referenceCount = (*referenceCount)++;
            physicalFrame = ((unsigned long) stolenFrame) << OFFSET_SIZE;
        }
    }

    output = physicalFrame | offset;
    writePhysicalFrame(outFile, output);
    //write/print output
    
    return pageFault;

}

