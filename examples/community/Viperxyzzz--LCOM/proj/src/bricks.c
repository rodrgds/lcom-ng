#include "bricks.h"



Brick* create_brick(uint16_t x, uint16_t y)
{
    Brick* brick = (Brick*)malloc(sizeof(Brick));
    brick->x = x;
    brick->y = y;
    brick->is_destroyed = false;


    xpm_image_t img;

    uint8_t* map = xpm_load(brick_xpm,XPM_8_8_8,&img);

    if(map == NULL)
        return NULL;
    
    brick->width = img.width;
    brick->height = img.height;
    brick->map = img.bytes;

    return brick;
}

int draw_brick(Brick* brick)
{
    uint32_t index = 0;
    uint8_t* map = brick->map;
    for (uint16_t j = brick->y ; j < brick->y + brick->height; j++)
    {
        for(uint16_t i = brick->x; i < brick->x + brick->width; i++)
        {
            uint32_t color = 0;
            memcpy(&color,&map[index],3);

            pixel_set_color(i,j,color);
            index += 3;
        }
    }

    return 0;
}

void destroy_brick(Brick* brick)
{
    free(brick->map);
    free(brick);
}
