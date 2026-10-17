#ifndef _GUI_H_
#define _GUI_H_

#include "src/Controller/Game/GameController.h"
#include "src/Controller/Game/MapController.h"
#include "src/Controller/Game/PlayerController.h"
#include "src/Controller/VBE/GPU.h"
#include "src/GUI/bitmap.h"
#include "src/GUI/gui.h"
#include "src/Model/Elements/BGs.h"
#include "src/Model/Elements/Sprite.h"
#include "src/Model/Elements/player.h"
#include "src/Model/Position.h"
#include <lcom/lcf.h>

/**
 * @file GUI.h
 * @brief Declares functions and global resources related to graphical user interface rendering.
 *
 * Hhandles all GUI rendering, including menu screens, buttons, sprites, 
 * and the in-game HUD (hotbar, cursor, etc.).
 */


extern sprite_t *playButton;
extern sprite_t *optionsButton;
extern sprite_t *exitButton;
extern Position playButtonPosition;
extern Position optionsButtonPosition;
extern Position exitButtonPosition;
void setup_sprites(uint16_t mode);
void draw();
void draw_cursor(Position position);
void draw_game();
void draw_character(Position position);
void draw_hotbar(Backpack *b);
void draw_mainMenu_buttons();
void draw_optionsMenu_buttons();
void draw_pauseMenu_buttons();
void draw_Button(Position position, sprite_t *sprite);
void draw_pause();
void draw_options();
void draw_Map();

extern BG bg;

#endif
