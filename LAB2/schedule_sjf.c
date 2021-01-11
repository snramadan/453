// shortest job first

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
	int lowestBurst = 999999;
	int size = getSize(current);
	int count = 0;
	int index = 0;
	int i = 0;
	int used[size]; 
   temp = current;

	//initalize used array to all zero
   for (i = 0; i < size; i++)
	{
		used[i] = 0;
	}

   while(size > 0)
	{
		//find the lowest burst
		while (temp != NULL) 
		{
			if ((temp->task->burst < lowestBurst) && (used[count] != 1))
			{
				lowestBurst = temp->task->burst;
	         index = count;
			}
			temp = temp->next;
			count++;
		}
		//update the used array and mark the array at index as used
		used[index] = 1;
		temp = current;
		//loop through node struct to get to the node that needs to me returned
		for (i = 0; i < index; i++)
		{
			temp = temp->next;
		}
		//run and print info of node
		run(temp->task,temp->task->burst);
		printf("Task %s finished.\n", temp->task->name);
		//decrement size to not be in constant loop
		size--;
		//reinitialize
		temp = current;
		count = 0;
		index = 0;
		lowestBurst = 999999;
   }
}
