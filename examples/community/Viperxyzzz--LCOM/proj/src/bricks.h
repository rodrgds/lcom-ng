#ifndef BRICKS_H
#define BRICKS_H
#include <lcom/lcf.h>
#include "videocard.h"


#include "../assets/brick.xpm"



typedef struct
{
    uint16_t x,y; //x and y coordinates of the brick
    float width, height; //width and height of the brick
    bool is_destroyed; //true if the brick is destroyed,false otherwise
    uint8_t* map;//map pointing to the brick image
}Brick;

/**
 * @brief Create a Brick object
 * 
 * @param x horizontal position of the brick
 * @param y vertical position of the brick
 * @param width the width of the brick
 * @param height the height of the brick
 * @param color the desired color for the brick
 * @return Brick* returns a brick object
 */
Brick* create_brick(uint16_t x,uint16_t y);

/**
 * @brief Draws a brick 
 * 
 * @param brick the brick we want to draw
 * @return int 0 in case it goes successfully, 1 otherwise
 */
int draw_brick(Brick* brick);

/**
 * @brief Destroys a brick
 * 
 * @param brick the brick we want to destroy
 */
void destroy_brick(Brick* brick);

#endif
