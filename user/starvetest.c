#include "kernel/types.h"
#include "user/user.h"

/* Supplied observation workload. Run with CPUS=1; exit QEMU with Ctrl-a x. */
int
main(void)
{
  int gate[2], ready[2], pid[2];
  char token;

  if(pipe(gate) < 0 || pipe(ready) < 0){
    printf("starvetest: pipe failed\n");
    exit(1);
  }

  for(int i = 0; i < 2; i++){
    pid[i] = fork();
    if(pid[i] < 0){
      /* Closing the gate releases existing children with EOF. */
      close(gate[1]);
      close(gate[0]);
      close(ready[1]);
      close(ready[0]);
      for(int j = 0; j < i; j++)
        wait(0);
      printf("starvetest: fork failed\n");
      exit(1);
    }

    if(pid[i] == 0){
      close(gate[1]);
      close(ready[0]);
      if(write(ready[1], "r", 1) != 1)
        exit(1);
      close(ready[1]);
      if(read(gate[0], &token, 1) != 1)
        exit(1);
      close(gate[0]);

      /* No sleep, I/O, or exit after the gate: preemption keeps us runnable. */
      volatile uint64 value = 1;
      for(;;)
        value = value * 1664525 + 1013904223;
    }
  }

  close(gate[0]);
  close(ready[1]);
  int ok = ((pid[0] & 1) != (pid[1] & 1));
  for(int i = 0; i < 2; i++)
    if(read(ready[0], &token, 1) != 1)
      ok = 0;
  close(ready[0]);

  if(ok){
    int even = (pid[0] & 1) ? pid[1] : pid[0];
    int odd = (pid[0] & 1) ? pid[0] : pid[1];
    printf("starvetest: even pid %d; odd pid %d\n", even, odd);
    printf("starvetest: releasing children; Ctrl-p to inspect; Ctrl-a x to stop\n");
    if(write(gate[1], "gg", 2) != 2)
      ok = 0;
  }
  close(gate[1]);
  if(!ok)
    printf("starvetest: startup failed; restart in a fresh xv6 session\n");

  /* Normally blocks forever; the host ends this observation by exiting QEMU. */
  wait(0);
  wait(0);
  exit(ok ? 0 : 1);
}
