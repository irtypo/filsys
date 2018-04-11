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


int make_sfs(char *disk_name){
	int fd;
	char SUPER_DATA[BLOCK_SIZE] = "super block stuff\n";
	char FAT[4096];


	create_disk(disk_name, MAX_BLOCKS*BLOCK_SIZE);			// create disk
	fd = open_disk(disk_name);								// open disk
	printf("Disk opened. fd: %d\n", fd);

	write_block(fd, 0, SUPER_DATA);					// write super block to disk
	write_block(fd, 1, FAT);							// write FAT to disk



	return 0;
} 

