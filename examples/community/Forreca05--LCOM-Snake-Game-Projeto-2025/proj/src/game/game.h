/**
 * @file game.h
 * @brief Declares the main game loop functions for different game modes.
 *
 * This header provides function prototypes for running the game in singleplayer,
 * multiplayer, classic, and local multiplayer modes.
 */
#ifndef RUN_GAME_H
#define RUN_GAME_H

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdlib.h>
#include "../controllers/KBC/i8042.h"
#include "../controllers/KBC/keyboard/keyboard.h"
#include "../controllers/KBC/mouse/mouse.h"
#include "../controllers/gpu/gpu.h"
#include "../controllers/gpu/sprites/sprite.h"
#include "elements/snake.h"
#include "elements/freesnake.h"
#include "elements/apple.h"
#include "score.h"
#include "UTILS.h"
#include "../input/input.h"

/**
 * @brief Runs the multiplayer game loop.
 * @param option Game mode option.
 * @param isRemote True if playing remote multiplayer, false for local.
 * @return Final game state.
 */
int run_game_multiplayer(int option, bool isRemote);

/**
 * @brief Runs the main game loop for singleplayer or IO mode.
 * @param option Game mode option.
 * @return Final game state.
 */
int run_game(int option);

/**
 * @brief Runs the classic mode game loop.
 * @param option Game mode option.
 * @return Final game state.
 */
int run_game_classic(int option);

/**
 * @brief Runs the local multiplayer game loop.
 * @param option Game mode option.
 * @return Final game state.
 */
int run_game_multiplayer_local(int option);

#endif
