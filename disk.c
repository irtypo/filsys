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

#include "disk.h"


int cur_pos;


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
	printf("Disk created. fd: %d\n", fd);

	close(fd);
	return 0;
} // end create_disk


// opens file
int open_disk(char* filename){
	int fd = open(filename, O_RDWR);
	if(fd < 0)
		printf("Error opening disk.\n");
	
	return fd;
} // end open_disk


int read_block(int disk, int block_num, char *buf){

	if ((cur_pos = lseek(disk, (block_num * BLOCK_SIZE), SEEK_SET) < 0))
		printf("Failed read seek.\n");

	if (read(disk, buf, BLOCK_SIZE) != BLOCK_SIZE)
		printf("Read error.\n");
	else
		printf("read: %s", buf);

	return 0;
}


// writes to file
int write_block(int disk, int block_num, char *buf){
	ssize_t written;

	if ((cur_pos = lseek(disk, (block_num * BLOCK_SIZE), SEEK_SET) < 0))
		printf("Failed read seek.\n");

	if ((written = write(disk, buf, BLOCK_SIZE)) != BLOCK_SIZE)
		printf("Write error.\n");

	printf("wrote: %d\n", written);

	return 0;
}


// closes file
int close_disk(int disk){
	// remove(disk);
	printf("closing fd:%d\n", disk);
	return close(disk);
} // end close_disk


