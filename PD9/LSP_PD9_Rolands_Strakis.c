#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h> 


void *myThreadFun(void *vargp) 
{ 
	int i;
    for(i = 0; i < (*(int *)(vargp + 1)); ++i)
    {
    	printf("%c", *((char *)vargp));
    }
    
    return NULL; 
} 
   
int main(int argc, char *argv[]) 
{
    pthread_t thread_id; 
    time_t t;
	srand((unsigned) time(&t));
	char j = rand() % 50 + 50;
	char *args;
    int i;
    for(i = 0; i < atoi(argv[1]); i++){
    	args = malloc(2);
		args[0] = ++j;
		args[1] = atoi(argv[2]);
	    pthread_create(&thread_id, NULL, myThreadFun, args); 
    }
    pthread_exit(NULL);
    exit(0); 
}

/* -lpthread */