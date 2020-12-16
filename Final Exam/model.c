#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>

typedef struct ModelData {
    float mean;
    int mode;
    int min;
    int max;
} ModelData;

ModelData modelData;

void updateModelData(int values[20] , ModelData* m){
	m->mean = 0;
	m->min = 1000;
	m->max = -1000;
	m->mode = 0;
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
}

void displayModelData(ModelData* m){
	printf("Mean:\t%f\nMode:\t%d\nMin:\t%d\nMax:\t%d\n", m->mean, m->mode, m->min, m->max);
}

void model(pid_t view, pid_t controller){
	printf("view process id: %d\n", view);
	printf("controller process id: %d\n", controller);
	
	int buffer[20] = {12, 11, 10, 12, 12, 12,11, 6, 7, 2, 1, 3, 4,5,6,7,8,9,2 , 20};
	
	updateModelData(buffer, &modelData);
	displayModelData(&modelData);
}

void view(){
	printf("viewing");
}

void controller(){
	printf("controlling");
}

int main(void){
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
