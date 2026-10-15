#include "backpack.h"



void draw_backpack(){
    if(!backpack.is_dead && !backpack.is_in_inventory){
        draw(backpack, backpack);
    }
}

void get_backpack(int index){
    backpack.is_dead = false;
    backpack.is_in_inventory = true;
    inventory[index].colors_32 = backpack.colors_32;
    inventory[index].width = backpack.width;
    inventory[index].height = backpack.height;
    inventory[index].type = BACKPACK;
    backpack.type = BACKPACK;
}

void use_backpack(int index){
    inventory[index].is_in_inventory = false;
    equipment[1].colors_32 = backpack.colors_32;
    equipment[1].width = backpack.width;
    equipment[1].height = backpack.height;
    equipment[1].is_in_inventory = true;
    equipment[1].type = BACKPACK;
    equipment[1].idle_time = backpack.idle_time;
    inventory_size = 3;
}
