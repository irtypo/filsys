#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "disk.h"

int main(int argc, char* argv[]){
  create_disk("shrek", 1024);

  printf("fd: %d\n", open_disk("shrek"));
  return 0;
 
}
