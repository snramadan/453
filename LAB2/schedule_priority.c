// priority

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
    insertAtTail(current,task);
}

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
	int priority = 0;
	int size = getSize(current);
	int count = 0;
	int index = 0;
	int i = 0;
	int used[size]; 
   temp = current;

	//initialize array
   for (i = 0; i < size; i++)
	{
		used[i] = 0;
	}

   while(size > 0)
	{
		//find highest priority 
		while (temp != NULL) 
		{
			if ((temp->task->priority > priority) && (used[count] != 1))
			{
				priority = temp->task->priority;
	         index = count;
			}
			temp = temp->next;
			count++;
		}
		//mark array at index as used
		used[index] = 1;
		temp = current;
		//loop to get to node that needs to be dealt with
		for (i = 0; i < index; i++)
		{
			temp = temp->next;
		}
		//run and print on node
		run(temp->task,temp->task->priority);
		printf("Task %s finished.\n", temp->task->name);
		//decrement size so there is no infinite loop
		size--;
		//reinitialize
		temp = current;
		count = 0;
		index = 0;
		priority = 0;
   }
}
