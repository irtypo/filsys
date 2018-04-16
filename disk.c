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


int cur_pos;		// current pointer position


int create_disk(char* filename, size_t nbytes){

	int fd;											// file descriptor
	int result;										// result of lseek, offset location
	size_t cur_pos = 0;
	directory dir;

	fd = open(filename, O_RDWR | O_CREAT | O_EXCL, (mode_t)0777);
	if (fd < 0) {
		printf("Error creating disk. Disk exists.\n");
        return -1;
    }
  
	cur_pos = lseek(fd, nbytes - 1, SEEK_SET);		// stretch file to file size

	if ((write(fd, "\0", 1)) < 0) {					// stretch file
		printf("Error stretching disk.\n");
		close(fd);
        return -1;
    }
  
  	cur_pos = lseek(fd, 0, SEEK_SET);				// reposition file pointer
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

	if ((cur_pos = lseek(disk, (block_num * BLOCK_SIZE), SEEK_SET) < 0)){		// seeks to correct block
		printf("Failed read seek.\n");
		return -1;
	}

	if (read(disk, buf, BLOCK_SIZE) != BLOCK_SIZE){								// reads one block into buffer
		printf("Read error.\n");
		return -1;
	} else
		printf("read block: %s\n", buf);

	return 0;
}


int write_block(int disk, int block_num, char *buf){
	ssize_t written;															// number of bytes written
	int i;

	if ((cur_pos = lseek(disk, (block_num * BLOCK_SIZE), SEEK_SET) < 0)){		// get tp correct block
		printf("Failed read seek.\n");
		return -1;
	}

	// always false. not working atm
	written = write(disk, buf, (BLOCK_SIZE));
	// printf("wrtiten: %d\n", written);jh
	if (written > BLOCK_SIZE){					// buffer larger than block size
		printf("larger than 1 block");
		// scan FAT for free block.
		for (i=0; i < MAX_BLOCKS; i++){												// find first free block
			if (FAT[i] == 0){
				cur_pos = lseek(disk, (BLOCK_SIZE * i), SEEK_SET);					// seek to next available block
				written = write(disk, buf + written, BLOCK_SIZE);					// write block
			}
		}

		printf("Write error.\n");
		return -1;
	}

	// printf("wrote: %d\n", written);

	return 0;
}


int close_disk(int disk){
	// remove(disk);
	// printf("closing fd:%d\n", disk);
	return close(disk);
}


