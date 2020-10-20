#!/bin/bash

cat > Q2.c << EOF #Create the c program

#include <string.h>
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

	multiplier = atoi(argv[1]);
	fname = argv[2];


	fp = fopen(fname, "w");
	if (fp == NULL){
		//Invalid file name!!
		return 2;
	}

	for (x = 1; x <= 10; x++) {
		fprintf(fp, "%4d x %2d = %d\n", multiplier, x, multiplier * x);
	}

	fclose(fp);
	return 0;
}

EOF

gcc Q2.c -o Q2 #Create the exe

export MULTIPLIER
export FILENAME

while true
do
	clear
	
	echo "Enter command in the form of [MULTIPLIER] [FILENAME]"
	read MULTIPLIER FILENAME

	./Q2 $MULTIPLIER $FILENAME          #execute Q2
	
	#handle error codes
	if [ $? -eq 0 ]; then
  		echo "Script executed successfully"
  		echo ""
	elif [ $? -eq 1 ]; then
  		echo "Invalid argument count"
  		echo "Please use the following format ex: with multiplier of 3 and filename of f.txt"
  		echo "3 f.txt"
  		echo ""
  		echo "Press any key to exit"
  		read
  		exit 1
	elif [ $? -eq 2 ]; then
  		echo "Invalid file name"
  		echo ""
  		echo "Press any key to exit"
  		read
  		exit 2
	fi

	cat $FILENAME #print contents of $FILENAME to the console
	rm $FILENAME  #delete $FILENAME
	
	#allow the user to exit
	echo ""
	echo "Type 'quit' to exit or press [return] to continue"
	read userInput
	if [ "$userInput" == "quit" ]; then
   		exit 0
   	fi
	
done

