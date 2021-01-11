// priority round robin

#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

void add(char* name, int priority, int burst, struct node **current) 
{
	// traverse the list
   // call run
   Task *task = (Task*)calloc(1,sizeof(Task));
   task->name = name;
   task->priority = priority;
   task->burst = burst;
   insertAtTail(current,task);
}

//get amount of nodes and return value
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
	int index = 0;
	int count = 0;
	int c = 0;
	int i = 0;
	int used[size];
	int same[size];
	int bursts[size];
	int zero[size];
	temp = current;

	//initialize all arrays
	for (i = 0; i < size; i++)
	{
		used[i] = 0;
		same[i] = 0;
		zero[i] = 1;
		bursts[i] = temp->task->burst;
		temp = temp->next;
	}

	temp = current;
	while (size > 0)
	{
		//find priority
		while (temp != NULL)
		{
			if ((temp->task->priority > priority) && (used[c] != 1))
			{
				priority = temp->task->priority;
				index = c;
			}
			temp = temp->next;
			c++;
		}
		//add it to used array and array of same priorities
		used[index] = 1;
		same[index] = 1;
		temp = current;
		c = 0;
		count = 1;
		//find equal priorities
		while (temp != NULL)
		{
			if ((temp->task->priority == priority) && (same[c] != 1))
			{
				same[c] = 1;
				count++;
				used[c] = 1;
			}
			c++;
			temp = temp->next;
		}
		temp = current;
		//print out statements if there is no repeat priorities
		if (count == 1)
		{
			for (i = 0; i < index; i++)
			{
				temp = temp->next;
			}
			run(temp->task, temp->task->burst);
			printf("Task %s finished.\n", temp->task->name);
			same[index] = 0;
			zero[c] = 0;
			size--;
		}
		//loop through same area and do RR on the nodes
		else if (count > 1)
		{
			temp = current;
			c = 0;
			while(count > 0)
			{
				while(temp != NULL)
				{
					//check if the priority is marked as same
					//check if the the node does not have any more burst value
					if (same[c] == 1 && zero[c] != 0)
					{
						if (bursts[c] - 10 > 0)
						{
							temp->task->burst = bursts[c];
							run(temp->task, 10);
							bursts[c] = bursts[c] - 10;
						}
						else if (bursts[c] - 10 <= 0)
						{
							temp->task->burst = bursts[c];
							run(temp->task, temp->task->burst);
							printf("Task %s finished.\n", temp->task->name);
							bursts[c] = 0;
							zero[c] = 0;
							same[c] = 0;
							count--;
							size--;
						}
					}
					temp = temp->next;
					c++;
				}
				temp = current;
				c = 0;
			}
		}
		//restart all variables
		temp = current;
		c = 0;
		index = 0;
		priority = 0;
		count = 0;
	}
}
