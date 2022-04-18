
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define OFFSET_SIZE 7
#define BYTES_PER_PAGE 128


int main(int argc, char ** argv) {

	char *fileName;
	int fd_in;
	struct stat st;
	unsigned long filesize, pageNumber, offset, physicalFrame, output;
	unsigned long * memAccesses;
	int i; 

    unsigned long pageMap[] = {2, 4, 1, 7, 3, 5, 6};

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

    outPtr = fopen("part1-output", "wb");
	// Traverse all address
	for(i = 0; i < filesize/(sizeof(unsigned long)); i++) {

	    //printf("logical address %d = %#010lx\n", i, memAccesses[i]);


        pageNumber = memAccesses[i] >> OFFSET_SIZE;                //isolate page number
        offset = memAccesses[i] & (BYTES_PER_PAGE - 1);            //drop all values >= 2^7
        physicalFrame = pageMap[(int) pageNumber] << OFFSET_SIZE;  //access the proper frame and place it in high bit position
        output = physicalFrame | offset;                           //merge the physical frame and the offset into one value

        //printf("pageNumber: %#010lx offset: %#010lx\n", pageNumber, offset);
        //printf("output = %#010lx\n", output);

        fwrite((void *) &output, sizeof(output), 1, outPtr);

	}
    close(outPtr);
	free (memAccesses); //free dynamically allocated memory
}