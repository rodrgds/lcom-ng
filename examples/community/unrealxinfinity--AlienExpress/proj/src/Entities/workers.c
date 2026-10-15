#include "workers.h"


void move_workers(){
    for (int i = 0; i < (int) (sizeof(workers) / sizeof(workers[0])); i++){
        if(workers[i].is_dead)continue;
        int x = x_res*5;
        int y = y_res*5;
        int min = x;
        bool found = false;
        for (int z = 0; z < (int) (sizeof(teleports) / sizeof(teleports[0])); z++){
            if(!teleports[z].is_dead){
                int width = teleports[z].hitbox_x - workers[i].hitbox_x;
                int height = teleports[z].hitbox_y - workers[i].hitbox_y;
                int distance;
                if(abs(width) <= animated_img_workers.speed && abs(height) <= animated_img_workers.speed) {
                    workers[i].x = teleports[z].x;
                    workers[i].y = teleports[z].y;
                    workers[i].hitbox_x = workers[i].x+(workers[i].width/2);
                    workers[i].hitbox_y = workers[i].y+(workers[i].height/2);
                    if((counter_timer % 45) == 0)teleports[z].idle_time -= 1;
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
        for (int z = 0; z < (int) (sizeof(packages) / sizeof(packages[0])); z++){
            if(!packages[z].is_dead && !packages[z].is_in_inventory){
                int width = packages[z].x - workers[i].x;
                int height = packages[z].y - workers[i].y;
                int distance;
                if(abs(width) <= animated_img_workers.speed && abs(height) <= animated_img_workers.speed) {
                    workers[i].x = packages[z].x;
                    workers[i].y = packages[z].y;
                    if(wolf.x == packages[z].x){
                        workers[i].is_dead = true;
                        workers[i].is_in_inventory = false;
                    }
                    workers[i].hitbox_x = workers[i].x+(workers[i].width/2);
                    workers[i].hitbox_y = workers[i].y+(workers[i].height/2);
                    if(packages[z].x <=(x_res/2)){
                        if(packages[z].y <=(y_res/2)){
                            if(packages[z].x <= packages[z].y){
                                packages[z].x -= animated_img_workers.speed /2;
                                packages[z].hitbox_x -= animated_img_workers.speed/2;
                            }
                            else{
                                packages[z].y -= animated_img_workers.speed /2;
                                packages[z].hitbox_y -= animated_img_workers.speed/2;
                            }
                        }
                        else{
                            if(packages[z].x <= (y_res-packages[z].y)){
                                packages[z].x -= animated_img_workers.speed /2;
                                packages[z].hitbox_x -= animated_img_workers.speed/2;
                            }
                            else{
                                packages[z].y += animated_img_workers.speed /2;
                                packages[z].hitbox_y += animated_img_workers.speed/2;
                            }
                        }
                    }
                    else{
                        if(packages[z].y <=(y_res/2)){
                            if((x_res-packages[z].x) <= packages[z].y){
                                packages[z].x += animated_img_workers.speed /2;
                                packages[z].hitbox_x += animated_img_workers.speed/2;
                            }
                            else{
                                packages[z].y -= animated_img_workers.speed /2;
                                packages[z].hitbox_y -= animated_img_workers.speed/2;
                            }
                        }
                        else{
                            if((x_res - packages[z].x) <= (y_res-packages[z].y)){
                                packages[z].x += animated_img_workers.speed /2;
                                packages[z].hitbox_x += animated_img_workers.speed/2;
                            }
                            else{
                                packages[z].y += animated_img_workers.speed /2;
                                packages[z].hitbox_y += animated_img_workers.speed/2;
                            }
                        }
                    }
                    if((packages[z].x <= 1) || ((packages[z].x+packages[z].width) >= (x_res +10)) || (packages[z].y <= 1) || ((packages[z].y+packages[z].height) >= (y_res +10))){
                        packages[z].is_dead = true;
                    }
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
        if(found)continue;
        
        
        if(player.direction == STILL){
            int speed = 4;
            if(count == 2)speed = 2;
            int width = player.hitbox_x - workers[i].hitbox_x;
            int height = player.hitbox_y - workers[i].hitbox_y;

            int distance;
            int final_x = 0;
            int final_y = 0;
            if(abs(width) <= 10 && abs(height) <= 10) {
                workers[i].x = player.hitbox_x-10;
                workers[i].y = player.hitbox_y-10;
                workers[i].hitbox_x = workers[i].x+(workers[i].width/2);
                workers[i].hitbox_y = workers[i].y+(workers[i].height/2);
                continue;
            }
            else{
                distance = sqrt(width*width + height*height);
                final_x = (width*speed)/distance;
                final_y = (height*speed)/distance;
            }
            if(distance < min){
                if(final_x > 0) workers[i].direction = RIGHT;
                else if(final_x < 0) {
                    workers[i].direction = LEFT;
                    final_x = abs(final_x);
                }
                if(counter_timer % 5 == 0)workers[i].no_img += 1;
                if(!is_occupied(workers[i], final_x))manage_collision(&workers[i], final_x);


                if(final_y>0)workers[i].direction = DOWN;
                else if(final_y <0) {
                    workers[i].direction = UP;
                    final_y = abs(final_y);
                }
                if(!is_occupied(workers[i], final_y)) manage_collision(&workers[i], final_y);

                draw_workers(workers[i]);
                continue;
            }
        }
        if(min != (x_res*5)){
            int final_x = (x*animated_img_workers.speed)/min;
            int final_y = (y*animated_img_workers.speed)/min;
            if(final_x > 0) workers[i].direction = RIGHT;
            else if(final_x < 0) {
                workers[i].direction = LEFT;
                final_x = abs(final_x);
            }
            if(counter_timer % 5 == 0)workers[i].no_img += 1;
            if(!is_occupied(workers[i], final_x))manage_collision(&workers[i], final_x);


            if(final_y>0)workers[i].direction = DOWN;
            else if(final_y <0) {
                workers[i].direction = UP;
                final_y = abs(final_y);
            }
            if(!is_occupied(workers[i], final_y)) manage_collision(&workers[i], final_y);

        }
        else if(min == (x_res*5)){
                if(counter_timer % 5 == 0)workers[i].no_img += 1;
                int speed = animated_img_workers.speed/2;
                int width = workers[i].hitbox_x - player.hitbox_x;
                int height = workers[i].hitbox_y - player.hitbox_y;

                int distance;
                int final_x = 0;
                int final_y = 0;
                if(abs(width) <= speed && abs(height) <= speed) {
                    workers[i].x = player.x+speed;
                    workers[i].y = player.y+speed;
                    workers[i].hitbox_x = workers[i].x+(workers[i].width/2);
                    workers[i].hitbox_y = workers[i].y+(workers[i].height/2);
                    draw_workers(workers[i]);
                    continue;
                }
                else {
                    distance = sqrt(width*width + height*height);
                    if(distance <= 100)speed = 6;
                    else if(distance <= 200)speed = 4;
                    else if(distance <= 300)speed = 2;
                    else speed = 0;
                    final_x = (width*speed)/distance;
                    final_y = (height*speed)/distance;
                } 
                if(final_x > 0){
                    workers[i].direction = RIGHT;
                }
                else if(final_x < 0)workers[i].direction = LEFT;
                if(!is_occupied(workers[i], final_x)) manage_collision(&workers[i], final_x);
                
                if(final_y > 0)workers[i].direction = DOWN;
                else if(final_y<0)workers[i].direction = UP;
                if(!is_occupied(workers[i], final_y)) manage_collision(&workers[i], final_y);
        }
        draw_workers(workers[i]);
    }
}

void draw_workers(img_t worker){
        switch (worker.direction) {
            case UP:
                draw(animated_img_workers.up[worker.no_img % 8], worker);
                break;
            case DOWN:
                draw(animated_img_workers.down[worker.no_img % 8], worker);
                break;
            case LEFT:
                draw(animated_img_workers.left[worker.no_img % 8], worker);
                break;
            case RIGHT:
                draw(animated_img_workers.right[worker.no_img % 8], worker);
                break;
            default:
                break;
        }
}
