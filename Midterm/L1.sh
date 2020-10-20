#!/bin/bash

cat > myLab1.c << EOF #create myLab1.c and enter in the following text until the next EOF

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

EOF
gcc myLab1.c -o myLab1 #compile the new .c file into an exe named myLab1
./myLab1               #execute myLab1
