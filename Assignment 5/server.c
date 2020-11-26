/* Filename: server.c */
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

//int fill_buffer(char * bufptr, int size);

void fillBuffer(char* buffer, sem_t mutex){
	//srand(time(NULL));
	int i = 0;
	
	while(1){
		sem_wait(&mutex);
	
		buffer[i % BUF_SIZE] = rand() % ('T' - 'A') + 'A';
		
		sem_post(&mutex);
		
		if(i % NUM_BYTE_WAIT == 0){
			usleep(SLEEP_TIME_MICROSECONDS);
		}
		if(i % 1024 == 0 && i > 0){
			printf("Output:\t%s\n\n", buffer);
		}
		i++;	
	}
}

int main(int argc, char *argv[]) {
   //struct shmSeg *segPtr;
   
   struct shmSeg *segPtr = (struct shmSeg*) malloc(sizeof(struct shmSeg));
   
   
   int shmid = shmget(SHM_KEY, sizeof(struct shmSeg), 0644|IPC_CREAT);
   if (shmid == -1) {
      printf("blah");
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
   
   sem_init(&segPtr->mutex, 1, 1); 
   
   fillBuffer(segPtr->buf, segPtr->mutex);
   
   if (shmdt(segPtr) == -1) {
      perror("shmdt");
      return 1;
   }

   if (shmctl(shmid, IPC_RMID, 0) == -1) {
      perror("shmctl");
      return 1;
   }
   
   return 0;
}
