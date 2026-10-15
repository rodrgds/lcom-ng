#include "ammo.h"

void draw_ammo(int index){
    if(!ammo[index].is_dead && !ammo[index].is_in_inventory){
        draw(ammo[index], ammo[index]);
        draw_img_number(ammo[index], ammo[index].x+ammo[index].width-12,ammo[index].y+(ammo[index].height/2));
    }
}
void get_ammo(int index){
    for(int z = 0; z < (int) (sizeof(ammo) / sizeof(ammo[0])); z++){
        if(!ammo[z].is_in_inventory && ammo[z].is_dead){
            ammo[z].is_in_inventory = true;
            ammo[z].is_dead = false;
            ammo[z].idle_time = 8;
            ammo[z].type = AMMO;
            inventory[index].colors_32 = ammo[z].colors_32;
            inventory[index].width = ammo[z].width;
            inventory[index].height = ammo[z].height;
            inventory[index].type = AMMO;
            inventory[index].idle_time = 8;
            break;
        }
    }
}
