// first come first Serve
// just need to print it in the same order

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

void schedule(struct node* current) 
{
   struct node *temp;
   temp = current;
	//call run and print statement on all the nodes
   while (temp != NULL) 
	{
      run(temp->task,temp->task->burst);
		printf("Task %s finished.\n", temp->task->name);
      temp = temp->next;
   }
}
