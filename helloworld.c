#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

int SharedVariable = 0;
pthread_mutex_t lock;
pthread_barrier_t barr;


void *SimpleThread(void *numy){
	int *which;
	which = numy;
	int num;
	for(num =0; num<20; num++){
		if(random() > RAND_MAX/2)
			usleep(10);
#ifdef PTHREAD_SYNC		
		pthread_mutex_lock(&lock);
#endif
		printf("*** thread %d sees value %d\n", *which, SharedVariable);
		SharedVariable++;
#ifdef PTHREAD_SYNC
		pthread_mutex_unlock(&lock);
#endif

	}
	
	pthread_barrier_wait(&barr);
	printf("Thread %d sees final value %d\n", *which, SharedVariable);	
}

int main(int argc, char *argv[] )
{
	pthread_t *threadId;
	int i;
	if(argc != 2){
		printf("Too many or too little args, must have one arg\n");
		return 0;
	}
	else{
		int numThreads = atoi(argv[1]);
		threadId=(pthread_t *)malloc(sizeof(pthread_t) * atoi(argv[1]));
		int *j= (int *)malloc(sizeof(int)*numThreads);
		pthread_mutex_init(&lock, NULL);
		pthread_barrier_init(&barr, NULL, numThreads);
		for(i=0;i<numThreads;i++){
			j[i] = i;
		}
	
		for(i=0;i<numThreads;i++){
			pthread_create(&threadId[i], NULL, SimpleThread, &j[i]);
		}
		for(i=0;i<numThreads;i++){
			pthread_join(threadId[i], NULL);
			pthread_mutex_destroy(&lock);
		}
	}
	return 0;
}

