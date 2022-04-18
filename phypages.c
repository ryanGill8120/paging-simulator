
#include "phypages.h"

#define NO_FRAME_AVAILABLE -1
#define PHYSICAL_FRAME_SIZE 8 

//writePhysical (file pointer, pageNumber, offset)

int freeFrameCount = 1;
//getFreeFrame
int getFreeFrames(){
    if (freeFrameCount < PHYSICAL_FRAME_SIZE){
        return freeFrameCount;
    }   
    return NO_FRAME_AVAILABLE;
}

void removeFreeFrame(){
    freeFrameCount++;
}

void writePhysicalFrame(FILE* outFile, unsigned long physicalData){
    fwrite((void *) &physicalData, sizeof(physicalData), 1, outFile);
}