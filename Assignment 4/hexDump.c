#include "hexDump.h"
#include <stdio.h>

void hexDump(char* filePath, long unsigned int numBytes, int printASCII){
	printf("path:\t%s\nnumBytes:\t%lu\nprintASCII:\t%d\n", filePath, numBytes, printASCII);
}
