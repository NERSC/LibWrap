#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 


int main() 
{ 
  /* Write */
  int sz; 
   
  int fd = open("foo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); 
  if (fd < 0) { perror("r1"); exit(1);} 
  
  sz = write(fd, "hello\n", strlen("hello\n")); 
  close(fd); 

  /* Read */
  char *c = (char *) calloc(100, sizeof(char)); 
  
  fd = open("foo.txt", O_RDONLY); 
  if (fd < 0) { perror("r1"); exit(1); } 
  
  sz = read(fd, c, 10); 
  c[sz] = '\0'; 
  printf("Those bytes are as follows: % s\n", c); 
  return 0;
} 
