#include "kernel/types.h"
#include "kernel/riscv.h"
#include "user/user.h"

// Supplied Lab 4 test. Install as user/fault.c and add $U/_fault to UPROGS.
// Run "fault read" or "fault write" in the xv6 shell.
int
main(int argc, char *argv[])
{
  if(argc != 2 ||
     (strcmp(argv[1], "read") != 0 && strcmp(argv[1], "write") != 0)){
    fprintf(2, "usage: fault read|write\n");
    exit(1);
  }

  // sbrk(0) reports the current end of the process's memory without growing it.
  char *end = sbrk(0);
  if(end == (char *)-1){
    fprintf(2, "fault: sbrk(0) failed\n");
    exit(1);
  }

  // Choose a whole page outside the allocated range. If the break is already
  // page-aligned, it is the first address of the next, unallocated page.
  // Do not reserve this page: this access must remain invalid after lazy
  // allocation is implemented, too.
  uint64 address = PGROUNDUP((uint64)end);
  volatile char *target = (volatile char *)address;

  printf("fault: pid %d, break %p, target %p, mode %s\n",
         getpid(), (void *)end, (void *)address, argv[1]);

  // volatile ensures that the compiler emits the selected memory access.
  // A correctly protecting kernel terminates this process at that access.
  if(strcmp(argv[1], "write") == 0)
    *target = 'x';
  else
    (void)*target;

  fprintf(2, "fault: ERROR: access returned; expected process termination\n");
  exit(1);
}
