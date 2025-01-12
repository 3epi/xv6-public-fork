#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Shared memory

extern int get_sharedmem(uint, uint, int);
extern int close_sharedmem(void*);
extern void * open_sharedmem(int, void*, int);
extern int control_sharedmem(int, int, void*);

// system call handler for get_sharedmem
int
sys_get_sharedmem(void)
{
  int key, size, shmflag;
  // check for valid arguments
  if(argint(0, &key) < 0)
    return -1;
  if(argint(1, &size) < 0)
    return -1;
  if(argint(2, &shmflag) < 0)
    return -1;
  return get_sharedmem((uint)key, (uint)size, shmflag);
}

// system call handler for close_sharedmem
int sys_close_sharedmem(void)
{
  int i;
  // check for valid argument
  if(argint(0,&i)<0)
    return 0;
  return close_sharedmem((void*)i);
}

// system call handler for control_sharedmem
int
sys_control_sharedmem(void)
{
  int shmid, cmd, buf;
  // check for valid arguments
  if(argint(0, &shmid) < 0)
    return -1;
  if(argint(1, &cmd) < 0)
    return -1;
  if(argint(2, &buf) < 0)
    return -1;
  return control_sharedmem(shmid, cmd, (void*)buf);
}

// system call handler for open_sharedmem
void*
sys_open_sharedmem(void)
{
  int shmid, shmflag;
  int i;
  // check for valid arguments
  if(argint(0, &shmid) < 0)
    return (void*)0;
  if(argint(1,&i)<0)
    return (void*)0;
  if(argint(2, &shmflag) < 0)
    return (void*)0;
  return open_sharedmem(shmid, (void*)i, shmflag);
}