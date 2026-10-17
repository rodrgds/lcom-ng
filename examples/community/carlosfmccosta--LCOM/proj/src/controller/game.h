/**
 * @file game.h
 * @brief Interface for managing the main game state and entities.
 *
 * This header provides functions for creating, updating, drawing, and destroying the game,
 * as well as accessing game entities such as the player, walls, bullets, and enemies.
 */
#ifndef __GAME_H
#define __GAME_H

#include "assets/ammo.h"
#include "assets/cursor.h"
#include "view/draw_timer.h"
#include "view/health_bar.h"
#include <model/bullet.h>
#include <model/enemy.h>
#include <model/player.h>
#include <model/wall.h>
#include <queue.h>
#include "state.h"

/**
 * @brief Opaque type representing the game state and entities.
 */
struct s_game;
typedef struct s_game game_t;

/**
 * @brief Checks for collision between two sprites.
 *
 * @param sp1 Pointer to the first sprite.
 * @param sp2 Pointer to the second sprite.
 * @return true if the sprites collide, false otherwise.
 */
bool check_collision(sprite_t *sp1, sprite_t *sp2);

/**
 * @brief Initializes and allocates memory for the game.
 *
 * @param v_res Vertical resolution of the screen.
 * @param h_res Horizontal resolution of the screen.
 * @return Pointer to the initialized game structure.
 */
game_t *create_game(unsigned v_res, unsigned h_res);

/**
 * @brief Updates the game state each timer tick.
 *
 * @param game Pointer to the game structure.
 * @param h_res Horizontal resolution.
 * @param v_res Vertical resolution.
 * @param counter Timer counter.
 * @return 0 if success, non-zero if there was an error.
 */
int timer_update_game(game_t *game, unsigned h_res, unsigned v_res, int counter);

/**
 * @brief Draws all the elements of the game on the screen.
 *
 * @param game Pointer to the game structure.
 * @return 0 if success, non-zero if there was an error.
 */
int draw_game(game_t *game);

/**
 * @brief Updates post-event state in the game.
 *
 * @param game Pointer to the game structure.
 */
void timer_update_post_event_game(game_t *game);

/**
 * @brief Updates the game when it is idle (no input or timer).
 *
 * @param game Pointer to the game structure.
 */
void game_idle(game_t *game);

/**
 * @brief Frees all memory associated with the game.
 *
 * @param game Pointer to the game structure.
 */
void destroy_game(game_t *game);

/**
 * @brief Retrieves the game's player instance.
 *
 * @param game Pointer to the game structure.
 * @return Pointer to the player object.
 */
player_t *get_game_player(game_t *game);

/**
 * @brief Gets the array of walls in the game.
 *
 * @param game Pointer to the game structure.
 * @return The wall array used in the game.
 */
wall_array_t get_game_wall_arr(game_t *game);

/**
 * @brief Gets the array of bullets in the game.
 *
 * @param game Pointer to the game structure.
 * @return Array of pointers to bullets.
 */
bullet_t **get_game_bullets(game_t *game);

/**
 * @brief Retrieves the current score of the player.
 *
 * @param game Pointer to the game structure.
 * @return The current score.
 */
unsigned int get_game_score(game_t *game);

/**
 * @brief Gets the array of enemies in the game.
 *
 * @param game Pointer to the game structure.
 * @return Array of pointers to enemies.
 */
enemy_t **get_game_enemies(game_t *game);

#endif
