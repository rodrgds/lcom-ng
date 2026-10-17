#include "Player.h"

void player_init(Player* player){
    player->xPos = 0;
    player->yPos = 0;
}

void player_draw(const Player* player) {
    // Calculate top-left corner of the border rectangle
    int border_thickness = 2;
    int x = 80 + (280 * player->xPos) - border_thickness;
    int y = 10 + (210 * player->yPos) - border_thickness;

    // Draw border rectangle around the 200x200 card
    my_vg_draw_rectangle(x, y, 150 + 2 * border_thickness, 200 + 2 * border_thickness, 0x00FFFF);
}

void player_move_up(Player* player){
    if(player->yPos > 0) player->yPos--;
}

void player_move_down(Player* player){
    if(player->yPos < 3) player->yPos++;
}

void player_move_left(Player* player){
    if(player->xPos > 0) player->xPos--;
}

void player_move_right(Player* player){
    if(player->xPos < 3) player->xPos++;
}
