#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int subjectData = -1;
pthread_t subjectThread;
pthread_mutex_t lock;

void* subjectFunction(void *arg){
	while(1){
		int randNum = rand() % 1000 + 1;
		if (num % 5 != 0) continue;
		
		pthread_mutex_lock(&lock);
		
		subjectData = randNum;
		
		pthread_mutex_unlock(&lock);
	}
}

void main(int argc, char **argv){
	srand(time(NULL));
	
	if (pthread_mutex_init(&lock, NULL) != 0){
		printf("mutex init failed");
		return 1;
	}
	
	if(pthread_create(&subjectThread, NULL, &subjectFunction, NULL) == 0) return 2;

	
}

}



