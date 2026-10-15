#ifndef __Score_h
#define __Score_h



#include "Sprite.h"
#include "xpm/numbers.xpm"
#include "xpm/small_numbers.xpm"
#include "xpm/alphabet.xpm"
#include "xpm/slash.xpm"
#include "xpm/hyphen.xpm"
#include "xpm/colon.xpm"

#define MAX_RESULTS 9  // Maximum number of results to store
#define DIST_NUMS 40   // horizontal distance between small numbers drawn on screen
#define ADJUST_NUMS 10 // vertical adjustment in position for small numbers xpm

typedef struct {
    int date[6]; 
    int score;
} GameResult;


void update_score(int s);

void updateGameResults();

void loadScoreSprites();

void drawScoreGameOver(int score);

void drawScoreGameScore();

void drawScoreGamePlay();


#endif





