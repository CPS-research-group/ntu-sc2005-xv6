#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  char buf[5];
  
  pipe(p);
  if(fork() == 0){
    // Child
    char* buf;
    for (int j=0; j<5; j++) {
      if ((j % 2) == 0) {
        buf = "ping";
        write(p[1], buf, 4);
      } else {
        buf = "pong";
        write(p[1], buf, 4);
      }
    }
    exit(0);
  } else {
    // Parent
    for (int j=0; j<5; j++) {
      read(p[0], buf, 4);
      printf("%d: received %s\n", getpid(), buf);
    }

    close(p[0]);
    close(p[1]);
    wait(0);
    exit(0);
  }
}
