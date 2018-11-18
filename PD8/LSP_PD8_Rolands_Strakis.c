#include <stdio.h>

#define MY_BUFFER_SIZE 4096
#define ALLOCATED_FLAG_SIZE 1
#define NULL_CHARACTER 1

unsigned char mybuffer[MY_BUFFER_SIZE];

void *myalloc(size_t size);
int myfree(void * ptr);
unsigned long last = 0;

void *myalloc(size_t size){
	void *mem;
	unsigned long seekStart = last;
	unsigned long freeSize;
	do{
		if(*((char *)(mybuffer + seekStart + sizeof(unsigned long))) == '0'){
			freeSize = *((unsigned long *)(mybuffer + seekStart));
			if(freeSize > size){
				mem = mybuffer + seekStart + sizeof(unsigned long) + ALLOCATED_FLAG_SIZE;
				*((unsigned long *)(mem - ALLOCATED_FLAG_SIZE - sizeof(unsigned long))) = (unsigned long)size;
				*((char *)(mem - ALLOCATED_FLAG_SIZE)) = '1';
				*((char *)(mem + size)) = '\0';
				last = seekStart + sizeof(unsigned long) + ALLOCATED_FLAG_SIZE + size + NULL_CHARACTER;
				printf("last eq '%ld'\n", last);
				if(freeSize - size > sizeof(unsigned long) + ALLOCATED_FLAG_SIZE + NULL_CHARACTER){
					*(unsigned long *)(mybuffer + last) = freeSize - sizeof(unsigned long) - ALLOCATED_FLAG_SIZE - size - NULL_CHARACTER;
					*((char *)(mybuffer + last + sizeof(unsigned long))) = '0';
				}
				printf("Gave memory at address '%p'\n", mem - sizeof(unsigned long) - ALLOCATED_FLAG_SIZE);
				return mem;
			}
			printf("freeSize less than required '%ld' < '%ld'\n", freeSize, size);
		}
		seekStart += sizeof(unsigned long) + ALLOCATED_FLAG_SIZE + *((unsigned long *)(mybuffer + seekStart)) + NULL_CHARACTER;
		if(seekStart > MY_BUFFER_SIZE){
			seekStart = 0;
		}
		printf("seekStart eq '%ld'", seekStart);
	}while(seekStart != last);
	printf("Couldn't give memory of size: '%ld'\n", size);
	return NULL;
}

int myfree(void *ptr){
	if((char *)ptr >= (char *)mybuffer && (char *)ptr <= (char *)mybuffer + MY_BUFFER_SIZE){
		*((char *)(ptr - ALLOCATED_FLAG_SIZE)) = '0';
		printf("Freed memory from address '%p'\n", ptr - sizeof(unsigned long) - ALLOCATED_FLAG_SIZE);
		ptr = NULL;
		return 0;
	}
	return -1;
}

int main(){
	printf("mybuffer start address '%p'\n", mybuffer);
	printf("mybuffer end address '%p'\n", mybuffer + MY_BUFFER_SIZE);
	*((unsigned long *)mybuffer) = (unsigned long)MY_BUFFER_SIZE - sizeof(unsigned long) - ALLOCATED_FLAG_SIZE;
	*((char *)(mybuffer + sizeof(unsigned long))) = '0';
	/*
	printf("%ld\n", *(unsigned long *)mybuffer);
	printf("%ld\n", *(unsigned long *)(mybuffer + 1));
	*/
	void *a = myalloc(50);
	printf("%ld\n", *(unsigned long *)(a - ALLOCATED_FLAG_SIZE - sizeof(unsigned long)));
	printf("%c\n", *((char *)(a - ALLOCATED_FLAG_SIZE)));
	printf("%ld\n", *(unsigned long *)(mybuffer + last));

	void *b = myalloc(4000);
	printf("%ld\n", *((unsigned long *)(b - ALLOCATED_FLAG_SIZE - sizeof(unsigned long))));
	printf("%c\n", *((char *)(b - ALLOCATED_FLAG_SIZE)));

	myfree(a);
	myfree(b);
	printf("Start '%ld %c'\n", *((unsigned long *)mybuffer), *((char *)(mybuffer + sizeof(unsigned long))));

	void *c = myalloc(2000);
	printf("%ld\n", *(unsigned long *)(c - ALLOCATED_FLAG_SIZE - sizeof(unsigned long)));
	printf("%c\n", *((char *)(c - ALLOCATED_FLAG_SIZE)));
	printf("%ld\n", *(unsigned long *)(mybuffer + last));

	void *d = myalloc(10);
	printf("%ld\n", *(unsigned long *)(d - ALLOCATED_FLAG_SIZE - sizeof(unsigned long)));
	printf("%c\n", *((char *)(d - ALLOCATED_FLAG_SIZE)));
	printf("%ld\n", *(unsigned long *)(mybuffer + last));

	return 0;
}
