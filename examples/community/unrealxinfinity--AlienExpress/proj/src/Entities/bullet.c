#include "bullet.h"


void draw_bullets(){
    for (int i = 0; i < (int) (sizeof(bullets) / sizeof(bullets[0])); i++){
        if(!bullets[i].is_dead && !bullets[i].is_in_inventory){
            draw(bullets[i], bullets[i]);
        }
    }
}
void move_bullets(){
    for (int i = 0; i < (int) (sizeof(bullets) / sizeof(bullets[0])); i++){
        if(bullets[i].is_dead)continue;
        is_occupied(bullets[i], 20);
        manage_collision(&bullets[i], 10);
    }
}

