#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#include "src/include.h"
#include "CursorController.h"
#include "PlayerController.h"
#include "src/GUI/gui.h"
#include "MapController.h"
#include "src/Controller/Game/BackpackController.h"

/**
 * @file GameController.h
 * @brief Declares the core game control logic and global game context.
 *
 * Handles initialization, updating and rendering for all game states.
 * It coordinates the cursor, player, map, and manages transitions between game phases.
 */

typedef struct {
    Cursor *cursor;
} GameContext;

static GameContext ctx;
extern GameState gamestate;

struct sprite_t;
void update_menu(void);
void draw_menu(void);

void gamecontroller_init();
void gamecontroller_exit();
Cursor *get_cursor();
Player *get_player();

extern bool waiting_mouse;
extern bool waiting_keyboard;
extern uint8_t scan_code[2];
extern uint8_t size;
void update_game();
void draw_menu();
void draw_game();
void change_video_mode(uint16_t mode);

  void update_Keyboard();
  void update_Mouse();
  void update_Timer();

#endif
