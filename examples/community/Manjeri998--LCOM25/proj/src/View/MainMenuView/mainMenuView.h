#ifndef _INSTRUCTIONSVIEW_H_
#define _INSTRUCTIONSVIEW_H_

#include "../../Model/Sprite/sprite.h"
#include "../../Model/XPMs/mainMenu.xpm"
#include "../../Model/Background/background.h"
#include "../../Model/SelectorMine/selectorMine.h"

#define PLAY_X 343
#define PLAY_Y 260
#define INST_X 224
#define INST_Y 360
#define EXIT_X 340
#define EXIT_Y 460 

Sprite* mm_background;
Sprite* play;
Sprite* instructions;
Sprite* exit_;

void loadMainMenuSprites();
void setMainMenuBackgorund();
void drawSMine(SMine mine);

#endif
