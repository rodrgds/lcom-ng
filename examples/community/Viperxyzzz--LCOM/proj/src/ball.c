#include "ball.h"



Ball* create_ball(uint16_t x, uint16_t y)
{
    
    
    Ball* ball = (Ball*)malloc(sizeof(Ball));
    ball->x = x;
    ball->y = y;

    ball->dx = 3;
    ball->dy = 3;

    ball->old_x = x;
    ball->old_y = y;

    xpm_image_t img;
    xpm_load(ball_xpm,XPM_8_8_8,&img);

    ball->width = img.width;
    ball->height = img.height;
    ball->map = img.bytes;

    ball->diameter = 16;
    return ball;
}

int draw_ball(Ball* ball)
{

    uint32_t index = 0;
    uint8_t* map = ball->map;

    for (uint16_t j = ball->y ; j < ball->y + ball->height; j++)
    {
        for(uint16_t i = ball->x; i < ball->x + ball->width; i++)
        {
            uint32_t color = 0;
            memcpy(&color,&map[index],3);

            pixel_set_color(i,j,color);
            index += 3;
        }
    }


    return 0;

}

void destroy_ball(Ball* ball)
{
    free(ball->map);
    free(ball);
}

