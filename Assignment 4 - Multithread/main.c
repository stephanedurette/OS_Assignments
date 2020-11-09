#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include<unistd.h>

pthread_t subjectThread;
static int subjectData = -1;

pthread_t observerThreads[4];
int observerDivisors[4] = {3, 5, 7, 25};

pthread_mutex_t lock;

void *subjectFunction(void *arg){
	srand(time(NULL));
	int count = 0;
	while(count++ < 100000000){
		int randNum = rand() % 1000 + 1;
		if (randNum % 5 != 0) continue;
		
		pthread_mutex_lock(&lock);
		
		subjectData = randNum;
		
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

void *observerFunction(void *arg){
	int divisor = *(int*)arg;
	int count = 0;
	while(count < 3){
		if (subjectData % divisor == 0){
			printf("The number %d is divisible by %d\n", subjectData, divisor);
			count++;
		}
	}
	return NULL;
}

int main(int argc, char **argv){
	
	if (pthread_mutex_init(&lock, NULL) != 0){
		printf("mutex init failed\n");
		return 1;
	}
	
	
	if(pthread_create(&subjectThread, NULL, &subjectFunction, NULL) != 0){
		printf("subject thread creation failed\n");
		return 2;
	}
	
	for(int i = 0; i < 4; i++){
		if(pthread_create(&(observerThreads[i]), NULL, &observerFunction, &observerDivisors[i]) != 0){
			printf("observer thread creation failed\n");
			return 2;
		}
	}
	
	sleep(1);
	return 0;
}



