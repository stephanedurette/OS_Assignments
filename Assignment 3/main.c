#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//https://man7.org/linux/man-pages/man1/hexdump.1.html

void HexDump(char* filePath, long int maxBytes){

}

void main(int argc, char **argv){
	char path[] = "/dev/urandom";
	int numBytes = 160;

	char cmdString[256];
	sprintf(cmdString, "hexdump -C -n %d %s ", numBytes,path);
	system(cmdString);
}

