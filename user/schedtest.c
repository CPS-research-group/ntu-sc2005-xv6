#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NCHILD 4
#define ROUNDS 4
#define WORK 40000000

static char *
parity(int pid)
{
  return (pid % 2 == 0) ? "even" : "odd";
}

/* CPU-bound work: the process remains RUNNABLE when preempted. */
static void
cpu_work(void)
{
  volatile uint64 value = 1;

  for(uint64 i = 0; i < WORK; i++)
    value = value * 1664525 + i + 1013904223;
}

int
main(void)
{
  int gate[2];
  int created = 0;

  if(pipe(gate) < 0){
    printf("schedtest: pipe failed\n");
    exit(1);
  }

  for(int child = 0; child < NCHILD; child++){
    int pid = fork();

    if(pid < 0){
      printf("schedtest: fork failed\n");
      break;
    }

    if(pid == 0){
      char token;
      int me = getpid();

      close(gate[1]);

      /* Wait until the parent has created every child. */
      if(read(gate[0], &token, 1) != 1){
        printf("child %d: gate read failed\n", child);
        exit(1);
      }
      close(gate[0]);

      printf("child %d: pid %d (%s) start\n",
             child, me, parity(me));

      for(int round = 1; round <= ROUNDS; round++){
        cpu_work();
        printf("child %d: pid %d (%s) round %d/%d\n",
               child, me, parity(me), round, ROUNDS);
      }

      printf("child %d: pid %d (%s) done\n",
             child, me, parity(me));
      exit(0);
    }

    printf("parent: created child %d as pid %d (%s)\n",
           child, pid, parity(pid));
    created++;

    /*
     * Give an optional FCFS implementation distinguishable
     * creation times.
     */
    if(child + 1 < NCHILD)
      sleep(1);
  }

  close(gate[0]);

  /*
   * Release every child with one write. This makes the children
   * compete as runnable processes at approximately the same time.
   */
  char tokens[NCHILD];
  for(int i = 0; i < created; i++)
    tokens[i] = 'g';

  if(write(gate[1], tokens, created) != created)
    printf("schedtest: gate write failed\n");

  close(gate[1]);

  for(int i = 0; i < created; i++)
    wait(0);

  printf("schedtest: all children finished\n");
  exit(created == NCHILD ? 0 : 1);
}