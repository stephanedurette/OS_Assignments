#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int randomNumber = -1;
pthread_t subjectThread;
pthread_mutex_t lock;

void* subjectFunction(void *arg){
	while(1){
		randomNumber = rand() % 1000 + 1;
	}
}

void main(int argc, char **argv){
	srand(time(NULL));
	
	if (pthread_mutex_init(&lock, NULL) != 0){
		printf("mutex init failed");
	return 1;
}

}



