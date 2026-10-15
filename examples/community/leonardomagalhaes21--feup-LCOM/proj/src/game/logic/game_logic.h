/**
 * @file game_logic.h
 * @brief File containing the declaration of the game logic functions.
 */

#ifndef __GAME_LOGIC_H
#define __GAME_LOGIC_H

#define FLYMONS1_X 800
#define FLYMONS1_Y 100
#define FLYMONS2_X 100
#define FLYMONS2_Y 200

#include "game/classes/player.h"
#include "game/classes/enemy.h"
#include "game/classes/mouse_cursor.h"
#include "game/sprite/sprite.h"
#include "game/classes/bullet_node.h"
#include "game/classes/enemy_bullet.h"
#include "game/classes/bullet.h"
#include "game/menu/menu.h"
#include "game/logic/save_score.h"

#define DEFAULT_X_RESOLUTION_14C 1152
#define DEFAULT_Y_RESOLUTION_14C 864
#define FIRST_LINE_Y 650
#define SECOND_LINE_Y 470
/**
 * @brief Updates player logic based on keyboard and mouse inputs.
 * 
 * @param player Pointer to the player structure.
 * @param mouse Pointer to the mouse cursor structure.
 * @param key_a_pressed Indicates whether the 'A' key is pressed.
 * @param key_d_pressed Indicates whether the 'D' key is pressed.
 * @param key_w_pressed Indicates whether the 'W' key is pressed.
 * @param speed_x Pointer to the player's horizontal speed.
 * @param speed_y Pointer to the player's vertical speed.
 * @param unvulnerability Pointer to the player's invulnerability countdown.
 */
void update_player_logic(player *player, MouseCursor *mouse, bool key_a_pressed, bool key_d_pressed, bool key_w_pressed, int8_t *speed_x, int8_t *speed_y, int *unvulnerability);



/**
 * @brief Updates enemy logic based on game conditions.
 * 
 * @param mouse Pointer to the mouse cursor structure.
 * @param create_enemy Indicates whether a new enemy should be created.
 * @param player Pointer to the player structure.
 */
void update_enemy_logic(MouseCursor *mouse, bool create_enemy, player *player);


/**
 * @brief Updates bullet logic based on game conditions.
 * 
 * @param head Pointer to the pointer of the bullet nodes list.
 */
void update_bullet_logic(bullet_node **head);


/**
 * @brief Checks for collisions between the player and enemy bullets.
 * 
 * @param player Pointer to the player structure.
 * @param unvulnerability Pointer to the player's invulnerability countdown.
 * @return True if collisions occurred, false otherwise.
 */
bool check_enemy_bullet_collisions(player *player,int *unvulnerability);


/**
 * @brief Spawns new dead enemies based on random conditions.
 */
void spawn_dead_enemies();


#endif /* __GAME_LOGIC_H */

