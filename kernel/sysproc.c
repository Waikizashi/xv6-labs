#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

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

//my_trace syscalls using mask
uint64
sys_trace(void)
{
    int mask;
    argint(0, &mask);
    //никогда не настанет ошибка
    myproc()->trace_mask = mask; //nastavujeme tu masku
    //вызвали функцию, возвращает структуру,
//    struct p = myproc();
//    *p->trace_mask = mask;   -равносильно
    return 0;
}

uint64 sys_sysinfo(void){
  struct proc * p;
  struct sysinfo info;
  uint64 userInfoPointer;
  uint64 sysFreeMem;
  uint64 sysUsedProc;
  // get myproc
  p = myproc();
  // get pointer of struct sysinfo in user space
  argaddr(0,&userInfoPointer);
   
  // get free mem
  sysFreeMem = getFreeMem();
  // get free proc
  sysUsedProc = getUsedProc();
  info.freemem = sysFreeMem;
  info.nproc = sysUsedProc;
  // use copy out to copy info from kernel space to user space
  return copyout(p->pagetable,userInfoPointer,(char*)&info,sizeof(info));
}

