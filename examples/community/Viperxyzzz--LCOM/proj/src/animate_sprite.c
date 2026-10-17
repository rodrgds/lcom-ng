#include "animate_sprite.h"


AnimSprite *create_animSprite(uint16_t x, uint16_t y, uint8_t no_pic, xpm_row_t pic1[], ...)
{
    AnimSprite *asp = malloc(sizeof(AnimSprite));
    asp->sp = create_sprite(pic1, x, y, 0, 0);
    asp->map = malloc((no_pic) * sizeof(char *));
    asp->map[0] = asp->sp->map;
    va_list ap;
    va_start(ap, pic1);
    unsigned i;
    for(i = 1; i < no_pic; i++)
    {
        xpm_row_t* tmp = va_arg(ap,xpm_row_t*);
        
        
        xpm_image_t img;
        xpm_load(tmp,XPM_8_8_8,&img);

        asp->map[i] = img.bytes;
        if(asp->map[i] == NULL || img.width != asp->sp->width || img.height != asp->sp->height)
        {
            for(uint8_t j = 1; j < i; j++)
            {
                free(asp->map[i]);
            }
            free(asp->map);
            destroy_sprite(asp->sp);
            free(asp);
            va_end(ap);
            return NULL;
        }
    }
    va_end(ap);

    asp->aspeed = 8;asp->cur_aspeed = 0;asp->num_fig = i;asp->cur_fig = 0;
    return asp;
}

int animate_sprite(AnimSprite* asp)
{
    asp->cur_aspeed++;
    if(asp->cur_aspeed == asp->aspeed)
    {
        asp->cur_aspeed = 0;
        asp->cur_fig++;
        asp->sp->map = *(asp->map + asp->cur_fig);
        
    }
    return draw_sprite(asp->sp);
}


void destroy_animSprite(AnimSprite* asp)
{
    destroy_sprite(asp->sp);
    free(asp->map);
    free(asp);
}

