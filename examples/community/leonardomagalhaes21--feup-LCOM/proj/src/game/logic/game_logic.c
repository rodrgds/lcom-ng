#include "game_logic.h"
#include <stdlib.h>
#include <math.h>

bool is_falling = true;
float cuphead_offset = 0;
enemy monsters[10];
enemy monsters_fly[2];
extern vbe_mode_info_t info;
extern uint16_t mode;
extern GameState currentState;
bool facingLeft=false;
extern int score;
extern int counter_timer;
int renew=0;



void update_player_logic(player *player, MouseCursor *mouse, bool key_a_pressed, bool key_d_pressed, bool key_w_pressed, int8_t *speed_x, int8_t *speed_y, int *unvulnerability) {

    if (key_d_pressed || key_a_pressed) {
        if (key_a_pressed) {
            facingLeft = true;
        }
        else if (key_d_pressed) {
            facingLeft = false;
        }
        if (cuphead_offset == 0){
            player->sprite = cuphead1;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 1){
            player->sprite = cuphead2;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 2){
            player->sprite = cuphead3;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 3){
            player->sprite = cuphead4;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 4){
            player->sprite = cuphead5;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 5){
            player->sprite = cuphead6;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 6){
            player->sprite = cuphead7;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 7){
            player->sprite = cuphead8;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 8){
            player->sprite = cuphead9;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 9){
            player->sprite = cuphead10;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 10){
            player->sprite = cuphead11;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 11){
            player->sprite = cuphead12;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 12){
            player->sprite = cuphead13;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 13){
            player->sprite = cuphead14;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 14){
            player->sprite = cuphead15;
            cuphead_offset += 0.5;
        }
        else if (cuphead_offset == 15){
            player->sprite = cuphead16;
            cuphead_offset = 0;
        }
        else{
            cuphead_offset += 0.5;
        } 
    } else {
        player->sprite = cupheadstand;
        cuphead_offset = 0;
    }

    int8_t jump_force, gravity;
    if(mode == 0x14C) {
        gravity = 4 ;
        jump_force = -40; 
    }

    else if (mode == 0x115) {
        gravity = 4;
        jump_force = -35; 
    }

    else if (mode == 0x11A){
        gravity = 4 ;
        jump_force = -40; 
    }

    else if (mode == 0x110) {
        gravity = 4*info.YResolution/DEFAULT_Y_RESOLUTION_14C;
        jump_force = -40*info.YResolution/DEFAULT_Y_RESOLUTION_14C; 
    }
    else {
        return;
    }
    
    if (is_falling) {
        *speed_y += gravity;
    }

    player->y += *speed_y;

    if (player->y >= 570*info.YResolution/DEFAULT_Y_RESOLUTION_14C) {
        player->y = 570*info.YResolution/DEFAULT_Y_RESOLUTION_14C;
        *speed_y = 0;
        is_falling = false;
    }

    if (key_a_pressed) {
        *speed_x = -8; 
    } else if (key_d_pressed) {
        *speed_x = 8; 
    } else {
        *speed_x = 0; 
    }
    player->x += *speed_x;

    if (key_w_pressed && !is_falling) {
        is_falling = true;
        *speed_y = jump_force; 
    }
    if(player->x < -30 *info.XResolution/DEFAULT_X_RESOLUTION_14C){
        player->x = -30 *info.XResolution/DEFAULT_X_RESOLUTION_14C;
    }
    if(player->x > 1045 *info.XResolution/DEFAULT_X_RESOLUTION_14C){
        player->x = 1045 *info.XResolution/DEFAULT_X_RESOLUTION_14C;
    }
    
    if (key_a_pressed || facingLeft) {
        draw_reverse_sprite(player->sprite, player->x, player->y);
    }
    else {
        draw_sprite(player->sprite, player->x, player->y);
    }

    for (int i = 0; i < 10; i++) {
        if (monsters[i].alive) {  

            bool flag = check_collision(player->sprite, player->x, player->y, monsters[i].sprite, monsters[i].x, monsters[i].y);
            bool flag2= check_enemy_bullet_collisions(player,unvulnerability);

            if ((flag || flag2) && *unvulnerability > 60) {
            player->life--;
            *unvulnerability = 0;
            monsters[i].alive=false;
            if(player->life <= 0){
                currentState = SCOREBOARD;
                for(int i=0;i<10;i++){
                        monsters[i].alive=false;
                    }
                for (int j=0;j<2;j++){
                    monsters_fly[j].alive=false;
                    monsters_fly[j].shot=NULL;

                }
                //save_score(score);
                player->life = 5;
                player->x = 400*info.XResolution/DEFAULT_X_RESOLUTION_14C;
                player->y = 571*info.YResolution/DEFAULT_Y_RESOLUTION_14C;
            }
            }
        }
    }
    
}



void update_enemy_logic(MouseCursor *mouse, bool create_enemy,player *player) {
    if (create_enemy) {
        for (int i = 0; i < 10; i++) {
            if (!monsters[i].alive) {
                int spawn_side = rand() % 2; 
                int spawn_line = rand() % 2; 
                int spawn_x;
                int spawn_y = (spawn_line == 0) ? FIRST_LINE_Y : SECOND_LINE_Y;
                
                if (spawn_side == 0) {
                    spawn_x = -monsters[i].sprite->width; 
                    monsters[i].speed_x = 4; 
                } else {
                    spawn_x = info.XResolution;
                    monsters[i].speed_x = -4;
                }
                reviveEnemy(&monsters[i], spawn_x, spawn_y*info.YResolution/DEFAULT_Y_RESOLUTION_14C);
                monsters[i].speed_y = 0; 
                break;
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        if (monsters[i].alive) {
           

            if (monsters[i].y >= 600*info.YResolution/DEFAULT_Y_RESOLUTION_14C) {
                monsters[i].y = 600*info.YResolution/DEFAULT_Y_RESOLUTION_14C;
            }

            monsters[i].x += monsters[i].speed_x;

            if (monsters[i].x < 0) {
                monsters[i].x = 0;
                monsters[i].speed_x = abs(monsters[i].speed_x); 
            } else if (monsters[i].x + monsters[i].sprite->width >= info.XResolution) {
                monsters[i].x = info.XResolution - monsters[i].sprite->width; 
                monsters[i].speed_x = -abs(monsters[i].speed_x); 
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        if (monsters_fly[i].alive) {
            if (monsters_fly[i].shot == NULL) {
                monsters_fly[i].shot = createEnemyBullet(monsters_fly[i].x, monsters_fly[i].y+200, player->x, player->y, 1, bala_inimigo);
            } else {
                moveEnemyBullet(monsters_fly[i].shot);
                if (monsters_fly[i].shot->active) {
                    draw_sprite(monsters_fly[i].shot->sprite, monsters_fly[i].shot->x, monsters_fly[i].shot->y);
                } else {
                    destroyEnemyBullets(monsters_fly[i].shot);
                    monsters_fly[i].shot = NULL;
                }
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        if (monsters[i].alive) {

            if( monsters[i].speed_x <= 0) {
                draw_sprite(monsters[i].sprite, monsters[i].x, monsters[i].y);
            }
            else {
                draw_reverse_sprite(monsters[i].sprite, monsters[i].x, monsters[i].y);
            }
        
        }
    }
    for(int i=0; i<2;i++){
        if (monsters_fly[i].alive && monsters_fly[i].life > 0){
            if(i == 0){
                monsters_fly[i].y = FLYMONS1_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C + sin(counter_timer / 3) * 10;
            }else{
                monsters_fly[i].y = FLYMONS2_Y*info.YResolution/DEFAULT_Y_RESOLUTION_14C + sin(counter_timer / 3) * 10;
            }
            
            if(monsters_fly[i].x > player->x){
                draw_reverse_sprite(monsters_fly[i].sprite, monsters_fly[i].x, monsters_fly[i].y);
            }else{
                draw_sprite(monsters_fly[i].sprite, monsters_fly[i].x, monsters_fly[i].y);
            }
        }
            
    }
}





void update_bullet_logic(bullet_node **head) {
    bullet_node *prev = NULL;
    bullet_node *current = *head;
    
    while (current != NULL) {
        moveBullet(current->shot);
        draw_sprite(current->shot->sprite, current->shot->x, current->shot->y);

        if (current->shot->x < 0 || current->shot->x >= info.XResolution || current->shot->y < 0 || current->shot->y >= info.YResolution) {
            bullet_node *to_remove = current;
            current = current->next;

            if (prev == NULL) {
                *head = current;
            } else {
                prev->next = current;
            }

            destroyBulletNode(to_remove);
        } else {
            prev = current;
            current = current->next;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (monsters[i].alive) {
            current = *head;
            prev = NULL; 
            while (current != NULL) {
                bool flag = check_collision(current->shot->sprite, current->shot->x, current->shot->y, monsters[i].sprite, monsters[i].x, monsters[i].y);
                if (flag) {
                    monsters[i].alive = false;
                    bullet_node *to_remove = current;

                    if (prev == NULL) {
                        *head = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    destroyBulletNode(to_remove);
                    break;
                } else {
                    prev = current;
                    current = current->next;
                }
            }
        }
    }
    for(int i=0; i<2;i++) {
        if (monsters_fly[i].alive) {
            current = *head;
            prev = NULL;
            while (current != NULL) {
                bool flag = check_collision(current->shot->sprite, current->shot->x, current->shot->y, monsters_fly[i].sprite, monsters_fly[i].x, monsters_fly[i].y);
                if (flag) {
                    monsters_fly[i].life -= 3;
                    if (monsters_fly[i].life <= 0) {
                        monsters_fly[i].alive = false;
                        renew=0;
                    }
                    bullet_node *to_remove = current;

                    if (prev == NULL) {
                        *head = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    current = current->next;
                    destroyBulletNode(to_remove);
                    break;
                } else {
                    prev = current;
                    current = current->next;
                }
            }

        }
    }
    
}



bool check_enemy_bullet_collisions(player *player, int *unvulnerability) {
    for (int i = 0; i < 2; i++) {
        if (monsters_fly[i].alive && monsters_fly[i].shot != NULL && monsters_fly[i].shot->active) {
            bool hit = check_collision(player->sprite, player->x, player->y, monsters_fly[i].shot->sprite, monsters_fly[i].shot->x, monsters_fly[i].shot->y);
            if (hit && *unvulnerability > 60) {
                monsters_fly[i].shot->active = false;
                return hit;
            }
        }
    }
    return false;
}



void spawn_dead_enemies(){
    int spawn= rand() % 2; 
    if(monsters_fly[spawn].alive==false){
        monsters_fly[spawn].shot=NULL;
        monsters_fly[spawn].alive=true;
        monsters_fly[spawn].life=6;
    }
    else{
        if(spawn==0 || monsters_fly[1].alive==false){
            monsters_fly[1].shot=NULL;
            monsters_fly[1].alive=true;
            monsters_fly[1].life = 6;
        }
        else if(spawn==1 || monsters_fly[0].alive==false){
            monsters_fly[0].shot=NULL;

            monsters_fly[0].alive=true;
            monsters_fly[0].life = 6;
        }
            
    }

    
}

