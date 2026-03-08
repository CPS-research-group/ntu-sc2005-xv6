#include "kernel/types.h"
#include "user/user.h"

#define SBRK_ERROR ((char *)-1)

int countvp(void) {
  // TODO implement as syscall
  return 0;
}
int countpp(void) {
  // TODO implement as syscall
  return 0;
}

int
main(int argc, char *argv[])
{
  int n_pages = 30;
  int pg_size = 4096;
  int size = n_pages * pg_size;
  
  printf("Starting lazy allocation test...\n");
  
  int vp_before = countvp();
  int pp_before = countpp();
  printf("Before: VP=%d, PP=%d\n", vp_before, pp_before);
  
  // Allocate memory using sbrk (should be lazy)
  char *start = sbrk(size);
  if (start == SBRK_ERROR) {
    printf("sbrk failed\n");
    exit(1);
  }
  
  int vp_after_sbrk = countvp();
  int pp_after_sbrk = countpp();
  printf("After sbrk(%d): VP=%d, PP=%d\n", size, vp_after_sbrk, pp_after_sbrk);
  
  if (vp_after_sbrk - vp_before != n_pages) {
    printf("Error: VP count did not increase by %d\n", n_pages);
  }
  
  if (pp_after_sbrk != pp_before) {
    printf("Error: PP count changed immediately (should be lazy)\n");
  } else {
    printf("Success: PP count unchanged (lazy allocation verified)\n");
  }
  
  // Trigger page faults by writing
  printf("Touching pages...\n");
  for (int i = 0; i < n_pages; i++) {
    start[i * pg_size] = 'x';
  }
  
  int pp_after_touch = countpp();
  printf("After touching: PP=%d\n", pp_after_touch);
  
  if (pp_after_touch - pp_before >= n_pages) {
    printf("Success: PP count increased after touching pages\n");
  } else {
    printf("Error: PP count did not increase correctly\n");
  }
  
  exit(0);
}
