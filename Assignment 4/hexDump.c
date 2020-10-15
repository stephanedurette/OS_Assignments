#include "hexDump.h"
#include <stdio.h>

void hexDump(char* filePath, long unsigned int numBytes, int printASCII){
	printf("path:\t%s\nnumBytes:\t%lu\nprintASCII:\t%d\n\n", filePath, numBytes, printASCII);
	
	FILE *fp = fopen(filePath, "r");
	if (fp == NULL) return;
	
	for(long unsigned int i = 0; i < numBytes; i++){
		char c = fgetc(fp);
		if (c == EOF) break;
		
		if (i % 16 == 0){
			printf("\n%ld:\t", i);
		}
		
		printf(" %x", fgetc(fp) & 0xff);
	}
	
	fclose(fp);
}
