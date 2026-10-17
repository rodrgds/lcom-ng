#ifndef _WINSCREENVIEW_H_
#define _WINSCREENVIEW_H_

#include "../../Model/Sprite/sprite.h"
#include "../../Model/XPMs/winScreen.xpm"
#include "../../Model/Background/background.h"
#include "../../Model/SelectorMine/selectorMine.h"

#define TIME_X 343
#define TIME_Y 260
#define PLAY_AGAIN_X 224
#define PLAY_AGAIN_Y 360
#define EXIT_WIN_X 340
#define EXIT_WIN_Y 460 

Sprite* win_background;
Sprite* play_again;
Sprite* time;
Sprite* exit_win;

void loadWinScreenSprites();
void setWinScreenBackgorund();
void drawSMine(SMine mine);

#endif