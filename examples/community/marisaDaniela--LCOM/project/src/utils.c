#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

int fileExists(const char* filename) {
	FILE* file = fopen(filename, "r");

	if(file){
		fclose(file);
		return 1;
	}
	return 0;
}

const char* path(const char* name)
{
    // Creates and writes the pathname to reader
    char reader[256];
    sprintf(reader, "/home/lcom/lcom1718-t1g04/project/res/img/%s.bmp", name);

    // Creates pathname and copies the reader into it
    char* pathname = (char*) malloc(256);
    strcpy(pathname, reader);

    // Returns the pathname
    return pathname;
}

void writeScore(FILE* fp, int score)
{
	fprintf(fp, " score: %d \n", score);
		
}
