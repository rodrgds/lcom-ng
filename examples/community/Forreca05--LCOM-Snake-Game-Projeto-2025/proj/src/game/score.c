#include "score.h"
#include <stdio.h>
#include "../assets/xpm/all.xpm"

static void intToString(int score, char* scoreStr){
    snprintf(scoreStr, MAX_DIGIT_COUNT+1, "%d", score);
}

void drawScore(Score* self) {
    char scoreStr[MAX_DIGIT_COUNT+1];
    intToString(self->score,scoreStr);
    self->scoreSprite->updateText(self->scoreSprite,scoreStr);
    
    self->scoreSprite->draw(self->scoreSprite);
}

static void destroyScore(Score* self) {
    self->scoreSprite->destroy(self->scoreSprite);
    free(self);
}


Score* createScore(int score,int x, int y) {
    char scoreStr[MAX_DIGIT_COUNT+1];
    intToString(score,scoreStr);
    Text* scoreSprite=create_Text(scoreStr,x,y,0);
    if(!scoreSprite) return NULL;

    Score* self = malloc(sizeof(Score));
     if (!self) {
        scoreSprite->destroy(scoreSprite);
        return NULL;
    }

    self->scoreSprite=scoreSprite;
    self->score = score;

    self->draw = drawScore;
    self->destroy = destroyScore;
    return self;
}

