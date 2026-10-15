#include "package.h"



void draw_packages(){
    for(int i = 0; i < (int) (sizeof(packages) / sizeof(packages[0])); i++){
        if(!packages[i].is_dead && !packages[i].is_in_inventory){
            draw(packages[i], packages[i]);
        }
    }
}

void use_item(int index){
    if(inventory[index].type == PACKAGE && inventory[index].is_in_inventory){
        int rate = rand()%70;
        if(get_item(index, rate)){
            animated_img_player.speed += 1;
            for(int i = 0; i < (int) (sizeof(packages) / sizeof(packages[0])); i++){
                if(packages[i].is_in_inventory){
                    packages[i].is_in_inventory = false;
                    packages[i].is_dead = true;
                    packages[i].idle_time = inventory[index].idle_time;
                    break;
            }
        }
        }
    }
    else if(inventory[index].type == PISTOL && inventory[index].is_in_inventory){
        use_pistol(index);
    }
    else if(inventory[index].type == PIZZA && player_lives <40 && inventory[index].is_in_inventory){
        use_pizza(index);

    }
    else if(inventory[index].type == BOOTS && inventory[index].is_in_inventory){
        if(!equipment[2].is_in_inventory){
            use_boots(index);
        }
    }
    else if(inventory[index].type == BACKPACK && inventory[index].is_in_inventory){
        if(!equipment[1].is_in_inventory){
            use_backpack(index);
        }
    }
    else if(inventory[index].type == GRENADE && inventory[index].is_in_inventory){
        inventory[index].is_dead = true;
        inventory[index].is_in_inventory = false;
        for(int i = 0; i < (int) (sizeof(grenades) / sizeof(grenades[0])); i++){
            if(grenades[i].is_in_inventory && !grenades[i].is_dead){
                grenades[i].is_in_inventory = true;
                grenades[i].is_dead = true;
                grenades[i].direction = STILL;
                grenades[i].x = player.x;
                grenades[i].y = player.y;
                grenades[i].hitbox_x = grenades[i].x+(grenades[i].width/2);
                grenades[i].hitbox_y = grenades[i].y+(grenades[i].height/2);
                set_throwing_index(get_current_index());
                grenade_size++;
                break;
            }
        }
    }
    else if(inventory[index].type == BONE && inventory[index].is_in_inventory){
        inventory[index].is_dead = true;
        inventory[index].is_in_inventory = false;
        if(bone.is_in_inventory && !bone.is_dead){
            bone.is_in_inventory = true;
            bone.is_dead = true;
            bone.direction = STILL;
            bone.x = player.x;
            bone.y = player.y;
            bone.hitbox_x = bone.x+(bone.width/2);
            bone.hitbox_y = bone.y+(bone.height/2);
            set_throwing_index(get_current_index());
        }
    }
}

int get_item(int index, int rate){
    if(rate < 2 && watch.is_dead && !watch.is_in_inventory){
        get_watch(index);
        return 1;
    }
    else if(rate < 4 && boots.is_dead && !boots.is_in_inventory){
        get_boots(index);
        return 1;
    }
    else if(rate < 6 && backpack.is_dead && !backpack.is_in_inventory){
        get_backpack(index);
        return 1;
    }
    else if(rate < 12 && bone.is_dead && !bone.is_in_inventory){
        get_bone(index);
        return 1;
    }
    else if(rate < 20 && grenade_size){
        get_grenade(index);
        grenade_size--;
        return 1;
    }
    else if(rate < 50 && pistol_size){
        get_pistol(index);
        pistol_size--;
        return 1;
    }
    else if(rate < 40 && food_size){
        get_pizza(index);
        food_size--;
        return 1;
    }
    else if(rate < 70 && ammo_size){
        get_ammo(index);
        ammo_size--;
        return 1;
    }
    if(ammo_size){
        get_ammo(index);
        ammo_size--;
        return 1;
    }
    if(food_size){
        get_pizza(index);
        food_size--;
        return 1;
    }
    if(pistol_size){
        get_pistol(index);
        pistol_size--;
        return 1;
    }
    if(grenade_size){
        get_grenade(index);
        grenade_size--;
        return 1;
    }
    if(bone.is_dead && !bone.is_in_inventory){
        get_bone(index);
        return 1;
    }
    if(backpack.is_dead && !backpack.is_in_inventory){
        get_backpack(index);
        return 1;
    }
    if(boots.is_dead && !boots.is_in_inventory){
        get_boots(index);
        return 1;
    }
    if(watch.is_dead && !watch.is_in_inventory){
        get_watch(index);
        return 1;
    }
    return 0;
}



