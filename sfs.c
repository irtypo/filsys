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
int numFiles = 0;			// current number of files
directory file;				// directory structure information about file
int numOpenFiles = 0;		// number of open files


int make_sfs(char *disk_name){
	char SUPER_DATA[BLOCK_SIZE] = "super block stuff";				// filler super block data
	char FAT[4096];													// File Allocation Table


	if ((create_disk(disk_name, MAX_BLOCKS*BLOCK_SIZE)) < 0)		// create disk
		return -1;

	if ((fd = open_disk(disk_name)) < 0)							// open disk
		return -1;

	for (i=0; i < BLOCK_SIZE; i++){					// fill FAT
		if(i < 2)
			FAT[i] = 1;								// structure blocks
		else
			FAT[i] = 0;								// data blocks
	}

	write_block(fd, 0, SUPER_DATA);					// write super block to disk
	write_block(fd, 1, FAT);						// write FAT to disk

	close_disk(fd);									// close disk
	return 0;
} 


int mount_sfs(char *disk_name){
	if ((fd = open_disk(disk_name)) < 0)			// open disk
		return -1;

	 read_block(fd, 0, superBlock);					// read super block from disk into local memory
	 read_block(fd, 1, FAT);						// read FAT from disk into local memory

	 close_disk(fd);								// close disk
	 return 0;
}

int unmount_sfs(char *disk_name){
	if ((fd = open_disk(disk_name)) < 0)			// open disk
		return -1;

	write_block(fd, 1, FAT);						// update FAT on disk

	close_disk(fd);									// close disk
	return 0;
}

int sfs_open(char *file_name){
	
	if (numOpenFiles > MAX_FILES)					// max open file check
		return -1;

	if ((fd = open(file_name, O_RDWR, (mode_t)0777)) < 0){		// open file
	    printf("error: %s\n", strerror(errno));
		return -1;
	}

	numOpenFiles++;									// increment number of open files
	printf("file opened. fd: %d\n", fd);

	return fd;
}

int sfs_close(int fd){
	
	close(fd);								// close file
	numOpenFiles--;							// decrement number of open files
	file.numInstances--;					// decrement number of open instances of specific file

	printf("fd %d closed.\n", fd);
	return 0;
}


int sfs_create(char *file_name){

	if (numFiles > MAX_FILES)				// max number of files check
		return -1;

	if ((fd = open(file_name, O_RDWR | O_CREAT | O_EXCL, (mode_t)0777)) < 0)		// opens file
		return -1;

	numFiles++;								// increments total number of files

	for (i=0; i < MAX_BLOCKS; i++){			// scan for free block
		if (FAT[i] == 0){
			FAT[i] = -1;					// occupy last block
			break;
		}
	}

	strcpy(file.name, file_name);			// update directury strcture information
	file.size = 0;
	file.nextBlock = i;
	file.numInstances++;
	printf("file created. fd: %d\n", fd);

	close(fd);								// close file
	return 0;
}

int sfs_delete(char *file_name){	

	if (file.numInstances > 0)				// multiple instances open
		return -1;
	else									// no instances open
		unlink(file_name);					// delete file

	if (file.nextBlock < 0)					// file only one block
		FAT[file.nextBlock] = 0;			// free block


	printf("%s deleted.\n", file_name);
	return 0;
}

int sfs_write(int fd, void *buf, size_t count){



}