#include "disk.h"

// function headers
int create_disk(char* filename, size_t nbytes);
int open_disk(char* filename);
int read_block(int disk, int block_num, char *buf);
int write_block(int disk, int block_num, char *buf);
int close_disk(int disk);


int count = 0;

int create_disk(char* filename, size_t nbytes){

  int fd;	// file descriptor
  int result;   // result of lseek, offset location

  fd = open(filename, O_RDWR | O_CREAT | O_EXCL, (mode_t)0777);
  if (fd == -1) {
	printf("Error creating disk.\n");
        return -1;
    } // end if
  
  result = lseek(fd, nbytes - 1, SEEK_SET); //  stretch file to file size

  // null terminate, one char at end of file
  result = write(fd, "\0", 1);
    if (result < 0) {
	printf("Error 2 creating disk.\n");
	close(fd);
        return 1;
    }
  
  
  result = lseek(fd, 0, SEEK_SET); //  reposition file pointer
  
  // add file descriptor
  fdList[count] = fd;
  count++;


  printf("file desc is: %d\n", fd);


  close(fd);

  return 0;
} // end create_disk

int open_disk(char* filename){
  
  int fd = open(filename, O_RDWR);
  if(fd == -1){
    printf("Error opening disk.\n");
  }

  return fd;

} // end open_disk
