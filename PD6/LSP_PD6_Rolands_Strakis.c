#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

int main(void)
{
	void *i1, *i2;
	int i = 1;
    printf("allocating...\n");
    
    /*Eksperimenti*/
	/*
	i1 = malloc(UINT_MAX);
	printf ("%p  %p\n", sbrk(0), i1);
	while(1){
		i2 = malloc(1024*1024);
		printf ("%p  %p\n", sbrk(0), i2);
		
		i++;
		printf("i: %d\n", i);
	}
	*/

    /* Novērtējums */
    /*
    clock_t start, end;
    float seconds;

    start = clock();
    i1 = malloc(1024*1024*100);
    memset(i1, 0, sizeof(i1));
	end = clock();
	seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("Izpildes laiks vienam gabalam: %f\n", seconds);

	start = clock();
	for(i = 0; i < 100; i++){
		i2 = malloc(1024*1024);
		memset(i1, 0, sizeof(i1));
	}
	end = clock();
	seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("Izpildes laiks ciklam: %f\n", seconds);

	while(1){}
	*/
    printf("done\n");
    return 0;
}