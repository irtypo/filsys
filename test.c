/**
* CS 4348.003 Project 4 
* Anthony Iorio ati140030
* Lucas Castro ldc140030
* 
* Simple File System: Test Driver
**/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>


#include "disk.h"

int main(int argc, char* argv[]){
	char writeBuf[BLOCK_SIZE] = "nerds with more stuff\n";
	char writeBuf22[BLOCK_SIZE] = "get these fucking nerd ass virgins out of here, dude!\n";
	char readBuf[BLOCK_SIZE];
	size_t disks[10];
	uint words = 1048576;


	create_disk("disk0", words);					// create disk


	disks[0] = open_disk("disk0");				// open disk
	printf("Disk opened. fd: %d\n", disks[0]);

	write_block(disks[0], 0, writeBuf);					// write to disk


	// ssize_t num = write(disk0, "stuff written", BLOCK_SIZE);
	// printf("wrote %d bytes\n", num);
	write_block(disks[0], 2, writeBuf22);					// write to disk

	read_block(disks[0], 0, readBuf);

	read_block(disks[0], 2, readBuf);


	if (close(disks[0]) < 0)						// close disk
		printf("Error closing disk.\n");
	else
		printf("Disk closed. fd: %d\n", disks[0]);


	// fflush(disks[0]);
	fflush(stdout);

	return 0;
}
