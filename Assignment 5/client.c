/* Filename: shm_read.c */
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x1234
#define SEM_NAME "/semaphore"

struct shmseg {
   char buf[BUF_SIZE];
};

void DisplayData(char buffer[BUF_SIZE], sem_t* semaphore){
	sem_wait(semaphore);
	printf("%s\n", buffer);
	sem_post(semaphore);
	sleep(2);
}

int main(int argc, char *argv[]) {
   
   //init shared memory
   struct shmseg *shmp;
   int shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
   if (shmid == -1) {
      perror("Shared memory");
      return 1;
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
   
   while(1){
   	DisplayData(shmp->buf, semaphore);
   	sleep(2);
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
