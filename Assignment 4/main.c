#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexDump.h"
#include "utils.h"

#define FOUR_GIGS 4294967296UL
#define DEFAULT_PATH "/dev/urandom"

void main(int argc, char **argv){
	char* path;
	long unsigned int numBytes;

	switch(argc){
		case 1:
			hexDump(DEFAULT_PATH, 160, 0);
		break;
		case 2:
			if (!strcmp(argv[1], "-h")){
				hexDumpHelp();
			} else if(!strcmp(argv[1], "-d")){
				hexDump(DEFAULT_PATH, 160, 1);
			} else {
				numBytes = cfileexists(argv[1]) ? FOUR_GIGS : 160;
				path = cfileexists(argv[1]) ? argv[1] : DEFAULT_PATH;
				hexDump(path, numBytes, 0);
			}
		break;
		default:
			if (!strcmp(argv[1], "-d")){
				numBytes = cfileexists(argv[2]) ? FOUR_GIGS : 160;
				path = cfileexists(argv[2]) ? argv[2] : DEFAULT_PATH;
				hexDump(path, numBytes, 1);
			} else {
				printf("%s", "\nIncorrect format, type [./myprog -h] for usage options\n");
			}
		
		break;
	}
}



