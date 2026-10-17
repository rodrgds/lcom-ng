#ifndef __SCORES_H_
#define __SCORES_H_

#include "RTC.h"

typedef struct {
	Date date;
	char name[16];
	int score;
} Highscore;


void SaveScores();
void AddScore(Highscore h);
Highscore getHighscore();
void LoadScores();
void DrawScores(int x, int y);

#endif
