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
  counter[11]++;
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;
  counter[12]++;

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
  counter[14]++;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_count(void)
{
  counter[24]++;
  return count();
}

int sys_getppid(void){
  counter[21]++;
  struct proc *curproc = myproc();
  return curproc->parent->pid;
}

int sys_getChildren(void){
  int pid ;
  argint(0, &pid);
  return getChildren(pid);
}

int sys_cps(void){
  counter[25]++;
  return cps();
}

int
sys_changePriority (void)
{
  counter[26]++;
  int pr;
  if(argint(0, &pr) < 0)
    return -1;

  return changePriority (pr);
}

int
sys_changePolicy(void){
  counter[28]++;
  int plc;
    if(argint(0, &plc) < 0)
    return -1;

  return changePolicy (plc);
}

int
sys_waitForChildren(void){
  struct timeVariables* time;
    if(argptr(0, (void*)&time, (int) sizeof(time)) < 0)
    return -1;

  return waitForChildren (time);
}
