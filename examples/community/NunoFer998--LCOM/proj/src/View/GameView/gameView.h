#ifndef _GAMEVIEW_H_
#define _GAMEVIEW_H_

#include <stdint.h>

#include "../../Model/Sprite/sprite.h"
#include "../../Model/XPMs/game.xpm"
#include "../../Model/XPMs/digits.xpm"
#include "../../Model/XPMs/smallDigits.xpm"
#include "../../Model/Background/background.h"
#include "../../Model/SelectorMine/selectorMine.h" 
#include "../../Controller/GameController/gameController.h"

extern Square gameBoard[15][15];

extern uint8_t flags;
Sprite* game_background;
Sprite* sideBar;
Sprite* flag;
Sprite* mine;
Sprite* small_digits[8];
Sprite* time_digits[10];

Sprite* side_bar;

void drawGameBoardToBackground();
void drawFlag(uint8_t s_col, uint8_t s_row);
void loadGameSprites();
void DrawMine(uint8_t s_col, uint8_t s_row);
void DrawProximityCounter(uint8_t s_col, uint8_t s_row);
void DrawDig(uint8_t s_col, uint8_t s_row);
void DrawFlagCounter();
void drawGameSideBar();
void DrawTime(uint16_t time);

#endif
