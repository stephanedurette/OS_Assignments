#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexDump.h"
#include "utils.h"

#define FOUR_GIGS 4294967296
#define DEFAULT_PATH "/dev/urandom"

void main(int argc, char **argv){
	char* path;
	long unsigned int numBytes;

	switch(argc){
		case 1:
			numBytes = 160;
			hexDump(DEFAULT_PATH, 160, 0);
		break;
		case 2:
			if (!strcmp(argv[1], "-h")){
				printf("%s", "help!");
			} else {
				numBytes = cfileexists(argv[1]) ? 4294967296 : 160;
				path = cfileexists(argv[1]) ? argv[1] : DEFAULT_PATH;
				hexDump(path, numBytes, 0);
			}
		break;
		default:
		
		
		break;
	}
}



