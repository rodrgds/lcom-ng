#ifndef _MENU_H
#define _MENU_H

#include "../controller/mouse.h"

void KB_MenuControls(uint8_t scancode);

int processStartGameButton(uint16_t x, uint16_t y);

int processSettingsButton(uint16_t x, uint16_t y);

int processLeaderboardButton(uint16_t x, uint16_t y);

int processExitGameButton(uint16_t x, uint16_t y);

int drawMenu();

#endif