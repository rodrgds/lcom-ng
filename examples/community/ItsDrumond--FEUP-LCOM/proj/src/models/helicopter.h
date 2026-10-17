/**
 * @file helicopter.h
 * 
 */

#ifndef _LCOM_HELICOPTER_H_
#define _LCOM_HELICOPTER_H_

#include <stdint.h>

#include "devices/keyboard.h"
#include "player.h"

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t vx;
  uint16_t vy;
  uint8_t hp;
  bool alive;
} Helicopter;

/**
 * @brief This method updates the position of the helicopter
 * 
 * @param heli Pointer to the helicopter
 */
void update_heli_move(Helicopter* heli);


/**
 * @brief This method creates a bullet for the helicopter to shoot at the player
 * 
 * @param heli Pointer to the helicopter
 * @param player Pointer to the player
 */
void helicopter_shoot(Helicopter* heli ,Player* player);

/**
 * @brief This method draws the helicopter
 * 
 * @param heli Pointer to the helicopter
 */
void draw_helicopter(Helicopter* heli);

/**
 * @brief This method destroys the helicopter
 * 
 * @param heli Pointer to the helicopter
 */
void destroy_helicopter(Helicopter * heli);

/**
 * @brief This method initializes the helicopter with the default settings
 * 
 * @param heli Pointer to the helicopter
 */
void init_helicopter(Helicopter * heli);

/**
 * @brief This method increases the difficulty of the game by increasing the helicopter stats
 * 
 * @param heli Pointer to the helicopter
 */
void helicopter_add_difficulty(Helicopter * heli);

/**
 * @brief This method draws an explosion animation
 * 
 * @param heli Pointer to the helicopter
 * @return Returns an int value of one if the explosion is ongoing and zero if it is finished
 */
int draw_explosions(Helicopter * heli);


#endif // _LCOM_HELICOPTER_H_

