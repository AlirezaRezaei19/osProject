#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "fileExtern.h"


int
sys_fork(void)
{
  return fork();
}

int
sys_logsyscalls(void)
{
  return logsyscalls();
}

void printCount(int id,int pid,int hour,int minute)
{
int traceState = 1;

    // cmostime(r);
    // cprintf("time:",r->day);
    //cprintf("time %d\n",r->hour);
    // count[id+1]++;

    if(traceState)
    {
        switch(id)
        {
            case 1:
                cprintf("sys_fork %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 2:
                cprintf("sys_exit %d\n",pid,hour);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 3:
                cprintf("sys_wait %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 4:
                cprintf("sys_pipe %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 5:
                cprintf("sys_read %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 6:
                cprintf("sys_kill %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 7:
                cprintf("sys_exec %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 8:
                cprintf("sys_fstat %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 9:
                cprintf("sys_chdir %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 10:
                cprintf("sys_dup %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 11:
                cprintf("sys_getpid %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 12:
                cprintf("sys_sbrk %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 13:
                cprintf("sys_sleep %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 14:
                cprintf("sys_uptime %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 15:
                cprintf("sys_open %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 16:
                cprintf("sys_write %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                //cprintf("hour: %d\n",hour);
                break;
            case 17:
                cprintf("sys_mknod %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 18:
                cprintf("sys_unlink %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 19:
                cprintf("sys_link %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 20:
                cprintf("sys_mkdir %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 21:
                cprintf("sys_close %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 22:
                cprintf("sys_logsyscalls %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;
            case 23:
                cprintf("sys_syslog %d\n",pid);
                cprintf("hour: %d\n",hour);
                cprintf("minute: %d\n",minute);
                break;

        }

    }

}

void printLog(struct logsyscall syscall[] , int index){
    for (int i = 0; i < index ; ++i) {
        printCount(syscall[i].id, syscall[i].pid, syscall[i].hour,syscall[i].minute);
    }
}

int
sys_systemlog(void)
{

    printLog(record,index);
    return 0;
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
