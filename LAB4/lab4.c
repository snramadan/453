#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Process
{
    int start;
    int end;
	 int size;
	 int isHole;
    char *name;
	 struct Process *next;
} Process;

int main(int argc, char **argv)
{
    int mem, boolean, size, i, currMem, pMem, done, found;
	 char pApp = ' ';
	 char *pName = "";
    char in[100];
    boolean = 0;
    mem = 0;
	 done = 0;
	 found = 0;

    /* get memory size from arguments and check if correct */
    if (argv[1] == NULL)
    {
        printf("No memory size given\n");
		  exit(0);
    }
    else
    {
        mem = atoi(argv[1]);
    }

    //keep track of memory
	 currMem = mem;
		
	 /* create and initialize linkedlist */
    Process *process = (Process*)malloc(sizeof(Process));
	 process->start = 0;
	 process->end = mem;
	 process->isHole = 1;
	 process->name = (char*)malloc(sizeof(char)*7);
	 process->name = "Unused";
	 process->next = NULL;
    printf("allocator> ");

    while (boolean == 0) 
    {
		  //get line and check if incorrect
		  if (!fgets(in, 100, stdin))
		  {
		     exit(0);
		  }
        i = 0;
        size = 1;

		  //get size of line
        while (in[i] != '\0')
        {
            if (in[i] == ' ')
            {
                size++;
            }
            i++;
        }
        
		  //complte X C and STAT commands
        if (size == 1)
        {
				char *temp = strtok(in, " ");
            if (in[0] == 'X')
            {
                exit(0);
            }
            else if (in[0] == 'C')
            {
                /*compact*/
					 Process *pro = (Process*)malloc(sizeof(Process));
	             pro->start = 0;
	             pro->end = mem;
	             pro->isHole = 1;
	             pro->name = (char*)malloc(sizeof(char)*7);
	             pro->name = "Unused";
	             pro->next = NULL;
					 //Process *loop = process;
					 Process *loop = process;
					 //Process *new = (Process*)malloc(sizeof(Process));
					 //new->name = (char*)malloc(sizeof(char)*7);
					 //new->next = NULL;
					 while (loop != NULL)
					 {
						 if (loop->isHole == 0)
						 {
							 Process *temp = pro;
							 if (temp->start == 0 && temp->isHole == 1)
							 {
					          Process *new = (Process*)malloc(sizeof(Process));
					          new->name = (char*)malloc(sizeof(char)*7);
							    strcpy(new->name, loop->name);
								 new->start = 0;
							    new->end = loop->end;
								 new->isHole = 0;
								 new->next = temp;
								 temp->start = new->end+1;
								 pro = new; 
							 }
							 else
							 { 
								 done = 0;
						       while (temp != NULL && done == 0)
							    {
							       if (temp->next->isHole == 1)
							       { 
					                Process *new = (Process*)malloc(sizeof(Process));
					                new->name = (char*)malloc(sizeof(char)*7);
										 strcpy(new->name, loop->name);
								       new->start = temp->end+1;
							          new->end = temp->end + (loop->end - loop->start+1);
									    new->isHole = 0;
										 new->next = NULL;
									    Process *ptr = temp->next;
									    temp->next = new;
									    ptr->start = new->end+1;
									    temp->next->next = ptr;
									    done = 1;
							       }
								    temp = temp->next;

						       }
						    }
					    }
						 loop = loop->next;
					 }
				    process = pro;
            }
            else if (strcmp(temp, "STAT\n") == 0)
            {
                /*print stats*/
					 printf("\n");
					 Process *p = process;
					 while (p != NULL)
					 {
						 printf("Address[%d:%d] Process %s\n", p->start, p->end, p->name);
						 p = p->next;
					 }
            }
        }
		  //complete RQ and RL commands
        else
        {
			  char *str = strtok(in, " ");
			  found = 0;
			  int m = 0;
			  Process *temp = process;
			  //release first Process
			  if (strcmp(str, "RL") == 0)
			  {
			     str = strtok(NULL, " ");
				  strtok(str, "\n");
				  if (strcmp(str, temp->name) == 0)
				  {
				     temp->name = "Unused";
					  temp->isHole = 1;
					  m = temp->end - temp->start;
					  currMem = currMem + m;
					  found = 1;
					  if (temp->next->isHole == 1)
					  {  
					     Process *hold = temp->next->next;
						  temp->end = temp->next->end;
						  free(temp->next);
						  temp->next = hold;
					  }	
				  }
				  //release middle processes
				  while(temp != NULL && found == 0)
				  {
					  if (strcmp(str, temp->next->name) == 0)
					  {
					     temp->next->name = "Unused";
						  temp->next->isHole = 1;
						  m = temp->next->end - temp->next->start;
						  currMem = currMem + m;
						  found = 1;
						  //combine unused 
						  if (temp->next->next->isHole == 1)
						  {  
							  Process *hold = temp->next->next;
						     temp->next->next->start = temp->next->start;
							  free(temp->next);
							  temp->next = hold;
					     }	
						  if (temp->isHole == 1)
						  {
							  Process *hold = temp->next->next;
							  temp->end = temp->next->end;
							  free(temp->next);
							  temp->next = hold;
						  }
					  }
					  temp = temp->next;
				  }
				  if (found == 0)
				  {
					  printf("Error: Process %s was not found", str);
				  }
			  }
			  else if (strcmp(str, "RQ") == 0)
			  {
				  //get process name
				  str = strtok(NULL, " ");
				  pName = str;
				  //get Memory size request 
				  str = strtok(NULL, " ");
				  pMem = atoi(str);
				  //get approach
				  str = strtok(NULL, " ");
				  pApp = str[0];
				  
				  if (currMem - pMem < 0)
				  {
					  printf("Not enough memory left.\n");
				  }
				  else
				  {
					  //find first spot
					  //Process *head = process;
					  Process *temp = process;
					  Process *new = (Process*)malloc(sizeof(Process));
					  new->name = (char*)malloc(sizeof(char)*7);
					  new->next = NULL;
					  Process *ptr = temp;
				     if (pApp == 'F')
					  {
						  //add first process
						  if (temp->start == 0 && temp->isHole == 1)
						  {
							  strcpy(new->name, pName);
							  new->start = 0;
						     new->end = pMem;
							  new->isHole = 0;
							  new->next = temp;
							  temp->start = pMem+1;
							  process = new;
							  currMem = currMem - pMem;
						  }
						  //add more processes
						  else
						  {
							  done = 0;
							  while (temp != NULL && done == 0)
							  {
								  //printf("\nENTER: %s %d %d\n", temp->name, temp->start, temp->end);
								  if ((temp->next->isHole == 1) && ((temp->next->end - temp->next->start)+1 >= pMem))
							     {
									  strcpy(new->name, pName);
									  new->start = temp->end+1;
							        new->end = new->start-1 + pMem;
									  new->isHole = 0;
								     ptr = temp->next;
									  temp->next = new;
									  ptr->start = new->end+1;
									  temp->next->next = ptr;
									  currMem = currMem - pMem;
									  done = 1;
							     }
								  temp = temp->next;
							  }
						  }
					  }	
					  //find smallest place
					  else if (pApp == 'B')
					  {
					     Process *temp = process;
						  int MSize, j, done, c, min;
						  //char* min = (char*)malloc(sizeof(char)*7);
						  MSize = mem;
						  done = 0;
						  c = 1;
						  min = 0;
						  while (temp != NULL)
						  {
							  int length = (temp->end - temp->start)+1;
							  if ((temp->isHole == 1) && (length < MSize) && (length >= pMem))
							  {
								   min = c;
								   //strcpy(min, temp->name);
									MSize = length;
							  }
							  c++;
							  temp = temp->next;
						  }
					     Process *loop = process;
						  /*while (loop != NULL && done == 0)
						  {
							  if (strcmp(loop->name, min) == 0)
							  {
								  done = 1;
							  }
							  loop = loop->next;
						  }*/
						  for (j = 1; j < min - 1; j++)
						  {
							  loop = loop->next;
						  }
						  printf("Process %s\n", loop->name);
						  if (MSize >= pMem)
						  {
							  if (MSize == pMem)
							  {
						        strcpy(loop->name, pName);
						        loop->isHole = 0;
								  currMem = currMem - pMem;
							  }
							  else
							  {
						        Process *new = (Process*)malloc(sizeof(Process));
						        new->name = (char*)malloc(sizeof(char)*7);
								  strcpy(new->name, pName);
								  new->start = loop->end+1;
							     new->end = new->start-1 + pMem;
								  new->isHole = 0;
								  Process *ptr = loop->next;
								  loop->next = new;
								  ptr->start = new->end+1;
								  loop->next->next = ptr;
								  currMem = currMem - pMem;
							  }
                    }
						  else 
						  {
						     printf("Error: Not enough room for memory to fit.\n");
						  }
					  }
					  //find biggest place
					  else if (pApp == 'W')
					  {
					     Process *temp = process;
						  int MSize, j, done, max, c;
						  //char *max = (char*)malloc(sizeof(char)*7);
						  MSize = 0;
						  done = 0;
						  c = 1;
						  max = 0;
						  while (temp != NULL)
						  {
							  int length = (temp->end - temp->start)+1;
							  if (temp->isHole == 1 && length > MSize && length >= pMem)
							  {
								   max = c;
									//strcpy(max, temp->name);
									MSize = length;
							  }
							  temp = temp->next;
							  c++;
						  }
					     Process *loop = process;
						  /*while (loop != NULL && done == 0)
						  {
							  if (strcmp(max, loop->name) == 0)
							  {
								  done = 1;
							  }
							  loop = loop->next;
						  }*/
						  for (j = 1; j < max - 1; j++)
						  {
							  loop = loop->next;
						  }
						  if (MSize >= pMem)
						  {
							  if (MSize == pMem)
							  {
						        strcpy(loop->name, pName);
						        loop->isHole = 0;
								  currMem = currMem - pMem;
							  }
							  else
							  {
						        Process *new = (Process*)malloc(sizeof(Process));
						        new->name = (char*)malloc(sizeof(char)*7);
								  strcpy(new->name, pName);
								  new->start = loop->end+1;
							     new->end = new->start-1 + pMem;
								  new->isHole = 0;
								  Process *ptr = loop->next;
								  loop->next = new;
								  ptr->start = new->end+1;
								  loop->next->next = ptr;
								  currMem = currMem - pMem;
							  }
                    }
						  else 
						  {
						     printf("Error: Not enough room for memory to fit.\n");
						  }
					  }
				  }
			  }
        }
        printf("allocator> ");
    } 

    return 0;
}
