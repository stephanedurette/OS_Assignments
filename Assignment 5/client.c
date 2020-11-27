#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<fcntl.h>
#include<signal.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x1234
#define SEM_NAME "/semaphore"

int running = 1;

struct shmseg {
   char buf[BUF_SIZE];
};

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("received SIGINT\n");
    system("killall server");
    running = 0;
}

void DisplayData(char buffer[BUF_SIZE], sem_t* semaphore){
	if (signal(SIGINT, sig_handler) == SIG_ERR)
  		printf("\ncan't catch SIGINT\n");

	//get the buffer
	char workingBuffer[BUF_SIZE];
	sem_wait(semaphore); //start crit reg
	strcpy(workingBuffer, buffer);
	sem_post(semaphore); //end crit reg
	
	//get char frequencies
	int bins[(int)'T' + 1];
	for(int i = 'A'; i <= 'T'; i++) bins[i] = 0;
	for(int i = 0; i < BUF_SIZE; i++){
		bins[workingBuffer[i]] += 1;
	}
	
	//display frequencies
	system("@cls||clear");
	int asteriskValue = 8;
	int assumedMaxValue = 100;
	printf("assuming no value will go above %d occurences (average distribution)\n", assumedMaxValue);
	printf("* = %d occurences\n\t", asteriskValue);
	for(int i = 'A'; i <= 'T'; i++) printf("__");
	printf("\n");
	
	for(int j = BUF_SIZE / asteriskValue; j > 0; j--){
		if (j > assumedMaxValue / asteriskValue) continue;
		printf(">%03d |  ", (j - 1) * asteriskValue);
		for(int i = 'A'; i <= 'T'; i++) printf("%c ", bins[i] >= (j - 1) * asteriskValue ? '*' : ' ');
		printf("\n");	
	}
	
	printf("\t");
	for(int i = 'A'; i <= 'T'; i++) printf("__");
	printf("\n\t");
	for(int i = 'A'; i <= 'T'; i++) printf("%c ", i);
	printf("\n");
}

int main(int argc, char *argv[]) {
   
   //init shared memory
   struct shmseg *shmp;
   int shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
   
   //try to connect, launch server and retry, then exit
   if (shmid == -1) {
	system("./server");
    	sleep(1);
    	shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
    	
    	if(shmid == -1){
    		printf("client/server suite not working");
    		return 1;
    	}
   }
   
   //init semaphore
   sem_t *semaphore = sem_open(SEM_NAME, O_RDWR);
    if (semaphore == SEM_FAILED) {
        perror("sem_open(3) failed");
        exit(EXIT_FAILURE);
    }
   
   // Attach to the segment to get a pointer to it.
   shmp = shmat(shmid, NULL, 0);
   if (shmp == (void *) -1) {
      perror("Shared memory attach");
      return 1;
   }
   
   //main loop
   while(running){
   	DisplayData(shmp->buf, semaphore);
   	sleep(2); //for sanity
   }
   
   //close semaphore
   if (sem_close(semaphore) < 0)
        perror("sem_close(3) failed");
   
   //Detach shared memory
   if (shmdt(shmp) == -1) {
      perror("shmdt");
      return 1;
   }
   return 0;
}
