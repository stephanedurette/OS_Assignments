#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <string.h>
typedef struct ModelData {
    int data[20];
    float mean;
    int mode;
    int min;
    int max;
    int updated;
} ModelData;

int fd[2];

void updateModelData(int values[20] , ModelData* m){
	m->mean = 0;
	m->min = 1000;
	m->max = -1000;
	m->mode = 0;
	
	for(int i = 0; i < 20; i++) m->data[i] = values[i];
	
	char occurences[2000];
	for(int i = 0; i < 2000; i++) occurences[i] = 0;
	
	for(int i = 0; i < 20; i++){
		int v = values[i];
		m->mean += v / 20.0;
		m->min = v < m->min ? v : m->min;
		m->max = v > m->max ? v : m->max;
		occurences[v + 999] += 1;
	}
	
	int maxOccurence = 0;
	for(int i = 0; i < 2000; i++){
		if (occurences[i] > maxOccurence){
			maxOccurence = occurences[i];
			m->mode = i - 999;
		}
	}
	m->updated = 1;
}

void displayModelData(ModelData* m){
	printf("\n\nMean:\t%f\nMode:\t%d\nMin:\t%d\nMax:\t%d\nBuffer Data:\t[", m->mean, m->mode, m->min, m->max);
	for(int i = 0; i < 20; i++){
		printf("%d ", m->data[i]);
	}
	printf("]\n");
}

void model(pid_t view, pid_t controller){
	//model-view shared memory
	key_t key = ftok("shmfile",65);
	int shmid = shmget(key,1024,0666|IPC_CREAT);
	ModelData* m = (ModelData*) shmat(shmid,(void*)0,0);
	
	printf("view process id: %d\n", view);
	printf("controller process id: %d\n", controller);
	
	int buffer[20]; 
	for(int i = 0; i < 20; i++) buffer[i] = 0;
	
	while(1){
		char buff[512];
		close(fd[1]);
		read(fd[0], buff, 512);
		
	
		updateModelData(buffer, m);
	}
	//detach from shared memory
	shmdt(m);
}

void view(){
	//model-view shared memory
	key_t key = ftok("shmfile",65);
	int shmid = shmget(key,1024,0666|IPC_CREAT);
	ModelData* m = (ModelData*) shmat(shmid,(void*)0,0);
	
	
	while(1){
		if (m->updated == 1){
			displayModelData(m);
			m->updated = 0;
		}
	}
	
	//detach from shared memory
	shmdt(m);
}

void controller(){
	while(1){
		char input[512];
		int value = 1000;
		while (value > 999 || value < -999){
			printf("Enter an integer value between -999 and 999: ");
			scanf("%d", &value) == 1;
		}

		close(fd[0]);
		write(fd[1], input, 512);
	
	}
}

int main(void){
	pipe(fd);
	pid_t view_pid, controller_pid;
	view_pid = fork();
	
	if (view_pid == 0){ //view process
		view();
	} else{
		controller_pid = fork();
		if(controller_pid == 0){ //controller process
			controller();
		} else {
			model(view_pid, controller_pid); //model process
		}
	}
	return 0;
}
