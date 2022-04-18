
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "pagetable.h"
#include "phypages.h"

int main(int argc, char ** argv) {

	char *fileName;
	int fd_in, pageFaultNum = 0;
	struct stat st;
	unsigned long filesize;
	unsigned long * memAccesses;
	int i; 
	unsigned int referenceCount = 0;

	page_table* pageTable =  initPageTable();

	FILE* outPtr;

	if(argc != 2) {
		fprintf(stderr, "Usage: %s SequenceFile\n", argv[0]);
		exit(0); 
	}
	fileName = argv[1];
	stat(fileName, &st);
	filesize = st.st_size;
	//allocate space for all addresses
	memAccesses = (unsigned long*) malloc( (size_t) filesize );
	//use open and read
	fd_in = open(fileName, O_RDONLY);
	if(fd_in == -1) {
		fprintf(stderr, "fd is invalid, with error %d\n", strerror(errno));
		exit(-1);
	}
	//read all bytes from file at once without check errors !!!
	read(fd_in, memAccesses, (int) filesize);
	close(fd_in);

	outPtr = fopen("part2-output", "wb");

	// Traverse all address
	for(i = 0; i < filesize/(sizeof (unsigned long)); i++) {

		pageFaultNum += processPageEntry(memAccesses[i], pageTable, &referenceCount, outPtr);
		

	}
	printf("Part 2 page faults: %d\n", pageFaultNum);
	close(outPtr);
	free (memAccesses); //free dynamically allocated memory
}