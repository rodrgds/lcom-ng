#pragma once

#include <lcom/lcf.h>
#include "../controllers/video/video.h"
#include "../controllers/kbc/mouse.h"
#include "../controllers/kbc/kbd.h"
#include "cursor/cursor.h"
#include "player/player.h"
#include "alien/alien.h"
#include "barrier/barrier.h"
#include "projectiles/projectile.h"
#include "entry/entry.h"
#include "hud/hud.h"

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_GAME_OVER,
    EXIT_GAME
} GameState;

/**
 * @brief Main game loop that handles all game logic, rendering, and input processing.
 *
 * This function initializes the game, subscribes to necessary interrupts, and enters the main event loop.
 * It processes timer, keyboard, and mouse events, updates the game state, and manages transitions between
 * different game states (menu, playing, game over, exit).
 *
 * @return 0 on success, non-zero on failure.
 */
int game_loop();
void game_update();
void game_render();
