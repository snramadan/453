// round robin

#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

void add(char* name, int priority, int burst, struct node **current) {
    // traverse the list
    // call run
    Task *task = (Task*)calloc(1,sizeof(Task));
    task->name = name;
    task->priority = priority;
    task->burst = burst;
    insert(current,task);
}

//get size of node struct
int getSize(struct node* current)
{
	int size = 0;
   struct node* temp = current;
  
   if (size == 0 && temp != NULL)
	{
		size++;
	}

	while(temp->next != NULL)
	{
		size++;
		temp = temp->next;
	}

	return size;
}

void schedule(struct node* current) 
{
   struct node *temp;
	int zeroes = 0;
	int quantSize = 10;
	int size = getSize(current);
	int count = 0;
	int i = 0;
	int nodes[size];
	int bursts[size];
   temp = current;

	//initialize arrays
	for (i = 0; i < size; i++)
	{
		nodes[i] = 1;
		bursts[i] = temp->task->burst;
		temp = temp->next;
	}

   temp = current;
	//loop through until number of zeros == to the size 
   while(zeroes < size)
	{
		while (temp != NULL)
		{
			//if there is extra burst left over then call run and print
			//update burst array
			if ((nodes[count] != 0) && (bursts[count] - quantSize > 0))
			{
				temp->task->burst = bursts[count];
				run(temp->task,quantSize);
				bursts[count] = bursts[count] - quantSize;
			}
			//if there is less than 10 burst value then call run and print 
			//change burst to 0 and nodes array at index to 0
			//increment zeroes 
			else if ((nodes[count] != 0) && (bursts[count] - quantSize <= 0))
			{
				temp->task->burst = bursts[count];
				run(temp->task,temp->task->burst);
				printf("Task %s finished.\n", temp->task->name);
				bursts[count] = 0;
				nodes[count] = 0;
				zeroes++;
			}
			temp = temp->next;
			count++;
		}
		//reinitialize
		temp = current;
		count = 0;
   }	
}
