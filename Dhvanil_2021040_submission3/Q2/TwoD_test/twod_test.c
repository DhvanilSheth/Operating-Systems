#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

#define KERNEL_2D_MEMCPY 451

int main()
{
  int rows = 2;
  int cols = 3;
  float user[2][3]={{1.1,2.2,3.3},{4.4,5.5,6.6}};
  float dest[2][3]={{0,0,0},{0,0,0}};
  printf("User array before copy\n");
  for(int i=0;i<rows;i++){
      for(int j=0;j<cols;j++){
          printf("%.2f ",user[i][j]);
      }
      printf("\n");
  }
  printf("Destination array before syscall\n");
  for(int i=0;i<rows;i++){
      for(int j=0;j<cols;j++){
          printf("%.2f ",dest[i][j]);
      }
      printf("\n");
  }
  //make the system call to copy the array
  long res = syscall(KERNEL_2D_MEMCPY,*user,*dest,rows,cols);
  //print the destination array after syscall
  printf("Destination array after syscall\n");
  for(int i=0;i<rows;i++){
      for(int j=0;j<cols;j++){
          printf("%.2f ", dest[i][j]);
      }
      printf("\n");
  }

  printf("System call returned %ld\n", res);
  return res;
}
