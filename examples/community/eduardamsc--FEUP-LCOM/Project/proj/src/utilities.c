#include "utilities.h"
#include <minix/drivers.h>
#include <minix/driver.h>

#include <stdio.h>

#define GFXMODE "0x114"

int fileExists(const char* filename) {
	FILE* file = fopen(filename, "r");

	if (file) {
		fclose(file);
		return 0;
	}

	return 1;
}

const char* getImagePath(const char* image){
	char num[256];
	sprintf(num, "/home/proj/res/images/%s.bmp", image, GFXMODE);

	char* str = (char*) malloc(256);
	strcpy(str, num);

	return str;
}
