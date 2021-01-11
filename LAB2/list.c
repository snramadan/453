/**
 * Various list operations
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"


// add a new task to the list of tasks
void insert(struct node **head, Task *newTask) {
    // add the new task to the list
    struct node *newNode = malloc(sizeof(struct node));

    newNode->task = newTask;
    newNode->next = *head;
    *head = newNode;
}

// delete the selected task from the list
void delete(struct node **head, Task *task) {
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name,temp->task->name) == 0) {
        *head = (*head)->next;
    }
    else {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name,temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

// traverse the list
void traverse(struct node *head) {
    struct node *temp;
    temp = head;

    while (temp != NULL) {
        printf("[%s] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }
}

// adding at the end
void insertAtTail(struct node** head, Task *new_task) {
    // if(traverse(node) != null) {
    //     insertAtTail(node,data);
    // }
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
    struct node* current_node = *head;

    new_node->task = new_task;
    new_node->next = NULL;
	
    if(current_node == NULL) 
	 {
       *head = new_node;
    }
    else 
	 {
      // otherwise, iterate through linkedlist
		while(current_node->next != NULL) 
		{
			current_node = current_node->next;
		}

      // found the end
      current_node->next = new_node;
    }
}
