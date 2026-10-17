#ifndef BALL_H
#define BALL_H
#include <math.h>
#include <stdio.h>
#include "videocard.h"


#include "../assets/ball.xpm"




typedef struct 
{
    uint16_t x,y,old_x,old_y;
    int dx,dy;
    int width,height;
    int diameter;
    uint8_t* map;

}Ball;
/**
 * @brief Create a ball object
 * 
 * @param x horizontal position of the brick
 * @param y vertical position of the brick
 * @return Ball* returns a Ball* object
 */
Ball* create_ball(uint16_t x, uint16_t y);


/**
 * @brief draws a ball
 * 
 * @param ball the ball we want to draw
 * @return int 0 in case it goes successfully,1 otherwise
 */
int draw_ball(Ball* ball);
/**
 * @brief Destroy a ball
 * 
 * @param ball the ball we want to destroy
 */
void destroy_ball(Ball* ball);

#endif
