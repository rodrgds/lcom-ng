/**
 * @file bullet.h
 * 
 */

#ifndef _LCOM_BULLET_H_
#define _LCOM_BULLET_H_
#include <lcom/lcf.h>
#include <stdint.h>
#include "player.h"
#include "helicopter.h"
#include "devices/values.h"
#include "devices/keyboard.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    int16_t vx;
    int16_t vy;
    bool in_game;
} Bullet;


/**
 * @brief This method initializes the player and the helicopter bullets
 * 
 */
void (initialize_bullets)();

/**
 * @brief This method draws a player bullet
 * 
 * @param bullet Pointer to the bullet
 */
int (draw_bullet_player)(Bullet* bullet);

/**
 * @brief This method draws a helicopter bullet
 * 
 * @param bullet Pointer to the bullet
 */
int (draw_bullet_heli)(Bullet* bullet);

/**
 * @brief This method draws all player and helicopter bullets.
 * 
 */
void draw_c_bullets();

/**
 * @brief This method updates the position of the bullets fired by the helicopter and checks the collisions with the player
 * 
 * @param player Pointer to the player
 */
void heli_update_bullets(Player * player);

/**
 * @brief This method creates a bullet fired by the helicopter towards the player
 * 
 * @param obj Pointer to the helicopter
 * @param target Pointer to the player
 */
void heli_create_bullet( Helicopter* obj,Player* target);

/**
 * @brief This method verifies if a bullet collides with the helicopter
 * 
 * @param heli Pointer to the helicopter
 * @param bullet Pointer to the bullet
 */
void verify_heli_collision (Helicopter * heli, Bullet* bullet);

/**
 * @brief This method updates the position of the bullets fired by the player and checks the collisions with the helicopter
 * 
 * @param heli Pointer to the helicopter
 */
void player_update_bullets(Helicopter * heli);

/**
 * @brief This method creates a new bullet fired by the player towards the current mouse position
 * 
 * @param obj Pointer to the player
 * @param pp Pointer to the packet
 * @param mouse Pointer to the mouse
 */
void player_create_bullet( Player* obj,struct packet *pp, Mouse *mouse);

/**
 * @brief This method verifies if a bullet has collided with the player
 * 
 * @param player Pointer to the player
 * @param bullet Pointer to the bullet
 */
void verify_player_collision (Player * player, Bullet* bullet);

/**
 * @brief This method draws the remaining player bullets
 * 
 * @param bullets Number of remaining bullets
 */
void draw_remaining_bullets(uint8_t bullets);



#endif // _LCOM_BULLET_H_
