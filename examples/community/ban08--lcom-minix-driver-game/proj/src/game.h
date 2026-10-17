#ifndef _GAME_H_
#define _GAME_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

/**
 * @brief Starts and runs one frame of the game loop.
 *
 * This function handles:
 * - Drawing menu or game screen based on current state.
 * - Initializing the environment and background.
 * - Managing vertical movement (jumping/falling).
 * - Drawing the player and enemies.
 * - Scrolling window logic.
 * - Collision detection.
 */
void game_start();

/**
 * @brief Draws all visible enemies within the current scrolling window.
 *
 * Also handles collision detection between enemies and the player.
 * If a collision is detected, the game returns to the menu screen.
 */
void draw_enemies();

/**
 * @brief Draws the menu screen, including "MENU", "DINO" title, and options for play and exit.
 *
 * This function uses pixel/rectangle primitives to render letters and buttons manually.
 */
void draw_menu_screen();

/**
 * @brief Handles player vertical movement during jumping or falling.
 *
 * This function modifies the `y_position` based on whether the player is going up or falling down.
 * - Jumping stops at a maximum height.
 * - Falling stops when reaching the ground level.
 */
void game_jump();

#endif
