/**
	@file files.c
	@author Daniel Marques
	@author João Almeida

	Declaration of the functions used to read and write to the highscores text file.
	Has the ability to read and store multiple highscores, however in this project
		we have chosen to only store and read 1 highscore 

*/

#include "files.h"
#include <stdio.h>

void readHighScores(unsigned int array[], unsigned int size, char * file_name) {
	//Initialize array as zeros
	unsigned int n = 0;
	while (n != size) {
		array[n] = 0;
		n++;
	}

	FILE * filePtr;
	filePtr = fopen(file_name, "r");
	if (filePtr != NULL) {
		n = 0;
		while (n != size && !feof(filePtr)) {
			unsigned int currentScore;
			fscanf(filePtr, "%u", &currentScore);
			array[n] = currentScore;
			n++;
		}
	}
}

void writeHighScores(unsigned int array[], unsigned int size, char * file_name) {
	FILE * filePtr;
	filePtr = fopen(file_name, "w");
	unsigned int n = 0;
	while (n != size) {
		fprintf(filePtr, "%u\n", array[n]);
		n++;
	}
}

void updateHighScores(unsigned int array[], unsigned int size, unsigned int score) {
	unsigned int n = 0;
	unsigned int overWritten = 0;
	while (n < size) {
		if (score > array[n]) {
			overWritten = array[n];
			array[n] = score;
			n++;
			break;
		}
		n++;
	}
	if (n < size) {
		while (n < size) {
			unsigned int current = array[n];
			array[n] = overWritten;
			overWritten = current;
			n++;
		}
	}
}
