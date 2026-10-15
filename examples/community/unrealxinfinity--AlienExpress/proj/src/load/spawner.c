#include "spawner.h"


void spawn(){
    int time = minutes*60+seconds;
    if(time < 10){
        spawn_monsters(2);
        spawn_packages(5);
    }
    else if(time < 20){
        spawn_monsters(2);
        spawn_packages(5);
        spawn_teleports(15);
    }
    else if(time < 90){
        spawn_workers(15);
        spawn_monsters(3);
        spawn_packages(4);
        spawn_teleports(15);
    }
    else{
        spawn_monsters(3);
        spawn_packages(4);
        spawn_teleports(12);
        spawn_workers(12);
        spawn_wolf(15);
    }
}


void spawn_monsters(int rate){
    if(rate == 0)rate = 1;
    int monster_count = rand() % (rate);
    if(monster_count >= 19)monster_count = 19;
    for (int i = 0; i < (int) (sizeof(enemies_lv1) / sizeof(enemies_lv1[0])); i++){
        if(monster_count == 0)break;
        if(enemies_lv1[i].is_dead){
            int x = rand()%2;
            int y = rand()%2;
            int real_x = 0;
            int real_y = 0;
            if(x == 0 && y == 0){
                real_x = 0;
                int y_height = rand() % (y_res);
                real_y = y_height;
                enemies_lv1[i].direction = RIGHT;
            }
            if(x == 1 && y == 0){
                real_x = x_res;
                int y_height = rand() % (y_res);
                real_y = y_height;
                enemies_lv1[i].direction = LEFT;
            }
            if(x == 0 && y == 1){
                real_y = 0;
                int x_width = rand() % (x_res);
                real_x = x_width;
                enemies_lv1[i].direction = DOWN;
            }
            if(x == 1 && y == 1){
                real_y = y_res;
                int x_width = rand() % (x_res);
                real_x = x_width;
                enemies_lv1[i].direction = UP;
            }
            enemies_lv1[i].is_dead = false;
            enemies_lv1[i].x = real_x;
            enemies_lv1[i].y = real_y;
            enemies_lv1[i].hitbox_x = (enemies_lv1[i].width/2)+real_x;
            enemies_lv1[i].hitbox_y = (enemies_lv1[i].height/2)+real_y;
            monster_count--;
        }
    }
}
void spawn_workers(int rate){
    if(rate == 0)rate = 1;
    int monster_count = rand() % (rate);
    if(monster_count == 2){
        monster_count -= 1;
        for (int i = 0; i < (int) (sizeof(workers) / sizeof(workers[0])); i++){
            if(monster_count == 0)break;
            if(workers[i].is_dead){
                int x = rand()%2;
                int y = rand()%2;
                int real_x = 0;
                int real_y = 0;
                if(x == 0 && y == 0){
                    real_x = 0;
                    int y_height = rand() % (y_res);
                    real_y = y_height;
                    workers[i].direction = RIGHT;
                }
                if(x == 1 && y == 0){
                    real_x = x_res;
                    int y_height = rand() % (y_res);
                    real_y = y_height;
                    workers[i].direction = LEFT;
                }
                if(x == 0 && y == 1){
                    real_y = 0;
                    int x_width = rand() % (x_res);
                    real_x = x_width;
                    workers[i].direction = DOWN;
                }
                if(x == 1 && y == 1){
                    real_y = y_res;
                    int x_width = rand() % (x_res);
                    real_x = x_width;
                    workers[i].direction = UP;
                }
                workers[i].is_dead = false;
                workers[i].x = real_x;
                workers[i].y = real_y;
                workers[i].hitbox_x = (workers[i].width/2)+real_x;
                workers[i].hitbox_y = (workers[i].height/2)+real_y;
                monster_count--;
            }
        }
    }
}
void spawn_wolf(int rate){
    if(wolf.is_dead){
        if(rate == 0)rate = 1;
        int monster_count = rand() % (rate);
        if(monster_count == 2){
            monster_count -= 1;
            int x = rand()%2;
            int y = rand()%2;
            int real_x = 0;
            int real_y = 0;
            if(x == 0 && y == 0){
                real_x = 0;
                int y_height = rand() % (y_res);
                real_y = y_height;
                wolf.direction = RIGHT;
            }
            if(x == 1 && y == 0){
                real_x = x_res;
                int y_height = rand() % (y_res);
                real_y = y_height;
                wolf.direction = LEFT;
            }
            if(x == 0 && y == 1){
                real_y = 0;
                int x_width = rand() % (x_res);
                real_x = x_width;
                wolf.direction = DOWN;
            }
            if(x == 1 && y == 1){
                real_y = y_res;
                int x_width = rand() % (x_res);
                real_x = x_width;
                wolf.direction = UP;
            }
            catched = 14;
            wolf.is_dead = false;
            wolf.is_in_inventory = false;
            wolf.idle_time = (rand() % 3) + 1;
            wolf.x = real_x;
            wolf.y = real_y;
            wolf.hitbox_x = (wolf.width/2)+real_x;
            wolf.hitbox_y = (wolf.height/2)+real_y;          
        }
    }
}
void spawn_packages(int rate){
    if(rate == 0)rate = 1;
    int random = rand() % rate;
    int package_count;
    if(random == 1)package_count = 1;
    else package_count = 0;
    
    for (int i = 0; i < (int) (sizeof(packages) / sizeof(packages[0])); i++){
        if(package_count == 0)break;
        if(packages[i].is_dead && !packages[i].is_in_inventory){
            int real_x = rand() % (x_res-32);
            int real_y = rand() % (y_res-32);
            packages[i].x = real_x;
            packages[i].y = real_y;
            packages[i].hitbox_x = (packages[i].width/2)+real_x;
            packages[i].hitbox_y = (packages[i].height/2)+real_y;
            package_count--;
            packages[i].is_dead = false;
        }
    }
}
void spawn_teleports(int rate){
    if(rate == 0)rate = 1;
    int random = rand() % rate;
    int teleport_count;
    if(random == 1)teleport_count = 1;
    else teleport_count = 0;
    for (int i = 0; i < (int) (sizeof(teleports) / sizeof(teleports[0])); i++){
        if(teleport_count == 0)break;
        if(teleports[i].is_dead && !teleports[i].is_in_inventory){
            int teleport_time = (rand() % 30)+30;
            int real_x = rand() % (x_res-32);
            int real_y = rand() % (y_res-32);
            teleports[i].x = real_x;
            teleports[i].y = real_y;
            teleports[i].hitbox_x = (teleports[i].width/2)+real_x;
            teleports[i].hitbox_y = (teleports[i].height/2)+real_y;
            teleport_count--;
            teleports[i].is_dead = false;
            teleports[i].is_in_inventory = false;
            teleports[i].idle_time = teleport_time;
        }
    }
}
void spawn_bullet(int index){
    for(int i = 0; i < (int) (sizeof(bullets) / sizeof(bullets[0])); i++){
        if(bullets[i].is_dead && !bullets[i].is_in_inventory){
            bullets[i].is_dead = false;
            bullets[i].x = player.hitbox_x;
            bullets[i].y = player.hitbox_y;
            bullets[i].hitbox_x = (bullets[i].width/2)+bullets[i].x;
            bullets[i].hitbox_y = (bullets[i].height/2)+bullets[i].y+4;
            bullets[i].direction = player.direction;
            if(bullets[i].direction == STILL){
                bullets[i].direction = player.prev_direction;
            }
            inventory[index].idle_time -= 1;
            break;
        }
    }
}
