/**
 * @file player.h
 * 
 */

#ifndef _LCOM_PLAYER_H_
#define _LCOM_PLAYER_H_
#include <lcom/lcf.h>
#include <stdint.h>

#include "../devices/keyboard.h"
#include "../devices/values.h"
#include "platforms.h"


#define MOVE_UP_MAKE 0x11
#define MOVE_DOWN_MAKE 0x1f 
#define MOVE_LEFT_MAKE 0x1e 
#define MOVE_RIGHT_MAKE 0x20
#define MOVE_UP_BREAK 0x91
#define MOVE_DOWN_BREAK 0x9f 
#define MOVE_LEFT_BREAK 0x9e 
#define MOVE_RIGHT_BREAK 0xa0 

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t hp; 
    uint8_t frame;
} Player;

/**
 * @brief This method makes the player move up
 * 
 * @param obj Pointer to the player
 * @param speed Speed at which the player moves
 */
void moveUp(Player* obj, uint16_t speed);


/**
 * @brief This method makes the player move down
 * 
 * @param obj Pointer to the player
 * @param speed Speed at which the player moves
 */
void moveDown(Player* obj, uint16_t speed);

/**
 * @brief This method makes the player move left
 * 
 * @param obj Pointer to the player
 * @param speed Speed at which the player moves
 */
void moveLeft(Player* obj, uint16_t speed);

/**
 * @brief This method makes the player move right
 * 
 * @param obj Pointer to the player
 * @param speed Speed at which the player moves
 */
void moveRight(Player* obj, uint16_t speed);

/**
 * @brief This method makes the player jump
 * 
 * @param obj Pointer to the player
 * @param speed Speed at which the player jumps
 */
void jump(Player* obj, uint16_t speed);

/**
 * @brief This method processes a scancode and performs the corresponding action
 * 
 * @param obj Pointer to the player
 * @param data Pointer to the scancode to be processed
 */
void process_scancode(Player* obj, uint8_t* data);

/**
 * @brief This method processes a packet and creates a bullet if the left button is pressed
 * 
 * @param obj Pointer to the player
 * @param pp Pointer to the packet
 * @param mouse Pointer to the mouse
 */
void process_packet(Player* obj, struct packet *pp, Mouse *mouse);

/**
 * @brief This method draws an health bar based on the given health value
 * 
 * @param hp Health value of the player
 */
void draw_hp_bar(uint8_t hp);

/**
 * @brief This method draws the player
 * 
 * @param player Pointer to the player
 */
void draw_player(Player * player);

/**
 * @brief This method updates the movement of the player based on the input received
 * 
 * @param player Pointer to the player
 */
void player_update_mov(Player *player);

/**
 * @brief This method checks if the player collides with any given platforms
 * 
 * @param player Pointer to the player
 * @param platform Platforms to check for collision
 * @param jump_down Indicates if the player is in the fall part of the jump
 * @return Returns an int value of one if the player collides with a platform and zero otherwise
 */
int check_collision_player(Player *player, Platform platform[], bool jump_down);

#endif // _LCOM_PLAYER_H_
