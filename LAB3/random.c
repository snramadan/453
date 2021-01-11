#include <stdlib.h>
#include <stdio.h>
 
int main()
{
	FILE *f = fopen("input.txt", "w");
	int i;
	srand(1);
	for (i = 0; i < 5000; i++)
	{
		fprintf(f, "%d ", (rand()%5000));
	}
	close(f);
	return 1;
}
