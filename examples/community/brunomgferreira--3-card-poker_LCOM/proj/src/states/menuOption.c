#include <lcom/lcf.h>
#include "menuOption.h"

bool (is_point_on_menu_option)(MenuOption option, uint32_t x, uint32_t y) {
    return (x >= option.x && x <= option.x + option.width && y >= option.y && y <= option.y + option.height);
}

void (menu_move_selector)(uint8_t scancode, uint8_t *selected, uint8_t number_of_options) {
    switch (scancode) {
        case 0x48:
            // Move selector up
            if (*selected == 0) {
                *selected = number_of_options - 1;
            } else {
                *selected = *selected - 1;
            }
            break;

        case 0x50:
            // Move selector down
            if (*selected == number_of_options - 1) {
                *selected = 0;
            } else {
                *selected = *selected + 1;
            }
            break;

        default:
            break;
    }
}

void (draw_menu_option_selector)(int x, int y, int width, int height, bool box) {
    if (box) {
        vg_draw_hline(x, y, width, 0xFFFFFF);
        vg_draw_hline(x, y + height, width, 0xFFFFFF);

        vg_draw_vline(x, y, height, 0xFFFFFF);
        vg_draw_vline(x + width, y, height, 0xFFFFFF);
    } else {
        coin_shadow_1->x = x + 12;
        coin_shadow_1->y = y + 1 + (height / 2) - (coin_shadow_1->map[0]->height / 2);
        animSprite_horizontal_flip(coin_shadow_1, 0, 1, 0.04);
        
        coin_1->x = x + 10;
        coin_1->y = y - 1 + (height / 2) - (coin_1->map[0]->height / 2);
        animSprite_horizontal_flip(coin_1, 0, 1, 0.04);

        coin_shadow_2->x = x + width - 10 - coin_shadow_2->map[0]->width;
        coin_shadow_2->y = y + 1 + (height / 2) - (coin_shadow_2->map[0]->height / 2);
        animSprite_horizontal_flip(coin_shadow_2, 0, 1, 0.04);

        coin_2->x = x + width - 12 - coin_2->map[0]->width;
        coin_2->y = y - 1 + (height / 2) - (coin_2->map[0]->height / 2);
        animSprite_horizontal_flip(coin_2, 0, 1, 0.04);
    }
}
