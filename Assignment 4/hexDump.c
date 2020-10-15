#include "hexDump.h"
#include <stdio.h>
#include <string.h>

void hexDump(char* filePath, long unsigned int numBytes, int printASCII){
	printf("path:\t\t%s\nnumBytes:\t%lu\nprintASCII:\t%s\n\n", filePath, numBytes, printASCII ? "true" : "false");
	
	FILE *fp = fopen(filePath, "r");
	if (fp == NULL) return;
	
	char lineChars[17] ;
	lineChars[16] = '\0';
	long unsigned int i;
	
	for(i = 0; i < numBytes; i++){
		char c = fgetc(fp);
		if (c == EOF) break;
		
		if (i % 16 == 0){
			if(printASCII && i){
				printf("\t|\t%s", lineChars);
			}
			printf("\n%08lX:\t", i);
		}
		lineChars[i % 16] = (c >= ' ' && c<= '~') ? c : '.';
		printf(" %02X", c & 0xff);
	}
	
	int validChars = i % 16;
	while(i++ % 16 != 0) printf("   ");
	if(printASCII) printf("\t|\t%.*s", validChars, lineChars);
	
	printf("\n\n");
	fclose(fp);
}
