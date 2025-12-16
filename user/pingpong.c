#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  char byte = 'x';
  if(pipe(p) < 0){
    fprintf(2, "pipe error\n");
    exit(1);
  }
  int pid = fork();
  if(pid < 0){
    fprintf(2, "fork error\n");
    exit(1);
  }
  
  if(pid == 0){
    // Child
    if(read(p[0], &byte, 1) != 1){
      fprintf(2, "child read error\n");
      exit(1);
    }
    printf("%d: received ping\n", getpid());
    write(p[1], &byte, 1);
    exit(0);
  } else {
    // Parent
    write(p[1], &byte, 1);
    if(read(p[0], &byte, 1) != 1){
      fprintf(2, "parent read error\n");
      exit(1);
    }
    printf("%d: received pong\n", getpid());
    wait(0);
    exit(0);
  }
}
