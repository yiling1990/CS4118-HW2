#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "recordlist.h"
#define NULL (void*)0

int
sys_startrecording(){
  if(proc->logging == 1)
  {
     return -1;
  }

  else
  {
    proc->logging = 1;
    return 0;
  }
}

int
sys_stoprecording(){
  
  if(proc->logging == 0)
  {
     return -1;
  }
  
  else
  {
     proc->logging = 0;
     return 0;
  }
}

int
sys_fetchrecords(void)
{
  struct record *records;
  int num_records;
  //cprintf("sys_fetchrecords Records argument: %d\n",records);
  if(argptr(0,(void*)&records,sizeof(*records)) < 0)
    return -1;
  if(argint(1, &num_records) < 0)
    return -1;
  return fetchrecords(records, num_records);
  //return 0;
  //if first arg is null return total number of records (not num_records) 

}

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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int fetchrecords(struct record *records, int num_records)
{
	cprintf("Records argument: %d",records);
	struct rnode *cur = proc->recordlist;
	struct record *curRecord;
	int count = 0;
	if (records != 0)
	{
		cprintf("Copying records\n");
		while(cur != NULL && cur != 0)
		{
			//cprintf("%d\n", cur->rec);
			//cprintf("%d\n", cur->rec.value);
			if (count < num_records)
			{
				curRecord = cur->rec;
				records[count] = *curRecord;
				cprintf("\ncount:%d\n", count);
				cprintf("Memory location: %d\n", &records[count]);
				cprintf("syscalltype:%d\n", (&records[count])->type);
				int type = (&records[count])->type;
				if(type == 0)
					cprintf("syscall #:%d\n", (&records[count])->value.intval);
				if(type == 1)
					cprintf("syscall int:%d\n", (&records[count])->value.intval);
				if(type == 2)
					cprintf("syscall ptr:%d\n", (&records[count])->value.ptrval);
				if(type == 3)
					cprintf("syscall str:%s\n", (&records[count])->value.strval);
				if(type == 4)
					cprintf("syscall return val:%d\n", (&records[count])->value.intval);
				records[count] = *curRecord;
				cur = cur->next;
				count++;
			}
			else
				break;
		}
	}
	else
	{
		cprintf("Counting records\n");
		while (cur != NULL)
		{
			count++;
			cprintf("syscall#:%d\n", records[count].type);
			curRecord = cur->rec;
			cprintf("Record type2: %d", curRecord->type);
			cur = cur->next;
		}

	}
	cprintf("NumRecordsReturned: %d\n",count);
	return count;
}
