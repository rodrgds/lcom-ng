#include "view_menu.h"
#include "sprite.h"

int drawMainMenu()
{

        if (drawSprite(background, SCREEN_START_X, SCREEN_START_Y))
                return 1;
        if (drawButton(Start, StartPressed, START_BUTTON_X, START_BUTTON_Y))
                return 1;
        if (drawButton(Exit, ExitPressed, EXIT_BUTTON_X, EXIT_BUTTON_Y))
                return 1;
        if (drawButton(table, table_pressed, TABLE_BUTTON_X, TABLE_BUTTON_Y))
                return 1;
        if (drawButton(multiplayer_not, multiplayer, MULTIPLAYER_BUTTON_X, MULTIPLAYER_BUTTON_Y))
                return 1;
        if (drawCursor(0))
                return 1;

        return 0;
}

int draw_animated_sprite(animated_sprite_t *anim)
{
        if (anim == NULL)
                return 1;

        // Get the current sprite frame using cur_pixmap
        sprite_t *current_sprite = anim->pixmaps[anim->cur_pixmap];

        uint16_t height = current_sprite->height;
        uint16_t width = current_sprite->width;

        // important CACHE
        for (int h = 0; h < height; h++)
        {
                for (int w = 0; w < width; w++)
                {
                        uint32_t current_color = current_sprite->pixmap[w + h * width];
                        if (current_color == DEFOULT_COLOR)
                                continue;

                        vg_draw_pixel(anim->x + w, anim->y + h, current_color);
                }
        }
        // transparency
        return 0;
}
