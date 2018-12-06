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


int
sys_backtrace(void)
{
    int n = 0;
    int i = 0;

    struct trapframe *tf = myproc()->tf;

    // Registers
    cprintf("eax:0x%x\n", tf->eax);
    cprintf("ebx:0x%x\n", tf->ebx);
    cprintf("ecx:0x%x\n", tf->ecx);
    cprintf("edx:0x%x\n", tf->edx);
    // Pointers
    cprintf("edi:0x%x\n", tf->edi);
    cprintf("esi:0x%x\n", tf->esi);
    cprintf("esp:0x%x\n", tf->esp);
    cprintf("eip:0x%x\n", tf->eip);
    cprintf("ebp:0x%x\n", tf->ebp);
    uint * us = (uint *) tf->ebp;
    while(us)
    {
        cprintf("#%d 0x%x\n", i++, *(us + 1));
        if(*(us+1) == 0xffffffff) break;
        us = (uint *) * us;
    }

    if(n)
        return n;
    return -1;
}

//int
//sys_getprocinfo()
//{
//    int proc_num = 0;
//    struct uproc *up;
//    getprocinfo(proc_num, &up);
//    return -1;
//}
