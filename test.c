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
	int fd[20];
	char words[BLOCK_SIZE*5];
	strcpy(words, "writing stuff.\n");
	int written;

	make_sfs("disk0");
	mount_sfs("disk0");

	sfs_create("file0");
	fd[0] = sfs_open("file0");


	written = sfs_write(fd[0], words, BLOCK_SIZE*5);

	printf("b written: %d\n", written);
	// sfs_create("file1");
	// fd[1] = sfs_open("file1");


	sfs_close(fd[0]);

	getFAT();
	// sfs_close(fd[1]);


	// sfs_delete("file0");

	return 0;
}
