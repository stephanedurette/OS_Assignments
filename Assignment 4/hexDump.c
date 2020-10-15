#include "hexDump.h"
#include <stdio.h>

void hexDump(char* filePath, int numBytes, bool printASCII){
	printf("path:\t%s\nnumBytes:\t%d\nprintASCII\t%d\n", filePath, numBytes, printASCII);
}
