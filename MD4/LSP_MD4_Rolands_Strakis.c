#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/mman.h>

int main(void)
{
	void *i1, *i2, *i3;
	printf("allocating...\n");
	int i;
	printf("Malloc\n");
	for(i = 0; i < 1024; i++){
		i1 = malloc(1024*1024);
		printf("i: '%d'  i1: '%p'  sbrk: '%p'\n", i, i1, sbrk(0));
		memset(i1, 0, sizeof(i1));
	}
	printf("MMAP\n");
	for(i = 0; i < 1024;i++){
		i2 = mmap(0, 1024*1024, 3, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		printf("i: '%d'  i2: '%p'  sbrk: '%p'\n", i, i2, sbrk(0));
		memset(i2, 0, sizeof(i2));
	}
	printf("Sbrk\n");
	for(i = 0; i < 1024; i++){
		i3 = sbrk(1024*1024);
		printf("i: '%d'  i3: '%p'  sbrk: '%p'\n", i, i3, sbrk(0));
		memset(i3, 0, sizeof(i3));
	}
	while(1){}
	printf("done\n");
    	return 0;
}
