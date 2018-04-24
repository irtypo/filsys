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
	int fd[16];
	char wordsToWrite[BLOCK_SIZE];
	char wordsToRead[BLOCK_SIZE];
	strcpy(wordsToWrite, "words and stuff");
	int written;

	make_sfs("disk0");
	mount_sfs("disk0");

	sfs_create("file0");
	fd[0] = sfs_open("file0");
	written = sfs_write(fd[0], wordsToWrite, BLOCK_SIZE);
	written = sfs_write(fd[0], wordsToWrite, BLOCK_SIZE);
	// printf("wn: %d\n", written);

	// sfs_create("file1");
	// fd[1] = sfs_open("file1");
	// written = sfs_write(fd[1], wordsToWrite, BLOCK_SIZE);
	// // printf("wn: %d\n", written);


	// printfdNameTable();
	// printFAT();
	// printDirectory();

	// // sfs_delete("file0");

	// sfs_create("file2");
	// fd[2] = sfs_open("file2");
	// written = sfs_write(fd[2], wordsToWrite, BLOCK_SIZE*3);
	// printf("wn: %d\n", written);
	// written = sfs_write(fd[2], wordsToWrite, BLOCK_SIZE*3);

	sfs_seek(fd[0], 0);
	sfs_read(fd[0], wordsToRead, BLOCK_SIZE);
	// printf("buf: %s\n", wordsToRead);

	printfdNameTable();
	// printFAT();
	printDirectory();

	sfs_close(fd[0]);
	// sfs_close(fd[1]);
	// sfs_close(fd[2]);

	// sfs_close(fd[1]);
	// sfs_close(fd[2]);

	sfs_delete("file0");
	// sfs_delete("file2");


	// printfdNameTable();
	// printFAT();
	// printDirectory();

	unmount_sfs("disk0");

	printfdNameTable();
	printFAT();
	printDirectory();

	return 0;
}
