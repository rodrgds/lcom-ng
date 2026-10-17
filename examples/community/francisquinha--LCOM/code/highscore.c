/** @file highscore.c
 * @brief file containing the definitions of the functions that handle the game highscores
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

#include "highscore.h"

#define DEBUG_MALLOC 0

HighScore *create_highscore() {
	HighScore *highscore = (HighScore *)malloc(sizeof(HighScore));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for highscore\n", sizeof(HighScore));
	if (highscore == NULL)
		return NULL;
	unsigned short i;
	for (i = 0; i < 5; i++ )
		highscore->name[i] = 0;
	if ((highscore->datetime = create_datetime()) == NULL) {
		free(highscore);
		if (DEBUG_MALLOC) printf("Freed highscore\n");
		printf("create_highscore: error creating datetime!\n");
		return NULL;
	}
	highscore->score = 0;
	return highscore;
}

void destroy_highscore(HighScore *highscore) {
    free(highscore->datetime);
	if (DEBUG_MALLOC) printf("Freed datetime\n");
    free(highscore);
	if (DEBUG_MALLOC) printf("Freed highscore\n");
}

void load_highscores(HighScore *highscores[10]) {
	FILE *file;
	file = fopen("/home/lcom/proj/files/highscores.txt", "r");
	unsigned short i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 5; j++ )
			fscanf(file, "%hu ", &highscores[i]->name[j]);
		fscanf(file, "%hu ", &highscores[i]->datetime->seconds);
		fscanf(file, "%hu ", &highscores[i]->datetime->minutes);
		fscanf(file, "%hu ", &highscores[i]->datetime->hours);
		fscanf(file, "%hu ", &highscores[i]->datetime->day);
		fscanf(file, "%hu ", &highscores[i]->datetime->month);
		fscanf(file, "%hu ", &highscores[i]->datetime->year);
		fscanf(file, "%u\n", &highscores[i]->score);
	}
	fclose(file);
}

void save_highscores(HighScore *highscores[10]) {
	FILE *file;
	file = fopen("/home/lcom/proj/files/highscores.txt", "w");
	unsigned short i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 5; j++ )
			fprintf(file, "%hu ", highscores[i]->name[j]);
		fprintf(file, "%hu ", highscores[i]->datetime->seconds);
		fprintf(file, "%hu ", highscores[i]->datetime->minutes);
		fprintf(file, "%hu ", highscores[i]->datetime->hours);
		fprintf(file, "%hu ", highscores[i]->datetime->day);
		fprintf(file, "%hu ", highscores[i]->datetime->month);
		fprintf(file, "%hu ", highscores[i]->datetime->year);
		fprintf(file, "%u\n", highscores[i]->score);
	}
	fclose(file);
}

void update_highscores(HighScore *highscores[10], unsigned int new_score, unsigned short new_name[5], DateTime *new_datetime) {
	unsigned short i, j, k;
	for (i = 0; i < 10; i++) {
		if (new_score > highscores[i]->score)
			break;
	}
	if (i == 10) return;
	for (j = 9; j > i; j--) {
		for (k = 0; k < 5; k++)
			highscores[j]->name[k] = highscores[j - 1]->name[k];
		highscores[j]->datetime = highscores[j - 1]->datetime;
		highscores[j]->score = highscores[j - 1]->score;
	}
	for (k = 0; k < 5; k++)
		highscores[i]->name[k] = new_name[k];
	highscores[i]->datetime = new_datetime;
	highscores[i]->score = new_score;
}
