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

#define BUF_SIZE 1024
#define NUM_BYTE_WAIT 32
#define SLEEP_TIME_MICROSECONDS 250000
#define SHM_KEY 0x1234

struct shmSeg {
   char buf[BUF_SIZE];
};
//int fill_buffer(char * bufptr, int size);

void fillBuffer(char* buffer){
	//srand(time(NULL));
	int i = 0;
	
	while(1){
		buffer[i % BUF_SIZE] = rand() % ('T' - 'A') + 'A';
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
   struct shmSeg *segPtr;
   
   int shmid = shmget(SHM_KEY, sizeof(struct shmSeg), 0644|IPC_CREAT);
   if (shmid == -1) {
      perror("Shared memory");
      return 1;
   }
   
   // Attach to the segment to get a pointer to it.
   segPtr = shmat(shmid, NULL, 0);
   if (segPtr == (void *) -1) {
      perror("Shared memory attach");
      return 1;
   }
   
   fillBuffer(segPtr->buf);
   /* 
   bufptr = shmp->buf;
   spaceavailable = BUF_SIZE;
   for (numtimes = 0; numtimes < 5; numtimes++) {
      shmp->cnt = fill_buffer(bufptr, spaceavailable);
      shmp->complete = 0;
      printf("Writing Process: Shared Memory Write: Wrote %d bytes\n", shmp->cnt);
      bufptr = shmp->buf;
      spaceavailable = BUF_SIZE;
      sleep(3);
   }
   printf("Writing Process: Wrote %d times\n", numtimes);
   */
   
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
/*
int fill_buffer(char * bufptr, int size) {
   static char ch = 'A';
   int filled_count;
   
   //printf("size is %d\n", size);
   memset(bufptr, ch, size - 1);
   bufptr[size-1] = '\0';
   if (ch > 122)
   ch = 65;
   if ( (ch >= 65) && (ch <= 122) ) {
      if ( (ch >= 91) && (ch <= 96) ) {
         ch = 65;
      }
   }
   filled_count = strlen(bufptr);
   
   //printf("buffer count is: %d\n", filled_count);
   //printf("buffer filled is:%s\n", bufptr);
   ch++;
   return filled_count;
}
*/
