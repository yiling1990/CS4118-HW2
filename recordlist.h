#include "record.h"
#include "proc.h"
#define NULL (void*)0

//typedef struct rnode recordnode;

struct rnode
{
	struct record rec;
	struct rnode *next;
};


void add_record(struct record* re);
/*
{
	struct rnode *cur = rlist;
	while(cur->next != NULL)
	{
		cur = cur->next;
	}
	
	struct rnode *newnode = (struct rnode*)kalloc();
	newnode->rec = re;
	newnode->next = NULL;
	cur->next = newnode;
}
*/
int fetchrecords(struct record *records, int num_records);
/*
{
	struct rnode *cur = proc->recordlist;
	int count = 0;
	if (records != NULL)
	{
		while(cur != NULL)
		{
			if (count < num_records)
			{
				records[count] = *(cur->rec);
				cur = cur->next;
				count++;
			}
			else
				break;
		}
	}
	else
	{
		while (cur != NULL)
		{
			count++;
			cur = cur->next;
		}

	}
	return count;
}
*/
