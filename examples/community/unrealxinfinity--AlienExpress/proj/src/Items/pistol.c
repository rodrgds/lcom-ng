#include "pistol.h"



void draw_pistol(int index){
    if(!pistols[index].is_dead && !pistols[index].is_in_inventory){
        draw(pistols[index], pistols[index]);
        draw_img_number(pistols[index], pistols[index].x+pistols[index].width-12,pistols[index].y+(pistols[index].height/2));
    }
}

void use_pistol(int index){
    if(inventory[index].idle_time < 40){
        for(int i = 0; i < inventory_size*3; i++){
            if(inventory[i].type == AMMO && inventory[i].is_in_inventory){
                int ammo_count = inventory[index].idle_time;
                ammo_count += inventory[i].idle_time;
                if(ammo_count > 40){
                    inventory[i].idle_time = ammo_count-40;
                    inventory[index].idle_time = 40;
                }
                else{
                    inventory[index].idle_time += inventory[i].idle_time;
                    inventory[i].is_dead = true;
                    inventory[i].is_in_inventory = false;
                    for(int z = 0; z < (int) (sizeof(ammo) / sizeof(ammo[0])); z++){
                        if(ammo[z].is_in_inventory && !ammo[z].is_dead){
                            ammo[z].is_dead = true;
                            ammo[z].is_in_inventory = false;
                            ammo_size++;
                            break;
                        }
                    }
                }
                if(inventory[index].idle_time == 40)break;
            }
        }
    }
}

void get_pistol(int index){
    for(int z = 0; z < (int) (sizeof(pistols) / sizeof(pistols[0])); z++){
        if(!pistols[z].is_in_inventory && pistols[z].is_dead){
            pistols[z].is_in_inventory = true;
            pistols[z].is_dead = false;
            pistols[z].idle_time = 0;
            pistols[z].type = PISTOL;
            inventory[index].colors_32 = pistols[z].colors_32;
            inventory[index].width = pistols[z].width;
            inventory[index].height = pistols[z].height;
            inventory[index].type = PISTOL;
            inventory[index].idle_time = 0;
            break;
        }
    }
}

