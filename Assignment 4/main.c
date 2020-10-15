#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexDump.h"
#include "utils.h"

#define FOUR_GIGS 4294967296
#define DEFAULT_PATH "/dev/urandom"

void main(int argc, char **argv){
	char path[256];
	long int numBytes;

	switch(argc){
		case 1:
			numBytes = 160;
			hexDump(DEFAULT_PATH, 160, 0);
		break;
		case 2:
			int fileExists = cfileexits(argv[1]);
			numBytes = fileExists ? FOUR_GIGS : 160;
			path = fileExists ? argv[1] : DEFAULT_PATH;
			hexDump(path, numBytes, 0);
		break;
		default:
		
		
		break;
	}
}



