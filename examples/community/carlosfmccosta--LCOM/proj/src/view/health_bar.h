/**
 * @file health_bar.h
 * @brief Declares the function to draw the health bar using heart sprites.
 *
 * Provides the interface for rendering the player's health bar on the screen.
 */

#ifndef __HEALTH_BAR_H
#define __HEALTH_BAR_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "model/sprite.h"
#include "assets/heart.h"

/**
 * @brief Draws the health bar on the screen.
 * @param health The current health value.
 * @param h_bar Pointer to the heart sprite.
 * @return 0 on success, non-zero on error.
 */
int draw_health_bar(unsigned int health, sprite_t* h_bar);

#endif //__HEALTH_BAR_H
