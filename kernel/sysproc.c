#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// System call wrappers for shared user counter
uint64 sys_ucnt_set(void) {
  int idx, val;
  argint(0, &idx);
  argint(1, &val);
  ucnt_set(idx, val);
  return 0;
}

uint64 sys_ucnt_get(void) {
  int idx;
  argint(0, &idx);
  return ucnt_get(idx);
}

// System call wrappers for user buffer
uint64 sys_ubuf_write(void) {
  int val;
  argint(0, &val);
  ubuf_write((char)val);
  return 0;
}

uint64 sys_ubuf_read(void) {
  return (uint64) ubuf_read();
}

// Kernel-level semaphore syscall handlers
uint64 sys_sem_init(void) {
  int value;
  argint(0, &value);
  return sem_alloc(value);
}

uint64 sys_sem_wait(void) {
  int semid;
  argint(0, &semid);
  return sem_wait(semid);
}

uint64 sys_sem_signal(void) {
  int semid;
  argint(0, &semid);
  return sem_signal(semid);
}

uint64 sys_sem_getvalue(void) {
  int semid;
  argint(0, &semid);
  return sem_getvalue(semid);
}

uint64 sys_sem_free(void) {
  int semid;
  argint(0, &semid);
  return sem_free(semid);
}
