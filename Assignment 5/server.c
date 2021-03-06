#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<semaphore.h>
#include <fcntl.h>
#include<signal.h>

#define BUF_SIZE 1024
#define NUM_BYTE_WAIT 32
#define SLEEP_TIME_MICROSECONDS 250000
#define SHM_KEY 0x1234

#define SEM_NAME "/semaphore"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 1

struct shmSeg {
   char buf[BUF_SIZE];
};

int running = 1;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("received SIGINT\n");
    running = 0;
}

//fill ring buffer with random chars while running
void fillBuffer(char* buffer, sem_t *semaphore){
	system("@cls||clear");
	printf("-- Ring Buffer Contents --\n\nnothing yet\n\n");
   
	int i = 0;
	
	while(running){
		sem_wait(semaphore);
		buffer[i % BUF_SIZE] = rand() % ('T' - 'A' + 1) + 'A';
		sem_post(semaphore);
		
		if(i % NUM_BYTE_WAIT == 0 && i > 0){
			usleep(SLEEP_TIME_MICROSECONDS);
			system("@cls||clear");
			printf("-- Ring Buffer Contents --\n\n%s\n\n", buffer);
		}
		
		i++;	
	}
}

int main(int argc, char *argv[]) {
   //handle signals
   if (signal(SIGINT, sig_handler) == SIG_ERR)
  		printf("\ncan't catch SIGINT\n");

   struct shmSeg *segPtr;
   
   //init semaphore
   sem_t *semaphore = sem_open(SEM_NAME, O_CREAT, SEM_PERMS, INITIAL_VALUE);
   if (semaphore == SEM_FAILED) {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }
   
   //validate shared memory
   int shmid = shmget(SHM_KEY, sizeof(struct shmSeg), 0644|IPC_CREAT);
   if (shmid == -1) {
      perror("Shared memory");
      return 1;
   }
   
   // Attach to the segment to get a pointer to it.
   segPtr = shmat(shmid, NULL, 0);
   if (segPtr == (void *) -1) {
      perror("Shared memory attach");
      printf("blafdfh");
      return 1;
   }
   
   // program loop
   fillBuffer(segPtr->buf, semaphore);
   
   //close semaphore
   if (sem_close(semaphore) < 0) {
        perror("sem_close(3) failed");
        /* We ignore possible sem_unlink(3) errors here */
        sem_unlink(SEM_NAME);
        //exit(EXIT_FAILURE);
    }
   
   //detach shared memory
   if (shmdt(segPtr) == -1) {
      perror("shmdt");
      return 1;
   }
   
   //close shared memory
   if (shmctl(shmid, IPC_RMID, 0) == -1) {
      perror("shmctl");
      return 1;
   }
   
   return 0;
}
