#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

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


int
sys_list_all_processes(void)
{
  return list_all_processes();
}

int sys_create_palindrome(void) {
  int n;
  if (argint(0, &n) < 0)
    return -1;
  
  int reversed = 0, temp = n;
  while (temp != 0) {
    reversed = reversed * 10 + temp % 10;
    temp /= 10;
  }
  cprintf("%d%d\n", n, reversed);
  return 0;
}

int sys_sort_syscalls(void)
{
  int pid;
  if (argint(0, &pid) < 0)
    return -1;
  acquire(&ptable.lock);
  const char *syscall_names[] = {[0] = "unknown",[1] = "fork",[2] = "exit",[3] = "wait",[4] = "pipe",[5] = "read",[6] = "kill",[7] = "exec",[8] = "fstat",[9] = "chdir",[10] = "dup",[11] = "getpid",[12] = "sbrk",[13] = "sleep",[14] = "uptime",[15] = "open",[16] = "write",[17] = "mknod",[18] = "unlink",[19] = "link",[20] = "mkdir",[21] = "close",[22] = "create_palindrome",[23] = "move_file",[24] = "sort_syscalls",[25] = "get_most_invoked_syscall" };
  int flag=0;
  struct proc *p;
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if(p->pid == pid) {
          int count = p->syscall_count;
          int unordered_syscalls[count];
          int cnt;
          for (cnt =0;cnt < count;cnt++)       {
            unordered_syscalls[cnt] = p->syscalls[cnt];
          }

          for (int i=0;i < count-1 ;i++) {
            for (int j=0;j< count-i-1;j++) {
              if (unordered_syscalls[j] > unordered_syscalls[j+1]) {
                int temp = unordered_syscalls[j];
                unordered_syscalls[j] = unordered_syscalls[j+1];
                unordered_syscalls[j+1] = temp;
              }
            }
          }
          int new_add[100];
          int new_add_count=0;
          int unique=0;
          for (int i=0;i<count;i++) {
            for (int j =0;j<20;j++) {
              if (unordered_syscalls[i] == new_add[j])
                unique++;
            }
            if (unique == 0) {
              new_add[new_add_count] = unordered_syscalls[i];
              new_add_count++;
               }
            unique=0;
          }
          cprintf("Sorted System Calls for Process %s\n",p->name);
          for (int i=0;i<new_add_count;i++)
              cprintf("%s\n",syscall_names[new_add[i]]);
          flag++;
        }
  }
  release(&ptable.lock);
  if (flag ==0)
    return -1;
  return 0;
}
int sys_get_most_invoked_syscall(void)
{
  int pid;
  if (argint(0, &pid) < 0)
    return -1;
  acquire(&ptable.lock);
  const char *syscall_names[] = {[0] = "unknown",[1] = "fork",[2] = "exit",[3] = "wait",[4] = "pipe",[5] = "read",[6] = "kill",[7] = "exec",[8] = "fstat",[9] = "chdir",[10] = "dup",[11] = "getpid",[12] = "sbrk",[13] = "sleep",[14] = "uptime",[15] = "open",[16] = "write",[17] = "mknod",[18] = "unlink",[19] = "link",[20] = "mkdir",[21] = "close",[22] = "create_palindrome",[23] = "move_file",[24] = "sort_syscalls",[25] = "get_most_invoked_syscall" };
  struct proc *p;
  int flag=0;
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if(p->pid == pid) {
          int count = p->syscall_count;
          int uni_syscalls[30] = {0};
          int cnt;
          for (cnt =0;cnt < count;cnt++)       {
            uni_syscalls[p->syscalls[cnt]]++;
          }
          int max=0;
          int sys_num=0;
          for (int i=1;i<30;i++) {
              if ( (uni_syscalls[i]) > (uni_syscalls[i-1]) ) {
                max = uni_syscalls[i];
                sys_num = i;
              }
          }
          cprintf("Most Invoked System Call for Process %s\n",p->name);
          cprintf("Syscall Name: %s\nNumber of times: %d\n",syscall_names[sys_num],max);
          flag++;
        }
  }
  release(&ptable.lock);
  if (flag == 0)
    return -1;
  return 0;
}