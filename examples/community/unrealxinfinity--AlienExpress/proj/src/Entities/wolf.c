#include "wolf.h"

void draw_wolf(img_t wolf){
    switch (wolf.direction) {
        case UP:
            draw(animated_img_wolf.up[wolf.no_img % 8], wolf);
            break;
        case DOWN:
            draw(animated_img_wolf.down[wolf.no_img % 8], wolf);
            break;
        case LEFT:
            draw(animated_img_wolf.left[wolf.no_img % 8], wolf);
            break;
        case RIGHT:
            draw(animated_img_wolf.right[wolf.no_img % 8], wolf);
            break;
        case STILL:
            draw(animated_img_wolf.down[1], wolf);
            wolf.no_img = 0;
            break;
        default:
            break;
    }
}
void draw_friendly_wolf(img_t wolf){
    switch (wolf.direction) {
        case UP:
            draw(animated_img_friendly_wolf.up[wolf.no_img % 8], wolf);
            break;
        case DOWN:
            draw(animated_img_friendly_wolf.down[wolf.no_img % 8], wolf);
            break;
        case LEFT:
            draw(animated_img_friendly_wolf.left[wolf.no_img % 8], wolf);
            break;
        case RIGHT:
            draw(animated_img_friendly_wolf.right[wolf.no_img % 8], wolf);
            break;
        case STILL:
            draw(animated_img_friendly_wolf.down[1], wolf);
            wolf.no_img = 0;
            break;
        default:
            break;
    }
}


void move_wolf(){
    if(!wolf.is_dead && !wolf.is_in_inventory){
        int width = player.hitbox_x - wolf.hitbox_x;
        int height = player.hitbox_y - wolf.hitbox_y;
    

        if(!bone.is_dead && !bone.is_in_inventory){
            width = bone.hitbox_x - wolf.hitbox_x;
            height = bone.hitbox_y - wolf.hitbox_y;
        }

        int distance = 0;
        int final_x = 0;
        int final_y = 0;
        
        if((abs(width) <= 3 && abs(height) <= 3)) {
            distance = 0;
            final_x = 0;
            final_y = 0;
            if(!(!bone.is_dead && !bone.is_in_inventory))player_lives -=2;
            else{
                if(catched){
                    if((counter_timer % 30) == 0){
                        if(catched == 15)wolf.idle_time -= 1;
                        catched -= 1;
                        if(catched == 0){
                            wolf.is_dead = true;
                        }
                    }
                    
                }
                if(wolf.idle_time == 0){
                    wolf.is_in_inventory = true;
                    
                }
            }
        }
        else if(!bone.is_dead && bone.is_in_inventory && get_current_index() != -1 && (abs(width) <= 200 && abs(height) <= 200)){
            if(inventory[get_current_index()].type == BONE && inventory[get_current_index()].is_in_inventory){
                distance = 0;
                final_x = 0;
                final_y = 0;
                wolf.direction = STILL;
            }
            else {
                distance = sqrt(width*width + height*height);
                final_x = (width*3)/distance;
                final_y = (height*3)/distance;
                if(counter_timer % 5 == 0) wolf.no_img += 1;
            }
        }
        else {
            distance = sqrt(width*width + height*height);
            final_x = (width*3)/distance;
            final_y = (height*3)/distance;
            if(counter_timer % 5 == 0) wolf.no_img += 1;
        }

        if(final_x > 0) wolf.direction = RIGHT;
        else if(final_x < 0) {
            wolf.direction = LEFT;
            final_x = abs(final_x);
        }
        if(!is_occupied(wolf, final_x)){
            manage_collision(&wolf, final_x);
        }


        if(final_y>0)wolf.direction = DOWN;
        else if(final_y <0) {
            wolf.direction = UP;
            final_y = abs(final_y);
        }
        if(!is_occupied(wolf, final_y)){
            manage_collision(&wolf, final_y);
        }
        draw_wolf(wolf);
    }
    if(!wolf.is_dead && wolf.is_in_inventory){
        bool found = false;
        bool found_teleporter = false;
        int min = x_res;
        int x = 0;
        int y = 0;
        int package = -1;
        for (int z = 0; z < (int) (sizeof(packages) / sizeof(packages[0])); z++){
            if(!packages[z].is_dead && !packages[z].is_in_inventory){
                int width = packages[z].x - wolf.x;
                int height = packages[z].y - wolf.y;
                int distance;
                if(abs(width) <= 3 && abs(height) <= 3) {
                    wolf.x = packages[z].x;
                    wolf.y = packages[z].y;
                    wolf.hitbox_x = wolf.x+(wolf.width/2);
                    wolf.hitbox_y = wolf.y+(wolf.height/2);
                    package = z;
                    found = true;
                    break;
                }
                else {
                    distance = sqrt(width*width + height*height);
                    if(distance < min){
                        min = distance;
                        x = width;
                        y = height;
                    }
                }
                
            }
        }
        if(found){
            for (int z = 0; z < (int) (sizeof(teleports) / sizeof(teleports[0])); z++){
                if(!teleports[z].is_dead && !teleports[z].is_in_inventory){
                    int width = teleports[z].x - wolf.x;
                    int height = teleports[z].y - wolf.y;
                    int distance;
                    if(abs(width) <= 3 && abs(height) <= 3) {
                        wolf.x = teleports[z].x;
                        wolf.y = teleports[z].y;
                        wolf.hitbox_x = wolf.x+(wolf.width/2);
                        wolf.hitbox_y = wolf.y+(wolf.height/2);
                        int index = is_on_teleport(wolf);
                        if(index != -1){
                            teleports[index].is_dead = true;
                            teleports[index].is_in_inventory = true;
                            teleports[index].idle_time = teleports[index].idle_time *5;
                            packages[package].is_dead = true;
                            packages[package].is_in_inventory = false;
                        }
                        found_teleporter = true;
                        break;
                    }
                    else {
                        distance = sqrt(width*width + height*height);
                        if(distance < min){
                            min = distance;
                            x = width;
                            y = height;
                        }
                        found_teleporter = true;
                    }
                } 
            }
            if(!found_teleporter){
                x = player.x - wolf.x;
                y = player.y - wolf.y;
                if(!(abs(x)<= 50 && abs(y) <= 50)){
                    min = sqrt(x*x + y*y);
                    found_teleporter = true;
                }
                else {
                    found = false;
                    min = x_res;
                }
            }
        }
        if(found || min != x_res){
            if(counter_timer % 5 == 0) wolf.no_img += 1;

            x = (x*3)/min;
            y = (y*3)/min;
            if(x > 0) wolf.direction = RIGHT;
            else if(x < 0) {
                wolf.direction = LEFT;
                x = abs(x);
            }
            if(!is_occupied(wolf, x)){
                manage_collision(&wolf, x);
                if(found_teleporter){
                    packages[package].x = wolf.x;
                    packages[package].y = wolf.y;
                    packages[package].hitbox_x = packages[package].x +(packages[package].width/2);
                    packages[package].hitbox_y = packages[package].y +(packages[package].height /2);
                }
            }


            if(y>0)wolf.direction = DOWN;
            else if(y <0) {
                wolf.direction = UP;
                y = abs(y);
            }
            if(!is_occupied(wolf, y)){
                manage_collision(&wolf, y);
                if(found_teleporter){
                    packages[package].x = wolf.x;
                    packages[package].y = wolf.y;
                    packages[package].hitbox_x = packages[package].x +(packages[package].width/2);
                    packages[package].hitbox_y = packages[package].y +(packages[package].height /2);
                }
            }
        }
        else wolf.direction = STILL;
        
        draw_friendly_wolf(wolf);
    }
}
