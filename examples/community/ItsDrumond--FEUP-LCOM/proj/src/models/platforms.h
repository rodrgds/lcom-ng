/**
 * @file platforms.h
 * 
 */

#ifndef _LCOM_PLATFORMS_H_
#define _LCOM_PLATFORMS_H_

#include <stdint.h>

#define N_PLATFORMS 4


typedef struct {
uint16_t x; 
uint16_t y;
uint16_t width;
uint16_t height;
} Platform;

Platform platforms[N_PLATFORMS];

/**
 * @brief This method initializes the platforms for the game
 * 
 */
void initialize_platforms();

/**
 * @brief This method sets the location and the size of the platforms
 * 
 * @param x The x coordinate of the platform's top-left corner
 * @param y The y coordinate of the platform's top-left corner
 * @param width The width of the platform
 * @param height The height of the platform
 * @param number The index of the platform to modify
 */
void platforms_location(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint8_t number);

/**
 * @brief This method draws the platforms
 * 
 */
void draw_platforms();

#endif
