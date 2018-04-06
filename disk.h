 
#ifndef DISK_H_
#define DISK_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


#define BLOCK_SIZE 4096
#define MAX_BLOCKS 4096
#define MAX_DATA_BLOCKS 3500
#define MAX_FILES 64
#define MAX_OPEN_FILES 64
#define FNAME_LENGTH 16

int fdList[BLOCK_SIZE];

#endif
