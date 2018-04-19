/**
* CS 4348.003 Project 4 
* Anthony Iorio ati140030
* Lucas Castro ldc140030
* 
* Simple File System: Simple File System
**/


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#include "disk.h"

int make_sfs(char *disk_name);
int mount_sfs(char *disk_name);
int unmount_sfs(char *disk_name);

int sfs_open(char *file_name);
int sfs_close(int fd);
int sfs_create(char *file_name);
int sfs_delete(char *file_name);

int sfs_read(int fd, void *buf, size_t count);
int sfs_write(int fd, void *buf, size_t count);
int sfs_seek(int fd, int offset);

int fd;
int i = 0;
directory dirEntries[MAX_FILES];	// array of directory entries
int dirIndex = 0;
int curBlock;
int nextBlock;
char *fdTable[MAX_OPEN_FILES];

// create and open disk
int make_sfs(char *disk_name){
	strcpy(superBlock, "super block data\0");

	if ((create_disk(disk_name, MAX_BLOCKS*BLOCK_SIZE)) < 0)		// create disk
		return -1;

	if ((fd = open_disk(disk_name)) < 0)							// open disk
		return -1;

	for (i=0; i < BLOCK_SIZE; i++){					// fill FAT
		switch(i){
			case 0:
				FAT[i] = -1;
			 	break;
			 case 1:
			 	FAT[i] = 2;
			 	break;
			 case 2:
			 	FAT[i] = 3;
			 	break;
			 case 3:
			 	FAT[i] = 4;
			 	break;
			 case 4:
			 	FAT[i] = -1;
			 	break;
			 default:
			 	FAT[i] = 0;
			 	break;
		}
	}

	write_block(fd, 0, superBlock);					// write super block to disk
	write_block(fd, 1, (char*)FAT);					// write FAT to disk
	// write_block(fd, 5, (char*)dirEntries);		// write directory to disk

	close_disk(fd);									// close disk
	return 0;
} 

// mount disk
int mount_sfs(char *disk_name){
	if ((fd = open_disk(disk_name)) < 0)			// open disk
		return -1;

	 read_block(fd, 0, superBlock);					// read super block from disk into local memory
	 read_block(fd, 1, (char*)FAT);					// read FAT from disk into local memory

	 printf("disk mounted. fd: %d\n", fd);
	 // close_disk(fd);								// close disk
	 return 0;
}

// unmount disk
int unmount_sfs(char *disk_name){
	if ((fd = open_disk(disk_name)) < 0)			// open disk
		return -1;

	write_block(fd, 1, (char*)FAT);					// update FAT on disk

	close_disk(fd);									// close disk
	return 0;
}

// open file
int sfs_open(char *file_name){
	char name[FNAME_LENGTH];
	if (directoryCount() > MAX_FILES)								// max open file check
		return -1;

	if ((fd = open(file_name, O_RDWR, (mode_t)0777)) < 0){		// open file
	    printf("error: %s\n", strerror(errno));
		return -1;
	}

	dirIndex = getIndexFromName(file_name);						// get correct file directory from name
	dirEntries[dirIndex].numInstances++;

	printf("%s opened. fd: %d\n", dirEntries[dirIndex].name, fd);
	fdTable[fd] = file_name;

	return fd;
}


// close file
int sfs_close(int fd){



	if (close(fd) < 0)
		return -1;

	dirIndex = getIndexFromName(fdTable[fd]);			// get correct file directory from name
	dirEntries[dirIndex].numInstances--;			// decrement number of open instances of specific file



	printf("fd %d closed.\n", fd);
	return 0;
}

// create file
int sfs_create(char *file_name){

	dirIndex = getFreeDirectory();										// get correct file directory from name

	if (dirIndex > MAX_FILES)													// max number of files check
		return -1;

	if ((fd = open(file_name, O_RDWR | O_CREAT | O_EXCL, (mode_t)0777)) < 0)	// opens file
		return -1;

	strcpy(dirEntries[dirIndex].name, file_name);								// update directory strcture information
	dirEntries[dirIndex].size = 0;
	dirEntries[dirIndex].nextBlock = 0;
	printf("%s created. fd: %d\n", dirEntries[dirIndex].name, fd);
	// dirIndex++;																	// increment number of files in dir

	// printf("dirIndex: %d\n", dirIndex);
	close(fd);																	// close file
	return 0;
}


// delete file
int sfs_delete(char *file_name){	
	
	if(access(file_name, F_OK) < 0){									// existence check
    	printf("Delete on %s failed. File does not exist.\n", file_name);
    	return -1;
	}

	dirIndex = getIndexFromName(file_name);

	// printf("dirIndex: %d\n", dirIndex);
	// printf("instances of %s: %d\n", dirEntries[dirIndex].name, dirEntries[dirIndex].numInstances);

	if (dirEntries[dirIndex].numInstances > 0){							// multiple instances open
		printf("Delete on %s failed. Open instances.\n", file_name);
		return -1;
	} else{																// no instances open
		unlink(file_name);												// delete file
	

		curBlock = dirEntries[dirIndex].nextBlock; 
		// printf("curblock: %d\n", curBlock);
		while (FAT[curBlock] > 0){
			nextBlock = FAT[curBlock];
			FAT[curBlock] = 0;												// free block
		}	// i dont think this works.

		if (FAT[curBlock] == -1)
			FAT[curBlock] = 0;
		
		// dirEntries[dirIndex]name = NULL;

		strcpy(dirEntries[dirIndex].name, "");					// clean dir array
		dirEntries[dirIndex].size = 0;
		dirEntries[dirIndex].nextBlock = 0;
		dirEntries[dirIndex].numInstances = 0;
		

		// dirIndex--;
		// if (dirEntries[dirIndex].nextBlock != 0)							// file only one block

		printf("%s deleted.\n", file_name);
	}



	return 0;
}

// write to a file
int sfs_write(int fd, void *buf, size_t count){
	size_t bytesToWrite = count;

	// curBlock = getFreeBlock();
	if ((curBlock = getFreeBlock()) == -1337)							// disk full
		return 0;

	if (count <= BLOCK_SIZE){
		if ((write_block(fd, curBlock, (char*)buf)) < 0)
			return -1;
	}else{
		while (bytesToWrite > 0){
			if ((write_block(fd, curBlock, (char*)buf)) < 0)
				return -1;
			
			bytesToWrite -= BLOCK_SIZE;
			FAT[curBlock] = -1;

			if (bytesToWrite > 0){
				// nextBlock = getFreeBlock();
				if ((nextBlock = getFreeBlock()) == -1337)
					return 0;
				else 
					FAT[curBlock] = nextBlock;
			}

			lseek(fd, BLOCK_SIZE, SEEK_SET);							// seek to next available block
			curBlock = nextBlock;
		}
		// printf("curBlock: %d\n", curBlock);
	}
	FAT[curBlock] = -1;

	return (int)(count - bytesToWrite);
}

// read file
int sfs_read(int fd, void *buf, size_t count){
	size_t bytesToRead = count;


	curBlock = 0;
	// if ((curBlock = getFreeBlock()) == -1337)							// disk full
	// 	return 0;


	// if (count <= BLOCK_SIZE){
	// 	if ((read_block(fd, curBlock, (char*)buf)) < 0)
	// 		return -1;
	// }else{
	// 	while (bytesToRead > 0){
	// 		if ((read_block(fd, curBlock, (char*)buf)) < 0)
	// 			return -1;
			
	// 		bytesToRead -= BLOCK_SIZE;
	// 		curBlock = FAT[curBlock] = -1;

	// 		if (bytesToRead > 0){
	// 			// nextBlock = getFreeBlock();
	// 			if ((nextBlock = getFreeBlock()) == -1337)
	// 				return 0;
	// 			else 
	// 				FAT[curBlock] = nextBlock;
	// 		}

	// 		lseek(fd, BLOCK_SIZE, SEEK_SET);							// seek to next available block
	// 		curBlock = nextBlock;
	// 	}
	// 	// printf("curBlock: %d\n", curBlock);
	// }
	// FAT[curBlock] = -1;

	return (int)(count - bytesToRead);
}


int sfs_seek(int fd, int offset){
	return lseek(fd, offset, SEEK_SET);		// stretch file to file size
}













int getFreeBlock(){
	int i;
	for (i=596; i < MAX_BLOCKS; i++){									// find first free block
		if (FAT[i] == 0)
			return i;
	}
	return -1337;														// disk full
}


void getFAT(){
	for (i=0; i < MAX_BLOCKS; i++)										// find first free block
		printf("FAT[%d]: %d\n", i, FAT[i]);
}

int getIndexFromName(char *name){
	for (i = 0; i < MAX_FILES; i++)
		if (strcmp(dirEntries[i].name, name) == 0)
			return i;
}

void printDirectory(){
	int i;
	for (i = 0; i < MAX_FILES; i++)
		if (strcmp(dirEntries[i].name, "") != 0)
			printf("dir[%d].name: %s\n", i, dirEntries[i].name);
}

int directoryCount(){
	int count = 0;
	for (i = 0; i < MAX_FILES; i++)
		if (strcmp(dirEntries[i].name, "") != 0)
			count++;

	return count;
}

int getFreeDirectory(){
	for (i = 0; i < MAX_FILES; i++)
		if (strcmp(dirEntries[i].name, "") == 0)
			return i;
}

