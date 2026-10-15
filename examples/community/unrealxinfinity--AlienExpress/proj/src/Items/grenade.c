#include "grenade.h"

void draw_grenade(int index){
    if(!grenades[index].is_dead && !grenades[index].is_in_inventory){
        draw(grenades[index], grenades[index]);
    }
    if(grenades[index].is_dead && grenades[index].is_in_inventory && grenades[index].direction == STILL){
        throw_animation_grenade(&grenades[index], false);
    }
    if(grenades[index].is_dead && grenades[index].is_in_inventory && grenades[index].direction != STILL){
        draw(grenades[index], grenades[index]);
        throw_animation_grenade(&grenades[index], true);
    }
}


void get_grenade(int index){
    for(int z = 0; z < (int) (sizeof(grenades) / sizeof(grenades[0])); z++){
        if(!grenades[z].is_in_inventory && grenades[z].is_dead){
            grenades[z].is_in_inventory = true;
            grenades[z].is_dead = false;
            grenades[z].idle_time = 3;
            grenades[z].direction = STILL;
            inventory[index].colors_32 = grenades[z].colors_32;
            inventory[index].width = grenades[z].width;
            inventory[index].height = grenades[z].height;
            inventory[index].type = GRENADE;
            inventory[index].idle_time = 0;
            break;
        }
    }
}

void throw_animation_grenade(img_t *item, bool was_throwed){
    if(was_throwed){
        if(item->idle_time <= -3 && item->type == GRENADE){
            item->is_in_inventory = false;
            item->is_dead = true;
            item->direction = STILL;
            int x = item->x-120;
            if(x < 0)x = 0;
            int max_x = item->x + item->width +120;
            if(x > x_res)x=x_res;
            int y = item->y - 120;
            if(y < 0)y = 0;
            int max_y = item->y+item->height+120;
            if(y > y_res)y = y_res;
            
            set_explosion(item->hitbox_x,item->hitbox_y);

            kill_monster_radius(x, y, max_x, max_y);
            if((player.hitbox_x <= max_x && player.hitbox_x >= x) && (player.hitbox_y <= max_y && player.hitbox_y >= y)){
                player_lives -= 20;
                if(player_lives <= 0)player_lives = 0;
            }
        }
        else if(item->idle_time > 0){
            manage_collision(item, 10);
            item->idle_time -= 4;
            if(item->idle_time < 0){
                item->idle_time = 0;
            }
        }
        else{
            if((counter_timer%30)== 0)item->idle_time -= 1;
        }
    }
    else{
        if(get_is_holding_i() && get_throwing_index() == get_current_index()){
            if(item->idle_time < 70) item->idle_time += 4;
        }
        else{
            set_throwing_index(-5);
            item->is_in_inventory = true;
            item->is_dead = true;
            int offset = 3;
            if(player.direction == STILL)item->direction = player.prev_direction;
            else item->direction = player.direction;
            switch(item->direction){
                case UP:
                    item->x = player.x;
                    item->y = player.y-offset*animated_img_player.speed;
                    break;
                case LEFT:
                    item->x = player.x-offset*animated_img_player.speed;
                    item->y = player.y;
                    break;
                case RIGHT:
                    item->x = player.x+player.width+offset*animated_img_player.speed;
                    item->y = player.y;
                    break;
                case DOWN:
                    item->x = player.x;
                    item->y = player.y+player.height+offset*animated_img_player.speed;
                    break;
                default:
                    break;
            }
            item->hitbox_x = item->x +(item->width/2);
            item->hitbox_y = item->y +(item->height/2);
        }
    }
}


void kill_monster_radius(int x, int y, int max_x, int max_y){
    for(int i = 0; i < (int) (sizeof(enemies_lv1) / sizeof(enemies_lv1[0])); i++){
        if(!enemies_lv1[i].is_dead){
            int enemy_x = enemies_lv1[i].hitbox_x;
            int enemy_y = enemies_lv1[i].hitbox_y;
            if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
                enemies_lv1[i].is_dead = true;
                enemies_lv1[i].is_in_inventory = false;
            }
        }

    }
    for(int i = 0; i < (int) (sizeof(workers) / sizeof(workers[0])); i++){
        if(!workers[i].is_dead){
            int enemy_x = workers[i].hitbox_x;
            int enemy_y = workers[i].hitbox_y;
            if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
                workers[i].is_dead = true;
                workers[i].is_in_inventory = false;
            }
        }

    }
    for(int i = 0; i < (int) (sizeof(packages) / sizeof(packages[0])); i++){
        if(!packages[i].is_dead && !packages[i].is_in_inventory){
            int enemy_x = packages[i].hitbox_x;
            int enemy_y = packages[i].hitbox_y;
            if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
                packages[i].is_dead = true;
                max_health -= 5;
                player_lives -= 5;
                if(player_lives < 0)player_lives = 0;
            }
        }
    }
    for(int i = 0; i < (int) (sizeof(teleports) / sizeof(teleports[0])); i++){
        if(!teleports[i].is_dead && !teleports[i].is_in_inventory){
            int enemy_x = teleports[i].hitbox_x;
            int enemy_y = teleports[i].hitbox_y;
            if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
                teleports[i].is_dead = true;
            }
        }
    }
    for(int i = 0; i < (int) (sizeof(pistols) / sizeof(pistols[0])); i++){
        if(!pistols[i].is_dead && !pistols[i].is_in_inventory){
            int enemy_x = pistols[i].hitbox_x;
            int enemy_y = pistols[i].hitbox_y;
            if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
                pistols[i].is_dead = true;
            }
        }
        if(!pizza[i].is_dead && !pizza[i].is_in_inventory){
            int enemy_x = pizza[i].hitbox_x;
            int enemy_y = pizza[i].hitbox_y;
            if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
                pizza[i].is_dead = true;
            }
        }
        if(!ammo[i].is_dead && !ammo[i].is_in_inventory){
            int enemy_x = ammo[i].hitbox_x;
            int enemy_y = ammo[i].hitbox_y;
            if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
                ammo[i].is_dead = true;
            }
        }
        if(!grenades[i].is_dead && !grenades[i].is_in_inventory){
            int enemy_x = grenades[i].hitbox_x;
            int enemy_y = grenades[i].hitbox_y;
            if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
                grenades[i].is_dead = true;
            }
        }
    }
    if(!wolf.is_dead){
        int enemy_x = wolf.hitbox_x;
        int enemy_y = wolf.hitbox_y;
        if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
            wolf.is_dead = true;
        }
    }
    if(!backpack.is_dead && !backpack.is_in_inventory){
        int enemy_x = backpack.hitbox_x;
        int enemy_y = backpack.hitbox_y;
        if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
            backpack.is_dead = true;
        }
    }
    if(!boots.is_dead && !boots.is_in_inventory){
        int enemy_x = boots.hitbox_x;
        int enemy_y = boots.hitbox_y;
        if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
            boots.is_dead = true;
        }
    }
    if(!watch.is_dead && !watch.is_in_inventory){
        int enemy_x = watch.hitbox_x;
        int enemy_y = watch.hitbox_y;
        if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
            watch.is_dead = true;
        }
    }
    if(!bone.is_dead && !bone.is_in_inventory){
        int enemy_x = bone.hitbox_x;
        int enemy_y = bone.hitbox_y;
        if((enemy_x <= max_x && enemy_x >= x) && (enemy_y <= max_y && enemy_y >= y)){
            bone.is_dead = true;
        }
    }
}
