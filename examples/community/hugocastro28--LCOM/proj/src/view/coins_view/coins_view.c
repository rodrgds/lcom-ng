#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/graphics/vbe.h"
#include "../../devices/graphics/graphics.h"
#include "../../model/coins_model/coin.h"
#include "../initialize_pixmaps.h"
#include "coins_view.h"

extern SmallCoin small_coins[329];
extern uint8_t* small_coin_pixmap;
extern xpm_image_t small_coin_info;

extern BigCoin big_coins[5];
extern uint8_t* big_coin_pixmap;
extern xpm_image_t big_coin_info;

void (draw_small_coins)() {
    for (int i = 0; i < 323; i++)
        if (!small_coins[i].isCaptured)
            draw_xpm(small_coin_pixmap, small_coin_info, small_coins[i].pos.left_x, small_coins[i].pos.top_y);
}

void (erase_small_coins)() {
    for (int i = 0; i < 323; i++)
        draw_rectangle(small_coins[i].pos.left_x, small_coins[i].pos.top_y, small_coin_info.width, small_coin_info.height, BACKGROUND);
}

void (draw_big_coins)() {
    for (int i = 0; i < 5; i++)
        if (!big_coins[i].isCaptured)
            draw_xpm(big_coin_pixmap, big_coin_info, big_coins[i].pos.left_x, big_coins[i].pos.top_y);
}

void (erase_big_coins)() {
    for (int i = 0; i < 5; i++)
        draw_rectangle(big_coins[i].pos.left_x, big_coins[i].pos.top_y, big_coin_info.width, big_coin_info.height, BACKGROUND);
}
