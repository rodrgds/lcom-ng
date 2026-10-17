#ifndef _GAMEOVERVIEW_H_
#define _GAMEOVERVIEW_H_

#include "../../Model/Sprite/sprite.h"
#include "../../Model/XPMs/gameOver.xpm"
#include "../../Model/Background/background.h"
#include "../../Model/SelectorMine/selectorMine.h"

#define RETRY_X 303
#define RETRY_Y 250
#define OVER_EXIT_X 340
#define OVER_EXIT_Y 450 

Sprite* gover_background;
Sprite* retry;
Sprite* exit_;

void loadGameOverSprites();
void setGameOverBackgorund();


#endif
