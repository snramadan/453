#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SIZE 5000
#define THREADS 3   
int arr[SIZE];
int finalArr[SIZE];

typedef struct
{
    int first;
    int last;
} Index;

void *sortArrays(void *indexes)
{
   Index* in = (Index*)indexes;
   int f = in->first;
   int l = in->last+1;
   int z, i, j, t, k, temp, x;

   //selection sort 
	for (i = f; i < l-1; i++)
	{
		k = i;
		for ( j = i+1; j < l; j++)
		{
			if (arr[j] < arr[k])
			{
				k = j;
			}
		}
		temp = arr[k];
		arr[k] = arr[i];
		arr[i] = temp;
	}

   //place sorted arrays into the final array
   for(x = f; x < l; x++)
   {
      finalArr[x] = arr[x];
   }

   pthread_exit(0);
}

void *mergeArrays(void *indexes)
{
   FILE *F2 = fopen("output.txt", "w"); 
   Index* in = (Index*)indexes;
   int f = in->first;
   int l = in->last;
   int i, j, k, temp, ind, lineSize;

	//selection sort the final array
	for (i = f; i < l-1; i++)
	{
		k = i;
		for ( j = i+1; j < l; j++)
		{
			if (finalArr[j] < finalArr[k])
			{
				k = j;
			}
		}
		temp = finalArr[k];
		finalArr[k] = finalArr[i];
		finalArr[i] = temp;
	}

	//place array into the output file
	lineSize = 0;
   for(ind = 0; ind < SIZE; ind++)
   {
		if (lineSize < 10)
		{
			fprintf(F2, "%d ", finalArr[ind]);
	      lineSize++;
		}
		else
		{  
			fprintf(F2, "\n%d ", finalArr[ind]);
			lineSize = 0;
		}
   }

	close(F2);
   pthread_exit(0);
}

int main (int argc, const char * argv[])
{
	struct timespec start, finish;
	double elapsed;
   int i;
   FILE *F1 = fopen("input.txt", "r");
   //check if file exists
	if (F1 == NULL)
   {
      printf("Error opening file\n");
      exit(1);
   }
	/* scans for the numbers in the file*/
   for (i = 0; i < SIZE; i++)
	{
		fscanf(F1, "%d ", &(arr[i]));
	}
	close(F1);

   pthread_t threads[THREADS];

	clock_gettime(CLOCK_MONOTONIC, &start);  
	/* first thread */
   Index *in = (Index *)malloc(sizeof(Index));
   in->first = 0;
   in->last = (SIZE/2) - 1;
   pthread_create(&threads[0], 0, sortArrays, in);
    
   /* second thread */
   in = (Index *)malloc(sizeof(Index));
   in->first = (SIZE/2);
   in->last = SIZE - 1;
   pthread_create(&threads[1], 0, sortArrays, in);
    
   /* wait */
   for (i = 0; i < THREADS - 1; i++)
   {
		pthread_join(threads[i], NULL);
   }

	/* ... */
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

   printf("Elapsed Time for Two Thread: %f \n", elapsed);

	clock_gettime(CLOCK_MONOTONIC, &start);  
   /* merging thread */
	in = (Index *) malloc(sizeof(Index));
   in->first = 0;
   in->last = (SIZE);
   pthread_create(&threads[2], 0, mergeArrays, in);
   pthread_join(threads[2], NULL);

	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("Elapsed Time for One Thread: %f \n", elapsed);
   return 0;
}
