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
#include <errno.h>
#include <string.h>

#include "disk.h"
#include "sfs.h"

int create_disk(char* filename, size_t nbytes){
	int fd;											// file descriptor

	fd = open(filename, O_RDWR | O_CREAT | O_EXCL, (mode_t)0777);
	if (fd < 0) {
		printf("Error creating disk. Disk exists.\n");
        return -1;
    }
  
	lseek(fd, nbytes - 1, SEEK_SET);		// stretch file to file size

	if ((write(fd, "\0", 1)) < 0) {					// stretch file
		printf("Error stretching disk.\n");
		close(fd);
        return -1;
    }
  
  	lseek(fd, 0, SEEK_SET);				// reposition file pointer
	printf("Disk created. fd: %d\n", fd);

	close(fd);
	return 0;
}


int open_disk(char* filename){
	int fd = open(filename, O_RDWR);				// opens disk
	if(fd < 0)
		printf("Error opening disk.\n");
	
	return fd;
}


int read_block(int disk, int block_num, char *buf){

	if (lseek(disk, (block_num * BLOCK_SIZE), SEEK_SET) < 0){		// seeks to correct block
		printf("Failed read seek.\n");
		return -1;
	}

	// if (read(disk, buf, BLOCK_SIZE) != BLOCK_SIZE){								// reads one block into buffer
	if (read(disk, buf, BLOCK_SIZE) < 0){								// reads one block into buffer
	    printf("Read failure: %s\n", strerror(errno));
		return -1;
	} else
		printf("read block: %s\n", buf);

	return 0;
}


int write_block(int disk, int block_num, char *buf){
	ssize_t written;															// number of bytes written
	
	if (lseek(disk, (block_num * BLOCK_SIZE), SEEK_SET) < 0){		// get tp correct block
		printf("Failed write seek.\n");
	    // printf("error: %s\n", strerror(errno));
		return -1;
	}

	if ((written = write(disk, buf, BLOCK_SIZE)) < 0){
		printf("Write failure.\n");
		return -1;
	}
	// printf("wrote %d bytes\n", written);


	return 0;
}


int close_disk(int disk){
	// remove(disk);
	// printf("closing fd:%d\n", disk);
	return close(disk);
}


