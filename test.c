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
#include "sfs.h"

int main(int argc, char* argv[]){
	int fd[20];
	char wordsToWrite[BLOCK_SIZE];
	char wordsToRead[BLOCK_SIZE];
	strcpy(wordsToWrite, "words and stuff");
	int written;

	make_sfs("disk0");
	mount_sfs("disk0");

	// sfs_create("file0");
	// fd[0] = sfs_open("file0");
	// written = sfs_write(fd[0], wordsToWrite, BLOCK_SIZE*5);

	// sfs_create("file1");
	// fd[1] = sfs_open("file1");
	// written = sfs_write(fd[1], wordsToWrite, BLOCK_SIZE*2);


	// printfdTable();
	// printFAT();
	// printDirectory();

	// sfs_close(fd[0]);
	// sfs_delete("file0");

	// sfs_create("file2");
	// fd[2] = sfs_open("file2");
	// written = sfs_write(fd[2], wordsToWrite, BLOCK_SIZE*10);



	// printfdTable();
	// printFAT();
	// printDirectory();

	// sfs_close(fd[1]);
	// sfs_close(fd[2]);

	// sfs_delete("file1");
	// sfs_delete("file2");


	// printfdTable();
	// printFAT();
	// printDirectory();

	unmount_sfs("disk0");

	// printfdTable();
	// printFAT();
	// printDirectory();

	return 0;
}
