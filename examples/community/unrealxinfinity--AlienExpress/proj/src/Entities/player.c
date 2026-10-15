#include "player.h"




movement_t move[4]={STILL,STILL,STILL,STILL};

bool is_holding_i = false;

int throwing_index = -5;

int current_index = -1;


int get_current_index(){
    return current_index;
}
bool get_is_holding_i(){
    return is_holding_i;
}
int get_throwing_index(){
    return throwing_index;
}

void set_throwing_index(int throw_timer){
    throwing_index = throw_timer;
}
void reset_movement(){
    for(int i = 0; i<4; i++){
        move[i] = STILL;
    }
}

void player_controller(uint32_t scancode){
    if(scancode == MAKE_W)move[0] = UP;
    if(scancode == MAKE_D)move[1] = RIGHT;
    if(scancode == MAKE_A)move[2] = LEFT;
    if(scancode == MAKE_S)move[3] = DOWN;

    if(scancode == B(MAKE_W))move[0] = STILL;
    if(scancode == B(MAKE_D))move[1] = STILL;
    if(scancode == B(MAKE_A))move[2] = STILL;
    if(scancode == B(MAKE_S))move[3] = STILL;

    if(scancode == MAKE_1){
        selected.is_dead = false;
        selected.x = inventory[0].x-4;
        selected.y = inventory[0].y -4;
        selected.hitbox_x = inventory[0].hitbox_x-4;
        selected.hitbox_y = inventory[0].hitbox_y-4;
        current_index = 0;
    }
    if(scancode == MAKE_2){
        selected.is_dead = false;
        selected.x = inventory[1].x-4;
        selected.y = inventory[1].y-4;
        selected.hitbox_x = inventory[1].hitbox_x-4;
        selected.hitbox_y = inventory[1].hitbox_y-4;
        current_index = 1;
    }
    if(scancode == MAKE_3){
        selected.is_dead = false;
        selected.x = inventory[2].x-4;
        selected.y = inventory[2].y-4;
        selected.hitbox_x = inventory[2].hitbox_x-4;
        selected.hitbox_y = inventory[2].hitbox_y-4;
        current_index = 2;
    }

    
    if(scancode == MAKE_L){
        if(current_index != -1){
            if(inventory[current_index].type == PISTOL && inventory[current_index].is_in_inventory && inventory[current_index].idle_time > 0){
                spawn_bullet(current_index);
            }
        }
        kill_workers();
    }
    if(scancode == MAKE_I && current_index != -1){
        use_item(current_index);
        is_holding_i = true;

    }
    if(scancode == B(MAKE_I)){
        is_holding_i = false;
    }
    if(scancode == MAKE_P){
        remove_package(player);
        remove_item_from_ground(player);
    }
    if(scancode == MAKE_O){
        if(current_index != -1)remove_from_inventory(current_index);
    }

    if(current_index != -1){
        int speed = animated_img_player.speed;
        if(inventory[current_index].type == PISTOL && inventory[current_index].is_in_inventory){
            animated_img_player = animated_img_player_pistol;
        }
        else{
            animated_img_player = animated_img_player_bear_hands;
        }
        animated_img_player.speed = speed;
    }


}

void remove_from_inventory(int index){
    if(inventory[index].type == PACKAGE && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        for(int z = 0; z < (int) (sizeof(packages) / sizeof(packages[0])); z++){
            if(packages[z].is_in_inventory){
                packages[z].is_dead = false;
                packages[z].is_in_inventory = false;
                packages[z].y = player.y;
                packages[z].x = player.x;
                packages[z].hitbox_x = packages[z].x+(packages[z].width/2);
                packages[z].hitbox_y = packages[z].y+(packages[z].height/2);
                packages[z].idle_time = inventory[index].idle_time;
                animated_img_player.speed +=1;
                int teleport_index = is_on_teleport(player);
                if(teleport_index != -1){
                    packages[z].is_dead = true;
                    teleports[teleport_index].is_dead = true;
                    teleports[teleport_index].is_in_inventory = true;
                    teleports[teleport_index].idle_time = teleports[teleport_index].idle_time *5;
                }
                break;
            }
        }
    }
    if(inventory[index].type == PISTOL && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        for(int z = 0; z < (int) (sizeof(pistols) / sizeof(pistols[0])); z++){
            if(pistols[z].is_in_inventory){
                pistols[z].is_dead = false;
                pistols[z].is_in_inventory = false;
                pistols[z].y = player.y;
                pistols[z].x = player.x;
                pistols[z].hitbox_x = pistols[z].x+(pistols[z].width/2);
                pistols[z].hitbox_y = pistols[z].y+(pistols[z].height/2);
                pistols[z].idle_time = inventory[index].idle_time;
                break;
            }
        }
    }
    if(inventory[index].type == AMMO && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        for(int z = 0; z < (int) (sizeof(ammo) / sizeof(ammo[0])); z++){
            if(ammo[z].is_in_inventory){
                ammo[z].is_dead = false;
                ammo[z].is_in_inventory = false;
                ammo[z].y = player.y;
                ammo[z].x = player.x;
                ammo[z].hitbox_x = ammo[z].x+(ammo[z].width/2);
                ammo[z].hitbox_y = ammo[z].y+(ammo[z].height/2);
                ammo[z].idle_time = inventory[index].idle_time;
                break;
            }
        }
    }  
    if(inventory[index].type == PIZZA && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        for(int z = 0; z < (int) (sizeof(pizza) / sizeof(pizza[0])); z++){
            if(pizza[z].is_in_inventory){
                pizza[z].is_dead = false;
                pizza[z].is_in_inventory = false;
                pizza[z].y = player.y;
                pizza[z].x = player.x;
                pizza[z].hitbox_x = pizza[z].x+(pizza[z].width/2);
                pizza[z].hitbox_y = pizza[z].y+(pizza[z].height/2);
                pizza[z].idle_time = inventory[index].idle_time;
                break;
            }
        }
    }
    if(inventory[index].type == WATCH && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        watch.is_in_inventory = false;
        watch.is_dead = false;
        watch.x = player.x;
        watch.y = player.y;
        watch.hitbox_x = watch.x+(watch.width/2);
        watch.hitbox_y = watch.y+(watch.height/2);
    }
    if(inventory[index].type == BOOTS && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        boots.is_in_inventory = false;
        boots.is_dead = false;
        boots.x = player.x;
        boots.y = player.y;
        boots.hitbox_x = boots.x+(boots.width/2);
        boots.hitbox_y = watch.y+(watch.height/2);
    }
    if(inventory[index].type == BACKPACK && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        backpack.is_in_inventory = false;
        backpack.is_dead = false;
        backpack.x = player.x;
        backpack.y = player.y;
        backpack.hitbox_x = backpack.x+(backpack.width/2);
        backpack.hitbox_y = backpack.y+(backpack.height/2);
        int count = count_packages();
        while(count > 0){
            animated_img_player.speed += 1;
            count--;
        }
    }
    if(inventory[index].type == GRENADE && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        for(int z = 0; z < (int) (sizeof(grenades) / sizeof(grenades[0])); z++){
            if(grenades[z].is_in_inventory){
                grenades[z].is_dead = false;
                grenades[z].is_in_inventory = false;
                grenades[z].y = player.y;
                grenades[z].x = player.x;
                grenades[z].hitbox_x = grenades[z].x+(grenades[z].width/2);
                grenades[z].hitbox_y = grenades[z].y+(grenades[z].height/2);
                grenades[z].idle_time = inventory[index].idle_time;
                break;
            }
        }
    }
    if(inventory[index].type == BONE && inventory[index].is_in_inventory){
        inventory[index].is_in_inventory = false;
        if(bone.is_in_inventory){
            bone.is_dead = false;
            bone.is_in_inventory = false;
            bone.x = player.x;
            bone.y = player.y;
            bone.hitbox_x = bone.x+(bone.width/2);
            bone.hitbox_y = bone.y+(bone.height/2);
            bone.idle_time = inventory[index].idle_time;
        }
    }
}

void move_player(){
    bool is_idle = true;
    player.no_img += 1;
    for(int i = 0; i < 4; i++){
        if(move[i] != STILL){
            is_idle = false;
            player.direction = move[i];
            player.prev_direction = move[i];
            manage_collision(&player, animated_img_player.speed);
        }
    }
    if(is_idle){
        player.direction = STILL;
        player.no_img = 0;
    }
}

void draw_player(){
    switch(player.direction){
        case UP:
            draw(animated_img_player.up[player.no_img%8], player);
            break;
        case LEFT:
            draw(animated_img_player.left[player.no_img%8], player);
            break;
        case RIGHT:
            draw(animated_img_player.right[player.no_img%8], player);
            break;
        case DOWN:
            draw(animated_img_player.down[player.no_img%8], player);
            break;
        case STILL:
            player.direction = player.prev_direction;
            draw_player();
        default:
            break;
    }
}

void kill_workers(){
    for (int i = 0; i < (int) (sizeof(workers) / sizeof(workers[0])); i++){
        if(!workers[i].is_dead){
            if(manage_hitbox(workers[i], player)){
                workers[i].is_dead = true;
            }
        }
    }
}


void remove_item_from_ground(img_t character){
    for(int i = 0; i < (int) (sizeof(pistols) / sizeof(pistols[0])); i++){
        if(!pistols[i].is_dead && !pistols[i].is_in_inventory){
            if(manage_hitbox(pistols[i], character)){
                int index = is_inventory_full();
                if(index != -1){
                    pistols[i].is_in_inventory = true;
                    inventory[index].colors_32 = pistols[i].colors_32;
                    inventory[index].width = pistols[i].width;
                    inventory[index].height = pistols[i].height;
                    inventory[index].is_in_inventory = pistols[i].is_in_inventory;
                    inventory[index].type = PISTOL;
                    inventory[index].idle_time = pistols[i].idle_time;
                }
            }
        }
        if(!ammo[i].is_dead && !ammo[i].is_in_inventory){
            if(manage_hitbox(ammo[i], character)){
                int index = is_inventory_full();
                if(index != -1){
                    ammo[i].is_in_inventory = true;
                    inventory[index].colors_32 = ammo[i].colors_32;
                    inventory[index].width = ammo[i].width;
                    inventory[index].height = ammo[i].height;
                    inventory[index].is_in_inventory = ammo[i].is_in_inventory;
                    inventory[index].type = AMMO;
                    inventory[index].idle_time = ammo[i].idle_time;
                }
            }
        }
        if(!pizza[i].is_dead && !pizza[i].is_in_inventory){
            if(manage_hitbox(pizza[i], character)){
                int index = is_inventory_full();
                if(index != -1){
                    pizza[i].is_in_inventory = true;
                    inventory[index].colors_32 = pizza[i].colors_32;
                    inventory[index].width = pizza[i].width;
                    inventory[index].height = pizza[i].height;
                    inventory[index].is_in_inventory = pizza[i].is_in_inventory;
                    inventory[index].type = PIZZA;
                    inventory[index].idle_time = pizza[i].idle_time;
                }
            }
        }
        if(!grenades[i].is_dead && !grenades[i].is_in_inventory){
            if(manage_hitbox(grenades[i], character)){
                int index = is_inventory_full();
                if(index != -1){
                    grenades[i].is_in_inventory = true;
                    inventory[index].colors_32 = grenades[i].colors_32;
                    inventory[index].width = grenades[i].width;
                    inventory[index].height = grenades[i].height;
                    inventory[index].is_in_inventory = grenades[i].is_in_inventory;
                    inventory[index].type = GRENADE;
                    inventory[index].idle_time = grenades[i].idle_time;
                }
            }
        }
    }
    if(!watch.is_dead && !watch.is_in_inventory){
        if(manage_hitbox(watch, character)){
            int index = is_inventory_full();
            if(index != -1){
                watch.is_in_inventory = true;
                inventory[index].colors_32 = watch.colors_32;
                inventory[index].width = watch.width;
                inventory[index].height = watch.height;
                inventory[index].is_in_inventory = watch.is_in_inventory;
                inventory[index].type = WATCH;
            }
        }
    }
    if(!boots.is_dead && !boots.is_in_inventory){
        if(manage_hitbox(boots, character)){
            int index = is_inventory_full();
            if(index != -1){
                boots.is_in_inventory = true;
                inventory[index].colors_32 = boots.colors_32;
                inventory[index].width = boots.width;
                inventory[index].height = boots.height;
                inventory[index].is_in_inventory = boots.is_in_inventory;
                inventory[index].type = BOOTS;
            }
        }
    }
    if(!backpack.is_dead && !backpack.is_in_inventory){
        if(manage_hitbox(backpack, character)){
            int index = is_inventory_full();
            if(index != -1){
                backpack.is_in_inventory = true;
                inventory[index].colors_32 = backpack.colors_32;
                inventory[index].width = backpack.width;
                inventory[index].height = backpack.height;
                inventory[index].is_in_inventory = backpack.is_in_inventory;
                inventory[index].type = BACKPACK;
                int count = count_packages();
                while(count > 0){
                    if(animated_img_player.speed != 0)animated_img_player.speed -= 1;
                    count--;
                }
            }
        }
    }
    if(!bone.is_dead && !bone.is_in_inventory){
        if(manage_hitbox(bone, character)){
            int index = is_inventory_full();
            if(index != -1){
                bone.is_in_inventory = true;
                inventory[index].colors_32 = bone.colors_32;
                inventory[index].width = bone.width;
                inventory[index].height = bone.height;
                inventory[index].is_in_inventory = bone.is_in_inventory;
                inventory[index].type = BONE;
            }
        }
    }
}

void remove_package(img_t character){
    for(int i = 0; i < (int) (sizeof(packages) / sizeof(packages[0])); i++){
        if(!packages[i].is_dead && !packages[i].is_in_inventory){
            if(manage_hitbox(packages[i], character)){
                int index = is_inventory_full();
                if(index != -1){
                    packages[i].is_in_inventory = true;
                    inventory[index].colors_32 = packages[i].colors_32;
                    inventory[index].width = packages[i].width;
                    inventory[index].height = packages[i].height;
                    inventory[index].is_in_inventory = packages[i].is_in_inventory;
                    inventory[index].type = PACKAGE;
                    if(animated_img_player.speed != 0)animated_img_player.speed -=1;
                    inventory[index].idle_time = packages[i].idle_time;
                }
                
            }
        }
    }
}

int is_on_teleport(img_t character){
    for(int i = 0; i < (int) (sizeof(teleports) / sizeof(teleports[0])); i++){
        if(!teleports[i].is_dead){
            if(manage_hitbox(teleports[i], character)){
                return i;
            }
        }
    }
    return -1;
}



