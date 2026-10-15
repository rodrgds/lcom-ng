#include "bone.h"


void draw_bone(){
    if(!bone.is_dead && !bone.is_in_inventory){
        draw(bone, bone);
    }
    if(bone.is_dead && bone.is_in_inventory && bone.direction == STILL){
        throw_animation_bone(&bone, false);
    }
    if(bone.is_dead && bone.is_in_inventory && bone.direction != STILL){
        draw(bone, bone);
        throw_animation_bone(&bone, true);
    } 
}

void get_bone(int index){
    bone.is_dead = false;
    bone.is_in_inventory = true;
    bone.type = BONE;
    inventory[index].colors_32 = bone.colors_32;
    inventory[index].width = bone.width;
    inventory[index].height = bone.height;
    inventory[index].type = BONE;
}

void throw_animation_bone(img_t *item, bool was_throwed){
    if(was_throwed){
        if(item->idle_time > 0){
            manage_collision(item, 10);
            item->idle_time -= 4;
            if(item->idle_time < 0){
                item->idle_time = 0;
            }
        }
        else{
            item->is_in_inventory = false;
            item->is_dead = false;
            if(!wolf.is_dead){
                int width = bone.hitbox_x - wolf.hitbox_x;
                int height = bone.hitbox_y - wolf.hitbox_y;
                int distance = 0;
                if(!(width == 0 && height == 0)){
                    distance = sqrt(width * width + height * height);
                }
                if(distance >= 150){
                    if(catched != 15)catched = 15;
                }
            }
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

