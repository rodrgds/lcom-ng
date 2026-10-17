/**
 * @file game.h
 * @brief Core game logic, entity definitions, and main game loop.
 */

#pragma once
#include <lcom/lcf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hud.h"
#include "gamestate.h"
#include "graphics.h"
#include "constants.h"
#include "keyboard.h"
#include "mouse.h"
#include "sprites.h"

/**
 * @brief Main game loop.
 * @param game Pointer to the Game struct.
 * @param kb_mask Keyboard interrupt mask.
 * @param timer_mask Timer interrupt mask.
 * @param mouse_mask Mouse interrupt mask.
 * @return 0 on success, 1 on error.
 */
int run(Game *game, uint8_t kb_mask, uint8_t timer_mask, uint8_t mouse_mask);

/**
 * @brief Initializes game state and resources.
 * @return Void.
 */
void init_game(void);

/**
 * @brief Fires a bullet towards the given cursor position.
 * @param cursor_pos Target position.
 * @return Void.
 */
void fire_bullet(Vec_pos cursor_pos);

/**
 * @brief Spawns a new enemy if possible.
 * @return Void.
 */
void spawn_enemy(void);

/**
 * @brief Updates all game entities (player, enemies, bullets).
 * @return Void.
 */
void update_entities(void);

/**
 * @brief Draws the current game state to the screen.
 * @return Void.
 */
void draw_game(void);

/**
 * @brief Checks for and handles collisions between entities.
 * @return Void.
 */
void check_collisions(void);

/**
 * @brief Handles keyboard input.
 * @param scanMove Scancode of the pressed key.
 * @return Void.
 */
void handle_input(uint8_t scanMove);

/**
 * @brief Draws the crosshair at the given cursor position.
 * @param cursor Cursor position.
 * @return Void.
 */
void draw_crosshair(Vec_pos cursor);

/**
 * @brief Fast inverse square root (Quake 3 algorithm).
 * @param n Input value.
 * @return Approximate square root.
 */
float quake_sqrt(float n);

/**
 * @brief Runs the pause menu.
 * @param game Pointer to the Game struct.
 * @param kb_mask Keyboard interrupt mask.
 * @param mouse_mask Mouse interrupt mask.
 * @return Void.
 */
void run_pause_menu(Game *game, uint8_t kb_mask, uint8_t mouse_mask);
