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

int main(void){
	
	return 0;
}
