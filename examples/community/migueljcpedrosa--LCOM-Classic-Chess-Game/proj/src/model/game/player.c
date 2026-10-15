#include "player.h"
#include <stdlib.h>

Player* (create_player)(char name[], Color color, PlayerType type){

    Player* player = malloc(sizeof(Player));

    char* name_copy = malloc(sizeof(char) * 30);
    sprintf(name_copy, "%s", name);
    player->name = name_copy;
    player->color = color;
    player->type = type;

    player->time_counter = ROUND_TIME * 60;
    player->timer.start_time = 0;
    player->timer.wait_time = 0;

    return player;
}

void destroy_player(Player* player){
    
    free(player->name);
    free(player);
}
