#include "boots.h"


void draw_boots(){
    if(!boots.is_dead && !boots.is_in_inventory){
        draw(boots, boots);
    }
}

void get_boots(int index){
    boots.is_dead = false;
    boots.is_in_inventory = true;
    inventory[index].colors_32 = boots.colors_32;
    inventory[index].width = boots.width;
    inventory[index].height = boots.height;
    inventory[index].type = BOOTS;
    boots.type = BOOTS;
}

void use_boots(int index){
    inventory[index].is_in_inventory = false;
    equipment[2].colors_32 = boots.colors_32;
    equipment[2].width = boots.width;
    equipment[2].height = boots.height;
    equipment[2].is_in_inventory = true;
    equipment[2].type = BOOTS;
    equipment[2].idle_time = boots.idle_time;
    animated_img_player.speed += 1;
}

