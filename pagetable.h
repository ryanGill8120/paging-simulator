
#include <stdio.h>

typedef struct page_table {

    unsigned short int validBit;
    unsigned short int frameNumber;
    unsigned int referenceCount; 

} page_table;

page_table* initPageTable();
int processPageEntry(unsigned long, page_table*, unsigned int*, FILE*);