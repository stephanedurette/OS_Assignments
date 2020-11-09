#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include<unistd.h>

static int subjectData = -1;
pthread_t tid[1];
pthread_mutex_t lock;

void *subjectFunction(void *arg){
	srand(time(NULL));
	
	int i = 0;
	while(i++ < 10000){
		int randNum = rand() % 1000 + 1;
		if (randNum % 5 != 0) continue;
		
		pthread_mutex_lock(&lock);
		
		subjectData = randNum;
		printf("The number is: %d\n", subjectData);
		
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

int main(int argc, char **argv){
	
	if (pthread_mutex_init(&lock, NULL) != 0){
		printf("mutex init failed\n");
		return 1;
	}
	
	
	if(pthread_create(&(tid[0]), NULL, &subjectFunction, NULL) != 0){
		printf("subject thread creation failed\n");
		return 2;
	}
	
	sleep(1);
	return 0;
}



