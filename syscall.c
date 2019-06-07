#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"
#include "date.h"
#include "fileExtern.h"


int count[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//int traceState = 1;
struct rtcdate *r;






/*



void printCount(int id,int pid,int hour)
{


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
                break;
            case 2:
                cprintf("sys_exit %d\n",pid,hour);
                cprintf("hour: %d\n",hour);
                break;
            case 3:
                cprintf("sys_wait %d\n",pid, hour);
                break;
            case 4:
                cprintf("sys_pipe %d\n",pid, hour);
                break;
            case 5:
                cprintf("sys_read %d\n",pid, hour);
                break;
            case 6:
                cprintf("sys_kill %d\n",pid, hour);
                break;
            case 7:
                cprintf("sys_exec %d\n",pid, hour);
                break;
            case 8:
                cprintf("sys_fstat %d\n",pid, hour);
                break;
            case 9:
                cprintf("sys_chdir %d\n",pid, hour);
                break;
            case 10:
                cprintf("sys_dup %d\n",pid, hour);
                break;
            case 11:
                cprintf("sys_getpid %d\n",pid, hour);
                break;
            case 12:
                cprintf("sys_sbrk %d\n",pid, hour);
                break;
            case 13:
                cprintf("sys_sleep %d\n",pid, hour);
                break;
            case 14:
                cprintf("sys_uptime %d\n",pid, hour);
                break;
            case 15:
                cprintf("sys_open %d\n",pid, hour);
                break;
            case 16:
                cprintf("sys_write %d\n",pid, hour);
                cprintf("hour: %d\n",hour);
                break;
            case 17:
                cprintf("sys_mknod %d\n",pid, hour);
                break;
            case 18:
                cprintf("sys_unlink %d\n",pid, hour);
                break;
            case 19:
                cprintf("sys_link %d\n",pid, hour);
                break;
            case 20:
                cprintf("sys_mkdir %d\n",pid, hour);
                break;
            case 21:
                cprintf("sys_close %d\n",pid, hour);
                break;
            case 22:
                cprintf("sys_toggle %d\n",pid, hour);
                break;
            case 23:
                cprintf("sys_add %d\n",pid, hour);
                break;
            case 24:
                cprintf("sys_ps %d\n",pid, hour);
                break;
        }

    }

}

void printLog(struct logsyscall syscall[] , int index){
    for (int i = 0; i < index ; ++i) {
        printCount(syscall[i].id, syscall[i].pid, syscall[i].hour);
    }
}
*/



// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from the current process.
int
fetchint(uint addr, int *ip)
{
  struct proc *curproc = myproc();

  if(addr >= curproc->sz || addr+4 > curproc->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int
fetchstr(uint addr, char **pp)
{
  char *s, *ep;
  struct proc *curproc = myproc();

  if(addr >= curproc->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)curproc->sz;
  for(s = *pp; s < ep; s++){
    if(*s == 0)
      return s - *pp;
  }
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  return fetchint((myproc()->tf->esp) + 4 + 4*n, ip);
}

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size bytes.  Check that the pointer
// lies within the process address space.
int
argptr(int n, char **pp, int size)
{
  int i;
  struct proc *curproc = myproc();

  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  int addr;
  if(argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_logsyscalls(void);
extern int sys_systemlog(void);


static int (*syscalls[])(void) = {
[SYS_fork]    sys_fork,
[SYS_exit]    sys_exit,
[SYS_wait]    sys_wait,
[SYS_pipe]    sys_pipe,
[SYS_read]    sys_read,
[SYS_kill]    sys_kill,
[SYS_exec]    sys_exec,
[SYS_fstat]   sys_fstat,
[SYS_chdir]   sys_chdir,
[SYS_dup]     sys_dup,
[SYS_getpid]  sys_getpid,
[SYS_sbrk]    sys_sbrk,
[SYS_sleep]   sys_sleep,
[SYS_uptime]  sys_uptime,
[SYS_open]    sys_open,
[SYS_write]   sys_write,
[SYS_mknod]   sys_mknod,
[SYS_unlink]  sys_unlink,
[SYS_link]    sys_link,
[SYS_mkdir]   sys_mkdir,
[SYS_close]   sys_close,
[SYS_logsyscalls]   sys_logsyscalls,
[SYS_systemlog] sys_systemlog,
};

void
syscall(void)
{
  int num;
    cmostime(r);
  struct proc *curproc = myproc();

  num = curproc->tf->eax;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
      //printCount(num , curproc->pid);
      index++;
      record[index].id = num;
      record[index].pid = curproc->pid;
      record[index].hour = r->hour;
      record[index].minute = r->minute;

      //printLog(record,index);
    curproc->tf->eax = syscalls[num]();
  } else {
    cprintf("%d %s: unknown sys call %d\n",
            curproc->pid, curproc->name, num);
    curproc->tf->eax = -1;
  }
}
