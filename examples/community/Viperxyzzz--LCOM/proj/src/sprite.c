#include "Sprite.h"


Sprite* create_sprite(xpm_row_t pic[], uint16_t x,uint16_t y,uint16_t xspeed,uint16_t yspeed) 
{
    Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
    sprite->x = x;
    sprite->y = y;

    xpm_image_t img;
    xpm_load(pic,XPM_8_8_8,&img);
    
    sprite->width = img.width;
    sprite->height = img.height;
    sprite->map = img.bytes;
    
    return sprite;
}


int draw_sprite(Sprite* sp)
{
    uint32_t index = 0;
    uint8_t* map = sp->map;
    for (uint16_t j = sp->y ; j < sp->y + sp->height; j++)
    {
        for(uint16_t i = sp->x; i < sp->x + sp->width; i++)
        {
            uint32_t color = 0;
            memcpy(&color,&map[index],3);

            pixel_set_color(i,j,color);
            index += 3;
        }
    }
    return 0;
}


void destroy_sprite(Sprite* sp)
{
    free(sp->map);
    free(sp);
}
