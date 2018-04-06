/**
* CS 4348.003 Project 4 
* Anthony Iorio ati140030
* Lucas Castro ldc140030
* 
* Simple File System: Virtual Disk header
**/

#ifndef DISK_H_
#define DISK_H_

#define BLOCK_SIZE 4096
#define MAX_BLOCKS 4096
#define MAX_DATA_BLOCKS 3500
#define MAX_FILES 64
#define MAX_OPEN_FILES 64
#define FNAME_LENGTH 32

int fdList[BLOCK_SIZE];

int create_disk(char* filename, size_t nbytes);
int open_disk(char* filename);
int read_block(int disk, int block_num, char *buf);
int write_block(int disk, int block_num, char *buf);
int close_disk(int disk);

// directory structure
typedef struct {
	char name[FNAME_LENGTH];	// name of file
	size_t size;				// max size of file?
	size_t nextBlock;			// pointer to next block
}directory;

#endif
