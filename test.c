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
#include <fcntl.h>
#include "disk.h"

int main(int argc, char* argv[]){
	char *buf;
	size_t disk0;


	create_disk("disk0", 1024);					// create disk


	disk0 = open_disk("disk0");				// open disk
	printf("Disk opened. fd: %d\n", disk0);

	buf = "nerds and stuff";
	write_block(disk0, 0, buf);					// write to disk


	if (close(disk0) < 0)						// close disk
		printf("Error closing disk.\n");
	else
		printf("Disk closed. fd: %d\n", disk0);



	return 0;
}
