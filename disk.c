/**
* CS 4348.003 Project 4 
* Anthony Iorio ati140030
* Lucas Castro ldc140030
* 
* Simple File System: Virtual Disk 
**/


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <string.h>



#include "disk.h"

int count = 0;

int create_disk(char* filename, size_t nbytes){

	int fd;	// file descriptor
	int result;   // result of lseek, offset location
	size_t cur_pos = 0;
	directory dir;

	fd = open(filename, O_RDWR | O_CREAT | O_EXCL, (mode_t)0777);
	if (fd < 0) {
	printf("Error creating disk.\n");
        return -1;
    }
  
	cur_pos = lseek(fd, nbytes - 1, SEEK_SET); //  stretch file to file size

	// null terminate, one char at end of file
	if ((write(fd, "\0", 1)) < 0) {
		printf("Error 2 creating disk.\n");
		close(fd);
        return 1;
    }
  
  	cur_pos = lseek(fd, 0, SEEK_SET); //  reposition file pointer

	// add file descriptor
	fdList[count] = fd;
	count++;


	// dir.name = filename;
	// dir.size = nbytes;
	// dir.nextBlock = cur_pos;
	printf("Disk created. fd: %d\n", fd);

	// close_disk(fd);
	close(fd);
	return 0;
} // end create_disk


// opens file
int open_disk(char* filename){
	int fd = open(filename, O_RDWR);
	if(fd < 0){
		printf("Error opening disk.\n");
	}
	return fd;
} // end open_disk


int read_block(int disk, int block_num, char *buf){
	int cur_pos;

	cur_pos = lseek(disk, (block_num * BLOCK_SIZE), SEEK_SET);

	if (read(disk, buf, BLOCK_SIZE) != BLOCK_SIZE)
		printf("read error\n");
	else
		printf("read: %s\n", buf);


	return 0;
}




// writes to file
int write_block(int disk, int block_num, char *buf){
	int cur_pos;
	ssize_t written;
	cur_pos = lseek(disk, (block_num * BLOCK_SIZE), SEEK_SET);

	printf("seeked to %d\n", cur_pos);


	written = write(disk, buf, BLOCK_SIZE);
	printf("wrote: %d\n", written);


	return 0;
}


// closes file
int close_disk(int disk){
	// remove(disk);
	printf("closing fd:%d\n", disk);
	return close(disk);
} // end close_disk


