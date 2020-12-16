#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>

void model(pid_t view, pid_t controller){
	printf("view process id: %d\n", view);
	printf("controller process id: %d\n", controller);
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
