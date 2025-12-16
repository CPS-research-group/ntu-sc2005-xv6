#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  char buf[6];

  pipe(p1);
  pipe(p2);

  if(fork() == 0){
    // Implement the Child code here

  } else {
    // Implement the Parent code here



  }
}
