/**
* CS 4348.003 Project 4 
* Anthony Iorio ati140030
* Lucas Castro ldc140030
* 
* Simple File System: Simple File System header
**/

#ifndef SFS_H_
#define SFS_H_

#define MAX_DISKS 3
#define FIRST_DATA_BLOCK 596


char *fdNameTable[MAX_OPEN_FILES];
int fdPointerTable[MAX_OPEN_FILES];

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

int getFreeBlock();
int getLastBlock(int fd);
int getDirIndexFromName(char *name);
int directoryCount();
int getFreeDirectory();
void printDirectory();
void printFAT();
void printfdNameTable();
void initFDPointers();
void fillFAT();

#endif
