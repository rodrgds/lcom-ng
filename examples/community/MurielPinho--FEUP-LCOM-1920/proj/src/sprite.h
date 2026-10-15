#include <lcom/lcf.h>

#ifndef SPRITE_H
#define SPRITE_H

/** @defgroup Sprite Sprite
 * @{
 * Functions to control the project's sprites
 */
/**
 * @brief Struct type for the Sprite object
 */
typedef struct
{
    uint16_t x; /**< @brief uint16_t for the x coordinate of the sprite */
    uint16_t y; /**< @brief uint16_t for the y coordinate of the sprite */
    uint16_t width; /**< @brief uint16_t for the width of the sprite */
    uint16_t height; /**< @brief uint16_t for the height of the sprite */
    int angle;       /**< @brief int for the angle of the sprite */
    int xspeed;      /**< @brief int for the speed on the x axis of the sprite */
    int yspeed;      /**< @brief int for the speed on the y axis  of the sprite */
    uint8_t *map;    /**< @brief uint8_t* for the map of the sprite */
} Sprite;

/** @brief Extern variable that contains the number of interrupts from timer*/
extern uint32_t interruptCounter;
/**
 * @brief Function to create the sprite object
 * @param xpm xpm_map_t to the pixel map of the sprite 
 * @param x int for the initial x coordinate of the sprite 
 * @param y int for the initial y coordinate of the sprite 
 * @return Returns the created sprite object
 */
Sprite *create_sprite(xpm_map_t xpm, int x, int y);
/**
 * @brief Function to delete a sprite object
 * @param sp Sprite* for the sprite object 
 */
void destroy_sprite(Sprite *sp);
/**
 * @brief Function to draw a sprite object
 * @param sp Sprite* for the sprite object 
 * @return Return 0 if sucessfull on draw 
 */
int draw_sprite(Sprite *sp);
/**
 * @brief Function to clear a sprite from the video memory
 * @param sp Sprite* for the sprite object 
 */
void clear_sprite(Sprite *sp);
/**
 * @brief Function to set the speed of sprite object
 * @param xi int for the initial x coordinate of the sprite 
 * @param yi int for the initial y coordinate of the sprite 
 * @param xf int for the final x coordinate of the sprite 
 * @param yf int for the final y coordinate of the sprite
 * @param speed int for the how many pixels per interrupt the sprite moves 
 */
void set_sprite_speed(Sprite *sprite, int xi, int yi, int xf, int yf, int speed);
/**
 * @brief Function to update the position of a sprite object
 * @param sprite Sprite* for the sprite object 
 * @param xf int for the final x coordinate of the sprite 
 * @param yf int for the final y coordinate of the sprite
 * @return Return 0 if sucessfull on updating 
 */
int update_sprite(Sprite *sprite, uint16_t xf, uint16_t yf);
/**
 * @}
 */
#endif /*SPRITE_H*/
