/**

@file Missile.h
@brief Header file containing declarations of functions and struct used for handling and drawing the missile.

*/
#ifndef MISSILE_H
#define MISSILE_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include "proj.h"

struct Missile
{
    bool shot;
    uint16_t x;
    uint16_t y;
    uint16_t time;
    uint16_t frame_time;
    uint16_t frame;
};

/**
 * @brief changes missile variables to indicates that it is being shot
 * 
 */
void shoot_missile();
/**
 * @brief Handles missile animation and collision
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int handle_missile();
/**
 * @brief draw missile on a selected frame
 * 
 * @param frame selected frame of missile animation to draw
 * @return int 0 upon success, non-zero otherwise.
 */
int draw_missile(uint16_t frame);
/**
 * @brief decreases time until missile is available
 * 
 */
void increment_missile();
/**
 * @brief sets default values for missile variables
 * 
 */
void startup_missile();
/**
 * @brief informs if the missile is ready to be shot
 * 
 * @return true if the missile is ready to be shot
 * @return false if the missile is not ready to be shot
 */
bool missile_ready();

#endif //MISSILE_H
