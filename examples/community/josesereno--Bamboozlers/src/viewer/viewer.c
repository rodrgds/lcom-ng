#include "viewer.h"

extern Menu_state menu_state;

int(draw_sprite)(Sprite *sprite){
    uint32_t color;
    uint16_t width = sprite->width[sprite->i], height = sprite->height[sprite->i], x = sprite->x, y = sprite->y;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            color = sprite->pixmap_array[sprite->i][col + row * width];
            if (color == TRANSPARENT)
                continue;
            if (vg_draw_pixel(x + col, y + row, color))
                return 1;
        }
    }

    return 0;
}

int(draw_menu)(){
    switch (menu_state) {
    case MENU:
        draw_main_menu();
        break;
    case LEVEL_SELECTION:
        draw_level_select_menu();
        break;
    case INSTRUCTIONS:
        draw_instructions_menu();
        break;
    case GAME:
        draw_game_menu();
        break;
    case GAME_OVER:
        draw_game_over_menu();
        break;
    case EXIT:
        return 0;
    }
    return draw_cursor();
}

int(draw_cursor)(){
    switch (menu_state) {
        case MENU:
            return draw_menu_cursor();
        case GAME:
            return draw_game_cursor();
        case LEVEL_SELECTION:
            return draw_level_cursor();
        case INSTRUCTIONS:
            cursor->i = 0;
            return draw_sprite(cursor);
        case GAME_OVER:
            return draw_game_over_cursor();
        case EXIT:
            return 0;
    }
    return 0;
}
