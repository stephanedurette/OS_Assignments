
#include <stdio.h>

void printFibSequence(int count){
	if (count < 1) return;
	int terms[count];
	for(int i = 0; i < count; i++){
		terms[i] = (i < 2) ? 1 : terms[i - 1] + terms[i - 2];
		printf("fib %d:\t%d\n", i + 1, terms[i]);
	}
}

void main(){
	printFibSequence(10);
}

