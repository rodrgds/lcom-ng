#include "player.h"

PlayerInfo player1_info;
PlayerInfo player2_info;
extern vbe_mode_info_t mode_info;

int (move_player_up)(PlayerInfo *player_info) {
    if (player_info->y <= 0) {
        return 1;
    }
    player_info->y = player_info->y-1;
    return 0;
}

int (move_player_down)(PlayerInfo *player_info) {
    if (player_info->y >= mode_info.YResolution - 60){
        return 1;
    }
    player_info->y = player_info->y+1;
    return 0;
}

int (move_player_left)(PlayerInfo *player_info) {
    if (player_info->x <= 0) {
        return 1;
    }
    player_info->x = player_info->x-1;
    return 0;
}

int (move_player_right)(PlayerInfo *player_info) {
    if (player_info->x >= mode_info.XResolution-20) {
        return 1;
    }
    player_info->x = player_info->x+1;
    return 0;
}
