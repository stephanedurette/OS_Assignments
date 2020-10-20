#!/bin/bash

cat > Q2.c << EOF #create Q2.c  and enter in the following text until the next EOF

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
	FILE *fp;
	int x;
	int multiplier;
	char *fname;

	if (argc != 3) {
		// need a multiplier a file name to invoke this program properly!
		return 1;
	}

	multiplier = atoi (argv[1]);
	fname = argv[2];


	fp = fopen (fname, “w”);
	if (fp == NULL){
		//Invalid file name!!
		return 2;
	}

	for (x = 1; x <= 10; x++) {
		fprintf (fp, “%4d x %2d = %d\n”, multiplier, x, multiplier * x);
	}

	fclose(fp);
	return 0;
}


EOF
gcc Q2.c -o Q2 #compile the new .c file into an exe named Q2.c 
./Q2              #execute Q2
