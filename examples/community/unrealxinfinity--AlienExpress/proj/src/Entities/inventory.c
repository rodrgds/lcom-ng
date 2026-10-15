#include "inventory.h"

void draw_inventory(){
    int x = 6;
    int y = 6;
    for(int j = 0; j < inventory_size; j++){
        for(int i = 0; i < 3; i++){
            inventory_box.x = x+i*36;
            inventory_box.y = y+j*36;
            draw(inventory_box, inventory_box);
            if(inventory[j*3+i].is_in_inventory){
                draw(inventory[j*3+i], inventory[j*3+i]);
                if(inventory[j*3+i].type == AMMO || inventory[j*3+i].type == PISTOL){
                    draw_img_number(inventory[j*3+i], inventory[j*3+i].x+inventory[j*3+i].width-12,inventory[j*3+i].y+(inventory[j*3+i].height/2));
                }
            }
        }
    }
    for(int j = 0; j < 3; j++){
        inventory_box.x = 163;
        inventory_box.y = 2+(j*36);
        draw(inventory_box, inventory_box);
        if(equipment[j].is_in_inventory){
            draw(equipment[j], equipment[j]);
        }
    }
}

int is_inventory_full(){
    for(int i = 0; i < (inventory_size*3); i++){
        if(inventory[i].is_in_inventory == false){
            return i;
        }
    }
    return -1;
}

void remove_from_equipment(int index){
    if(equipment[index].type == BOOTS && equipment[index].is_in_inventory){
        equipment[index].is_in_inventory = false;
        boots.is_dead = false;
        boots.is_in_inventory = false;
        boots.x = player.x;
        boots.y = player.y;
        boots.idle_time = equipment[index].idle_time;
        animated_img_player.speed -= 1;
    }
    if(equipment[index].type == BACKPACK && equipment[index].is_in_inventory){
        equipment[index].is_in_inventory = false;
        backpack.is_dead = false;
        backpack.is_in_inventory = false;
        backpack.x = player.x;
        backpack.y = player.y;
        backpack.idle_time = equipment[index].idle_time;
        int count = count_packages();
        while(count > 0){
            animated_img_player.speed += 1;
            count--;
        }
        inventory_size = 1;
    }
}

int count_packages(){
    int count = 0;
    for(int i = 3; i < 9; i++){
        if(inventory[i].type == PACKAGE && inventory[i].is_in_inventory){
            count++;
        }
    }
    return count;
}
