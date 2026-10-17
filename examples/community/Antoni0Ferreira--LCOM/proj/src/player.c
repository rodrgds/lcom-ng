#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "player.h"

int initialize_player(struct player *player){

    uint16_t xi = X_INITIAL_PLAYER, yi = Y_INITIAL_PLAYER;

	xpm_image_t img;
    player->img = img;
    xpm_map_t playerMap = ship_xpm;

    player->sprite = xpm_load(playerMap, XPM_8_8_8, &player->img);
    if(player->sprite == NULL) return 1;

    player->x = xi; player->y = yi;

    return 0;

}

void draw_player(struct player *player, struct keyboard_state *state, int boost_value, int *points){
    if (state->W_DOWN == true) {
        player->y -= (4 + boost_value);

        if (player->y >= DOWN_BORDER_LIMIT) {
            player->y = Y_INITIAL_PLAYER;
            player->x = X_INITIAL_PLAYER;
            state->W_DOWN = false;
            *points += 1;
        }
    }
    if (state->A_DOWN == true) {
        player->x -= (4 + boost_value);

        if (player->x >= RIGHT_BORDER_LIMIT)
        player->x = 0;
    }
    if (state->S_DOWN == true){
        player->y += (4 + boost_value);
        if (player->y > (DOWN_BORDER_LIMIT - player->img.height))
            player->y = (DOWN_BORDER_LIMIT - player->img.height);
    }
    if (state->D_DOWN == true) {
        player->x += (4 + boost_value);

        if (player->x > (RIGHT_BORDER_LIMIT - player->img.width))
        player->x = (RIGHT_BORDER_LIMIT - player->img.width);
    }

    vg_draw_sprite(player->sprite, player->x, player->y, player->img);

}
