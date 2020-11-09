#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_t subjectThread;
static int subjectData = -1;

pthread_t observerThreads[4];
int observerDivisors[4] = {3, 5, 7, 25};

pthread_mutex_t lock;

//Run forever, update the global "subjectData" number when the random number is divisible by 5
void *subjectFunction(void *arg){
	srand(time(NULL));
	
	while(1){
		int randNum = rand() % 1000 + 1; // 1 <= randNum <= 1000
		if (randNum % 5 != 0) continue;
		
		pthread_mutex_lock(&lock); 	//begin critical region
		
		subjectData = randNum;
		
		pthread_mutex_unlock(&lock);	//end critical region
	}
	return NULL;
}

//Read the subjectData, if its divisible by the assigned divisor output the result. Do this 3 times then terminate 
void *observerFunction(void *arg){
	int divisor = *(int*)arg;
	int count = 0;
	while(count < 3){
		if (subjectData % divisor == 0){
			printf("The number %d is divisible by %d\n", subjectData, divisor);
			count++;
			sleep(1);	//sleep to prevent re-reading the same number 3 times in a row, give time for the subject function to update the value
		}
	}
	return NULL;
}

int main(int argc, char **argv){
	//Init mutex lock
	if (pthread_mutex_init(&lock, NULL) != 0){
		printf("mutex init failed\n");
		return 1;
	}
	
	//Create subject thread -> subjectFunction
	if(pthread_create(&subjectThread, NULL, &subjectFunction, NULL) != 0){
		printf("subject thread creation failed\n");
		return 2;
	}
	
	//Create observer threads -> observerFunction using the divisors as arguments
	for(int i = 0; i < 4; i++){
		if(pthread_create(&(observerThreads[i]), NULL, &observerFunction, &observerDivisors[i]) != 0){
			printf("observer thread creation failed\n");
			return 2;
		}
	}
	
	//Wait until all observer threads have terminated
	for (int i = 0; i < 4; i++) pthread_join(observerThreads[i], NULL);
	
	//Terminate subject thread
	if (pthread_cancel(subjectThread) != 0){
		printf("failed to cancel the subject thread\n");
		return 3;
	}
	
	return 0;
}



