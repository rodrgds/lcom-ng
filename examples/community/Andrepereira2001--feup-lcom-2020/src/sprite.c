#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "sprite.h"
#include "video_gr.h"
#include "videocardVars.h"
#include "gameVar.h"
#include "state_machine.h"
#include "game.h"

#include "gameXpm/car1.xpm"
#include "gameXpm/car2.xpm"
#include "gameXpm/car3.xpm"
#include "gameXpm/car4.xpm"
#include "gameXpm/car2.1.xpm"
#include "gameXpm/car2.2.xpm"
#include "gameXpm/car2.3.xpm"
#include "gameXpm/car2.4.xpm"
#include "gameXpm/background.xpm"
#include "gameXpm/gold.xpm"
#include "gameXpm/stone.xpm"
#include "gameXpm/obsidian.xpm"
#include "gameXpm/rope.xpm"
#include "gameXpm/rope_end.xpm"
#include "gameXpm/claw_open.xpm"
#include "gameXpm/claw_close.xpm"
#include "gameXpm/clock.xpm"

#include "menuXpm/cursor_pick.xpm"
#include "menuXpm/cursor_pick2.xpm"

#include "menuXpm/0.xpm"
#include "menuXpm/1.xpm"
#include "menuXpm/2.xpm"
#include "menuXpm/3.xpm"
#include "menuXpm/4.xpm"
#include "menuXpm/5.xpm"
#include "menuXpm/6.xpm"
#include "menuXpm/7.xpm"
#include "menuXpm/8.xpm"
#include "menuXpm/9.xpm"
#include "menuXpm/slash.xpm"
#include "menuXpm/twopoints.xpm"

#include "menuXpm/score_menu.xpm"
#include "menuXpm/leaderboard_menu.xpm"
#include "menuXpm/loading.xpm"

#include "menuXpm/leaderboard.xpm"
#include "menuXpm/menu.xpm"
#include "menuXpm/multiplayer.xpm"
#include "menuXpm/singleplayer.xpm"
#include "menuXpm/exit.xpm"
#include "menuXpm/player1.xpm"
#include "menuXpm/player2.xpm"


Sprite car_sp; 
Sprite golds_arr[N_GOLDS];
Sprite claw_sp;
Sprite rope_sp; // zero or more ocurrences to reach the car
Sprite clock_sp;
Sprite car_sp2;
Sprite claw_sp2;

CursorSprite cursor_sp;

MenuSprite zero_sp;
MenuSprite one_sp;
MenuSprite two_sp;
MenuSprite three_sp;
MenuSprite four_sp;
MenuSprite five_sp;
MenuSprite six_sp;
MenuSprite seven_sp;
MenuSprite eight_sp;
MenuSprite nine_sp;
MenuSprite slash_sp;
MenuSprite twopoints_sp;

MenuSprite leaderboard_menu_sp;
MenuSprite score_sp;
MenuSprite menu_sp;
MenuSprite singleplayer_sp;
MenuSprite multiplayer_sp;
MenuSprite leaderboard_sp;
MenuSprite exit_sp;
MenuSprite loading_sp;
MenuSprite player1_sp;
MenuSprite player2_sp;

/**
 * @brief used to count the passed ticks of the clock help us implemented the animations
 * 
 */
int animate_tick = 0;

Sprite *create_sprite(xpm_map_t xpm, int x, int y,int xspeed, int yspeed, enum xpm_image_type enumtype) {

    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    xpm_image_t img;
    if( sp == NULL )
        return NULL;

    img.bytes = xpm_load(xpm, enumtype, &img);
    sp->map = (uint32_t*) img.bytes;
    sp->width = img.width;
    sp->height = img.height;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    sp->x = x;
    sp->y = y;
    sp->n_animations = 1;
    sp->map_animates = (uint32_t**) malloc(sp->n_animations*sizeof(img.bytes));
    sp->map_animates[0] = (uint32_t*)img.bytes;
    sp->actual_animation_idx = 0;

    return sp;
}

MenuSprite * create_menu_sprite(xpm_map_t xpm,enum xpm_image_type enumtype){
    MenuSprite *sp = (MenuSprite *) malloc (sizeof(MenuSprite));
    xpm_image_t img;
    if( sp == NULL )
        return NULL;
    img.bytes = xpm_load(xpm, enumtype, &img);
    sp->map = (uint32_t*) img.bytes;
    sp->height = img.height;
    sp->width = img.width;
    return sp;
}

CursorSprite * create_cursor_sprite(xpm_map_t xpm, int x, int y, enum xpm_image_type enumtype){
    CursorSprite *sp = (CursorSprite *) malloc (sizeof(CursorSprite));
    xpm_image_t img;
    if( sp == NULL )
        return NULL;
    img.bytes = xpm_load(xpm, enumtype, &img);
    sp->map = (uint32_t*) img.bytes;
    sp->n_animations = 1;
    sp->map_animates = (uint32_t**) malloc(sp->n_animations*sizeof(img.bytes));
    sp->map_animates[0] = (uint32_t*)img.bytes;
    sp->actual_animation_idx = 0;
    sp->height = img.height;
    sp->width = img.width;
    sp->x = x;
    sp->y = y;
    return sp;
}

int create_sprite_background(xpm_map_t xpm,enum xpm_image_type enumtype){

    xpm_image_t img;
    img.bytes = xpm_load(xpm, enumtype, &img);
    if(img.bytes==NULL){
        return 1;
    }

    uint32_t * colors = (uint32_t*) img.bytes;
    for(int y=0; y<img.height; y++){
        for(int x=0; x<img.width; x++){
            vg_paint_pixel(x,y, *colors); 
            colors ++;
        }
    }
    copy_buffer_to_background_buffer();
    clean_buffer();
    return 0;
}

void destroy_sprite(Sprite *sp) {
    free(sp->map_animates);
    if( sp == NULL ) return;
    if( sp ->map ) free(sp->map);
        free(sp);
    sp = NULL; // XXX: pointer is passed by value
    // should do this @ the caller
}

void destroy_menu_sprite(MenuSprite * sp){
    if( sp == NULL ) return;
    if( sp ->map ) free(sp->map);
    free(sp);
    sp = NULL;
}

int draw_sprite(Sprite *sp){
    uint32_t * colors = sp->map; 
    for(int y=0; y<sp->height; y++){
        for(int x=0; x<sp->width; x++){
            vg_paint_pixel(x+sp->x,y+sp->y, *colors); 
            colors ++;
        }
    }
    return 0;

}

int draw_menu_sprite(int x, int y, MenuSprite *sp){
    uint32_t * colors = sp->map; 
    for(int yy=0; yy < sp->height; yy++){
        for(int xx=0; xx < sp->width; xx++){
            vg_paint_pixel((xx+x) % get_x_max(),yy+y, *colors); 
            colors ++;
        }
    }
    return 0;
}

int draw_cursor_sprite(CursorSprite *sp){
    uint32_t * colors = sp->map; 
    for(int y=0; y < sp->height; y++){
        for(int x=0; x < sp->width; x++){
            vg_paint_pixel(x+sp->x,y+sp->y, *colors); 
            colors ++;
        }
    }
    return 0;
}

int move_sprite(Sprite *sp,bool limitChange){
    if(sp->xspeed!= 0){ //displacement horizontal
        if(sp->x + sp->xspeed < 0){
            if(limitChange){
                if(sp->x + sp->width < 0){
                    sp->x = get_x_max();
                    while(sp->actual_animation_idx != 0 && sp->n_animations > 1){
                        change_sprite_animation(sp);
                    }
                }
            } 
            else{
                sp->xspeed = -sp->x;
            }
        }
        else if(sp->x + sp->xspeed > get_x_max()-sp->width && !limitChange){
            sp->xspeed = get_x_max() - sp->width - sp->x;
        }
        sp->x = sp->x + sp->xspeed;
    }
    if(sp->yspeed!=0){ //displacement vertical
        if(sp->y + sp->yspeed <0){
            sp->yspeed= -sp->y;
        }
        else if(sp->y + sp->yspeed > get_y_max()-sp->height){
            sp->yspeed = get_y_max() - sp->height - sp->y;
        }
        sp->y = sp->y + sp->yspeed;
    }
    return 0;
}

int move_cursor(int x, int y){
    cursor_sp.x += x;
    if(cursor_sp.x<0){
        cursor_sp.x = 0;
    }
    else if(cursor_sp.x > get_x_max()-cursor_sp.width){
        cursor_sp.x = get_x_max()-cursor_sp.width;
    }

    cursor_sp.y -= y;
    if(cursor_sp.y < 0){
        cursor_sp.y = 0;
    }
    else if(cursor_sp.y > get_y_max()-cursor_sp.height){
        cursor_sp.y = get_y_max()-cursor_sp.height;
    }
    return 0;
}

int set_claw_yspeed(int direction){
    claw_sp.yspeed = direction * CLAWSPEED;
    return 0;
}

int set_claw2_yspeed(int direction){
    claw_sp2.yspeed = direction * CLAWSPEED;
    return 0;
}

int check_claw_changes(){
    // xspeed depend of car speed
    claw_sp.xspeed = car_sp.xspeed;

    if(claw_sp.yspeed!=0){ //displacement vertical
        if(claw_sp.y + claw_sp.yspeed < car_sp.height + car_sp.y ){
            claw_sp.yspeed= car_sp.height + car_sp.y -claw_sp.y;
        }
    }
    return 0;
}

int check_claw2_changes(){
    // xspeed depend of car speed
    claw_sp2.xspeed = car_sp2.xspeed;

    if(claw_sp2.yspeed!=0){ //displacement vertical
        if(claw_sp2.y + claw_sp2.yspeed < car_sp2.height + car_sp2.y ){
            claw_sp2.yspeed= car_sp2.height + car_sp2.y -claw_sp2.y;
        }
    }
    return 0;
}

int set_car_speed(int direction){
    car_sp.xspeed = SPEED * direction - BACKGROUND_SPEED*2;
    return 0;
}

int set_car2_speed(int direction){
    car_sp2.xspeed = SPEED * direction - BACKGROUND_SPEED*2;
    return 0;
}

MenuSprite* get_sprite_number(int number){
    switch (number)
    {
        case 0:
            return &zero_sp;
            break;
        case 1:
            return &one_sp;
            break;
        case 2:
            return &two_sp;
            break;
        case 3:
            return &three_sp;
            break;
        case 4:
            return &four_sp;
            break;
        case 5:
            return &five_sp;
            break;
        case 6:
            return &six_sp;
            break;
        case 7:
            return &seven_sp;
            break;
        case 8:
            return &eight_sp;
            break;
        case 9:
            return &nine_sp;
            break;
        default:
            return NULL;
            break;
    }
}

/**
 * state: 0 -> catch state  __  1-> drop state
 * type: 0-> indefined __  1-> gold  __  2-> clock
 * idx: indexed of the element in the specified vector
 **/
void handle_sprite_changes(int state,int type, int idx, long long unsigned int* score, int * clock_counter,bool click){
    
    move_sprite(&car_sp,false);
    animate_tick = (animate_tick + 1) % ANIMATION_SPEED; 
    if(car_sp.xspeed!=-2 && animate_tick == 0){
        change_sprite_animation(&car_sp);
    }

    //depend from car changes
    check_claw_changes();

    //catch state
    if(state == 0){
        int coll_idx = -1;
        int coll_type = 0;
        for(int n = 0; n < N_GOLDS; n++){
            if (check_collision(claw_sp.x + 17,claw_sp.y+CLAW_SIZE,claw_sp.width-18-17,10,&golds_arr[n]) == 2){ // claw collison_gold
                coll_idx = n;
                coll_type = 1;
            }
        }
        if(check_collision(claw_sp.x + 17,claw_sp.y+CLAW_SIZE,claw_sp.width-18-17,10,&clock_sp) == 2){ // claw collision clock
            coll_idx = 0;
            coll_type = 2;
        }

        if(coll_idx != -1){
            //changes claw to closed
            change_sprite_animation(&claw_sp);
            if(coll_type == 1){
                golds_arr[coll_idx].xspeed = claw_sp.xspeed;
                golds_arr[coll_idx].yspeed = claw_sp.yspeed;
            }
            else if(coll_type == 2){
                clock_sp.xspeed = claw_sp.xspeed;
                clock_sp.yspeed = claw_sp.yspeed;
            }
            set_drop_state(coll_type,coll_idx);
        }
    }
    //drop state
    else if(state == 1){
        if(type == 1){
            golds_arr[idx].xspeed = claw_sp.xspeed;
            golds_arr[idx].yspeed = claw_sp.yspeed;
            //changes to gold
        }
        else if(type == 2){
            clock_sp.xspeed = claw_sp.xspeed;
            clock_sp.yspeed = claw_sp.yspeed;
        }

        //collect item and generate new pos
        if(claw_sp.y <= car_sp.y + car_sp.height && (claw_sp.x <= car_sp.x + car_sp.width || claw_sp.x + claw_sp.width <= car_sp.x)){
            if(type == 1){ // collect gold
                int new_gold_x;
                int new_gold_y; 
                bool collide = true;
                while(collide){
                    int coll = 0;
                    collide = false;
                    new_gold_x = rand() % (get_x_max() - GOLD_WIDTH - GOLD_WIDTH);
                    new_gold_y =  rand() % (get_y_max() - GOLD_HEIGHT - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        if(n != idx){
                            coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&golds_arr[n]);
                            if (coll == 1 || coll == 2){
                                collide = true;
                                //change_sprite_animation(&golds_arr[idx]);
                                break;
                            }
                        }
                    }
                    coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&clock_sp);
                    if (coll == 1 || coll == 2){
                        collide = true;
                        break;
                    }
                }
                golds_arr[idx].x = new_gold_x;
                golds_arr[idx].y = new_gold_y;
                golds_arr[idx].xspeed= -BACKGROUND_SPEED*2;
                golds_arr[idx].yspeed=0;

                if(golds_arr[idx].actual_animation_idx == 1){
                    *score = *score + SCORE_INCREMENT;
                    // change from gold to obsidian
                    change_sprite_animation(&golds_arr[idx]);
                    // change from obsdian to stone
                    change_sprite_animation(&golds_arr[idx]);
                }
                else if(golds_arr[idx].actual_animation_idx == 2){
                    *clock_counter = *clock_counter - ClOCKDECREMENT;
                     // change from obsdian to stone
                    change_sprite_animation(&golds_arr[idx]);
                }
                else{
                    if((long int)*score - SCORE_DECREMENT >= 0){
                        *score = *score - SCORE_DECREMENT;
                    }
                    else {
                        *score = 0;
                    }
                }
            }
            else if(type == 2){ // collect clock
                int new_clock_x;
                int new_clock_y; 
                bool collide = true;
                while(collide){
                    collide = false;
                    new_clock_x = rand() % (get_x_max() - clock_sp.width - GOLD_WIDTH);
                    new_clock_y =  rand() % (get_y_max() - clock_sp.height - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        int coll = check_collision(new_clock_x,new_clock_y,clock_sp.width,clock_sp.height,&golds_arr[n]);
                        if (coll == 1 || coll == 2){
                            collide = true;
                            break;
                        }
                    }
                }
                clock_sp.x = new_clock_x;
                clock_sp.y = new_clock_y;
                clock_sp.xspeed=-BACKGROUND_SPEED*2;
                clock_sp.yspeed=0;
                *clock_counter += CLOCKINCREMENT;
            }
            //opens claw
            change_sprite_animation(&claw_sp);
            set_catch_state();
        }
    }

    for(int n = 0; n < N_GOLDS; n++){
        int coll = check_collision(cursor_sp.x, cursor_sp.y + 5, cursor_sp.width/2,cursor_sp.height - 5,&golds_arr[n]) ;
        if(coll == 2 || coll == 1){
            if(click && golds_arr[n].actual_animation_idx == 0){
                int r = rand() % STONE_TRANS_RATE;
                change_sprite_animation(&golds_arr[n]);
                if(r == 0){
                    change_sprite_animation(&golds_arr[n]);
                }
            }
        }
        move_sprite(&golds_arr[n],true);
    }
    move_sprite(&clock_sp,true);
    move_sprite(&claw_sp,false);
}

void draw_game_screen(int clock_counter, long long unsigned int score){
    copy_background_buffer();

    draw_sprite(&car_sp);

    for(int i = 0; i < N_GOLDS; i++){
        draw_sprite(&(golds_arr[i]));
    }

    draw_sprite(&clock_sp);
    draw_sprite(&claw_sp);


    for(int i = claw_sp.y - rope_sp.height + 1; i >= (car_sp.height)/2 + (car_sp.height)/4 + car_sp.y - 5; i-= rope_sp.height){
        rope_sp.x = car_sp.x + (car_sp.width/2) - rope_sp.width/2;
        rope_sp.y = i;
        draw_sprite(&rope_sp);
    }


    draw_cursor_sprite(&cursor_sp);

    // WHEN CLOCK_COUNTER REACHES 0 PROGRAM DONT BREAK
    if(clock_counter <= 0){
        clock_counter = 0;
    }
    // CLOCK DRAW
    draw_menu_sprite(CLOCK_POS_X,CLOCK_POS_Y,get_sprite_number(clock_counter % 10));
    draw_menu_sprite(CLOCK_POS_X - NEXT_POS,CLOCK_POS_Y,get_sprite_number((clock_counter/10) % 10));
    draw_menu_sprite(CLOCK_POS_X - 2*NEXT_POS,CLOCK_POS_Y,get_sprite_number((clock_counter/100) % 10));

    // SCORE DRAW
    draw_menu_sprite(SCORE_POS_X + 5*NEXT_POS,SCORE_POS_Y,get_sprite_number(score % 10));
    draw_menu_sprite(SCORE_POS_X + 4*NEXT_POS,SCORE_POS_Y,get_sprite_number((score/10) % 10));
    draw_menu_sprite(SCORE_POS_X + 3*NEXT_POS,SCORE_POS_Y,get_sprite_number((score/100) % 10));
    draw_menu_sprite(SCORE_POS_X + 2*NEXT_POS,SCORE_POS_Y,get_sprite_number((score/1000) % 10));
    draw_menu_sprite(SCORE_POS_X + NEXT_POS,SCORE_POS_Y,get_sprite_number((score/10000) % 10));
    draw_menu_sprite(SCORE_POS_X ,SCORE_POS_Y,get_sprite_number((score/100000) % 10));


    copy_buffer();
}

/**
 * This function handles the alterations in the menu screen
 * Also draw it
 * 
 * return 0 -> menu screen    1-> singleplayer    2-> multiplayer     3-> help     4-> exit
 * 
 **/
int draw_menu_screen(bool click,Date_t * date){
    copy_background_buffer();

    if(car_sp.x == 0){
        set_car_speed(1);
    }
    else if(car_sp.x == get_x_max() - car_sp.width){
        set_car_speed(-1);
    }
    else if(car_sp.xspeed == 0){
        set_car_speed(1);
    }

    draw_sprite(&car_sp);

    draw_menu_sprite(300,0,&menu_sp);

    unsigned int curr_x = 0;
    curr_x += 3*zero_sp.width +5;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height -5,get_sprite_number(date->year%10));
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height -5,get_sprite_number((date->year/10)%10));
    curr_x += slash_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height -15,&slash_sp);
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height -5,get_sprite_number(date->month%10));
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height -5,get_sprite_number((date->month/10)%10));
    curr_x += slash_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height -15,&slash_sp);
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height -5,get_sprite_number(date->month_day%10));
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height -5,get_sprite_number((date->month_day/10)%10));

    curr_x = 3*zero_sp.width +15;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height*2 -20,get_sprite_number(date->seconds%10));
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height*2 -20,get_sprite_number((date->seconds/10)%10));
    curr_x += twopoints_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height*2 -20,&twopoints_sp);
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height*2 -20,get_sprite_number(date->minutes%10));
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height*2 -20,get_sprite_number((date->minutes/10)%10));
    curr_x += twopoints_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height*2 -20,&twopoints_sp);
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height*2 -20,get_sprite_number(date->hours%10));
    curr_x += zero_sp.width + 1;
    draw_menu_sprite(get_x_max() - curr_x ,get_y_max() - zero_sp.height*2 -20,get_sprite_number((date->hours/10)%10));

    if (cursor_sp.y + 5 >= 433 && cursor_sp.y + cursor_sp.height - 5 <= singleplayer_sp.height + 433 && cursor_sp.x >= 436
         && cursor_sp.x + cursor_sp.width/2 <= singleplayer_sp.width + 436){
        draw_menu_sprite(436,433,&singleplayer_sp);
        if(click){
            return 1;
        }
    }
    else if (cursor_sp.y + 5 >= 540 && cursor_sp.y + cursor_sp.height - 5 <= multiplayer_sp.height + 540 && cursor_sp.x >= 437
         && cursor_sp.x + cursor_sp.width/2 <= multiplayer_sp.width + 437){
        draw_menu_sprite(437,540,&multiplayer_sp);
        if(click){
            return 2;
        }
    }
    else if (cursor_sp.y + 5 >= 655 && cursor_sp.y + cursor_sp.height - 5 <= leaderboard_sp.height + 655 && cursor_sp.x >= 430
         && cursor_sp.x + cursor_sp.width/2 <= leaderboard_sp.width + 430){
        draw_menu_sprite(430,655,&leaderboard_sp);
        if(click){
            return 3;
        }
    }
    else if (cursor_sp.y + 5 >= 745 && cursor_sp.y + cursor_sp.height - 5 <= exit_sp.height + 745 && cursor_sp.x >= 518
         && cursor_sp.x + cursor_sp.width/2 <= exit_sp.width + 518){
        draw_menu_sprite(518,745,&exit_sp);
        if(click){
            return 4;
        }
    }
    
    draw_cursor_sprite(&cursor_sp);

    copy_buffer();
    return 0;
}

/**
 * used to draw the score screen
 * 
 **/
void draw_score_screen(long long unsigned int score){
    copy_background_buffer();
    move_sprite(&car_sp,false);

    if(car_sp.x == 0){
        set_car_speed(1);
    }
    else if(car_sp.x == get_x_max() - car_sp.width){
        set_car_speed(-1);
    }
    else if(car_sp.xspeed == 0){
        set_car_speed(1);
    }

    draw_sprite(&car_sp);
    draw_menu_sprite(0,0,&score_sp);

    draw_menu_sprite(M_SCORE_POS_X + 10*NEXT_POS,M_SCORE_POS_Y,get_sprite_number(score % 10));
    draw_menu_sprite(M_SCORE_POS_X + 8*NEXT_POS,M_SCORE_POS_Y,get_sprite_number((score/10) % 10));
    draw_menu_sprite(M_SCORE_POS_X + 6*NEXT_POS,M_SCORE_POS_Y,get_sprite_number((score/100) % 10));
    draw_menu_sprite(M_SCORE_POS_X + 4*NEXT_POS,M_SCORE_POS_Y,get_sprite_number((score/1000) % 10));
    draw_menu_sprite(M_SCORE_POS_X + 2*NEXT_POS,M_SCORE_POS_Y,get_sprite_number((score/10000) % 10));
    draw_menu_sprite(M_SCORE_POS_X ,M_SCORE_POS_Y,get_sprite_number((score/100000) % 10));

    copy_buffer();
}

void draw_multi_score_screen(long long unsigned int score, long long unsigned int score2){
    copy_background_buffer();
    move_sprite(&car_sp,false);

    if(car_sp.x == 0){
        set_car_speed(1);
    }
    else if(car_sp.x == get_x_max() - car_sp.width){
        set_car_speed(-1);
    }
    else if(car_sp.xspeed == 0){
        set_car_speed(1);
    }

    draw_sprite(&car_sp);
    draw_menu_sprite(0,0,&score_sp);  

    draw_menu_sprite(PLAYER1_SCORE_POS_X, PLAYER1_SCORE_POS_Y,&player1_sp);
    draw_menu_sprite(PLAYER1_SCORE_POS_X + 16*NEXT_POS,PLAYER1_SCORE_POS_Y,get_sprite_number(score % 10));
    draw_menu_sprite(PLAYER1_SCORE_POS_X + 14*NEXT_POS,PLAYER1_SCORE_POS_Y,get_sprite_number((score/10) % 10));
    draw_menu_sprite(PLAYER1_SCORE_POS_X + 12*NEXT_POS,PLAYER1_SCORE_POS_Y,get_sprite_number((score/100) % 10));
    draw_menu_sprite(PLAYER1_SCORE_POS_X + 10*NEXT_POS,PLAYER1_SCORE_POS_Y,get_sprite_number((score/1000) % 10));
    draw_menu_sprite(PLAYER1_SCORE_POS_X + 8*NEXT_POS,PLAYER1_SCORE_POS_Y,get_sprite_number((score/10000) % 10));
    draw_menu_sprite(PLAYER1_SCORE_POS_X + 6*NEXT_POS,PLAYER1_SCORE_POS_Y,get_sprite_number((score/100000) % 10));

    draw_menu_sprite(PLAYER2_SCORE_POS_X, PLAYER2_SCORE_POS_Y,&player2_sp);
    draw_menu_sprite(PLAYER2_SCORE_POS_X + 16*NEXT_POS,PLAYER2_SCORE_POS_Y,get_sprite_number(score2 % 10));
    draw_menu_sprite(PLAYER2_SCORE_POS_X + 14*NEXT_POS,PLAYER2_SCORE_POS_Y,get_sprite_number((score2/10) % 10));
    draw_menu_sprite(PLAYER2_SCORE_POS_X + 12*NEXT_POS,PLAYER2_SCORE_POS_Y,get_sprite_number((score2/100) % 10));
    draw_menu_sprite(PLAYER2_SCORE_POS_X + 10*NEXT_POS,PLAYER2_SCORE_POS_Y,get_sprite_number((score2/1000) % 10));
    draw_menu_sprite(PLAYER2_SCORE_POS_X + 8*NEXT_POS,PLAYER2_SCORE_POS_Y,get_sprite_number((score2/10000) % 10));
    draw_menu_sprite(PLAYER2_SCORE_POS_X + 6*NEXT_POS,PLAYER2_SCORE_POS_Y,get_sprite_number((score2/100000) % 10));

    copy_buffer();
}

void draw_leaderboard_screen(struct Point * point){
    copy_background_buffer();
    move_sprite(&car_sp,false);

    if(car_sp.x == 0){
        set_car_speed(1);
    }
    else if(car_sp.x == get_x_max() - car_sp.width){
        set_car_speed(-1);
    }
    else if(car_sp.xspeed == 0){
        set_car_speed(1);
    }

    draw_sprite(&car_sp);

    draw_menu_sprite(0,0,&leaderboard_menu_sp);

    int y = 270;
    for (int i = 0; i < 5; i++){

        Date_t date = point[i].date;
        long long unsigned int score = point[i].score;
        unsigned int curr_x = 0;
        curr_x += 15*zero_sp.width;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2 + y,get_sprite_number((date.month_day/10)%10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2 + y,get_sprite_number(date.month_day%10));
        curr_x += slash_sp.width + 1;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2 - 10 + y,&slash_sp);
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2  + y,get_sprite_number((date.month/10)%10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2  + y,get_sprite_number(date.month%10));
        curr_x += slash_sp.width + 1;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2  - 10+ y,&slash_sp);
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2  + y,get_sprite_number((date.year/10)%10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x , (zero_sp.height + 5)*i*2  + y,get_sprite_number(date.year%10));

        curr_x += zero_sp.width + 20;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2  + y,get_sprite_number((date.hours/10)%10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x , (zero_sp.height + 5)*i*2  + y,get_sprite_number(date.hours%10));
        curr_x += twopoints_sp.width + 15;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2  + y,&twopoints_sp);
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2   + y,get_sprite_number((date.minutes/10)%10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x , (zero_sp.height + 5)*i*2  + y,get_sprite_number(date.minutes%10));
        curr_x += twopoints_sp.width + 15;
        draw_menu_sprite(curr_x ,(zero_sp.height + 5)*i*2  + y,&twopoints_sp);
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x , (zero_sp.height + 5)*i*2  + y,get_sprite_number((date.seconds/10)%10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x , (zero_sp.height + 5)*i*2  + y,get_sprite_number(date.seconds%10));

        curr_x += 100;
        draw_menu_sprite(curr_x, (zero_sp.height + 5)*i*2  + y, get_sprite_number((score/100000) % 10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x, (zero_sp.height + 5)*i*2  + y, get_sprite_number((score/10000) % 10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x, (zero_sp.height + 5)*i*2  + y, get_sprite_number((score/1000) % 10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x, (zero_sp.height + 5)*i*2  + y, get_sprite_number((score/100) % 10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x, (zero_sp.height + 5)*i*2  + y, get_sprite_number((score/10) % 10));
        curr_x += zero_sp.width + 1;
        draw_menu_sprite(curr_x, (zero_sp.height + 5)*i*2  + y, get_sprite_number(score % 10));
    }

    copy_buffer();
}

int load_sprites(){
    if (create_sprite_background((xpm_map_t)back,XPM_8_8_8_8) != 0){
        printf("Error creating background\n");
        return 1;
    }

    copy_background_buffer();

    car_sp = *create_sprite((xpm_map_t)car1,0,282,0,0,XPM_8_8_8_8);
    add_animate_sprite(&car_sp,(xpm_map_t)car2,XPM_8_8_8_8);
    add_animate_sprite(&car_sp,(xpm_map_t)car3,XPM_8_8_8_8);
    add_animate_sprite(&car_sp,(xpm_map_t)car4,XPM_8_8_8_8);

    car_sp2 = *create_sprite((xpm_map_t)car21,0,282,0,0,XPM_8_8_8_8);
    add_animate_sprite(&car_sp2,(xpm_map_t)car22,XPM_8_8_8_8);
    add_animate_sprite(&car_sp2,(xpm_map_t)car23,XPM_8_8_8_8);
    add_animate_sprite(&car_sp2,(xpm_map_t)car24,XPM_8_8_8_8);

    //LOAD GOLDS
    for(int i = 0; i < N_GOLDS; i++){
        golds_arr[i] = *create_sprite((xpm_map_t)stone, 0,0,-BACKGROUND_SPEED*2,0,XPM_8_8_8_8);
        add_animate_sprite(&golds_arr[i],(xpm_map_t)gold,XPM_8_8_8_8);
        add_animate_sprite(&golds_arr[i],(xpm_map_t)obsidian,XPM_8_8_8_8);
    }

    //LOAD CLOCK
    clock_sp = *create_sprite((xpm_map_t)clock_xpm, 0,0,-BACKGROUND_SPEED*2,0,XPM_8_8_8_8);


    claw_sp = *create_sprite((xpm_map_t)claw_open,(car_sp.width/2) - 21 ,(car_sp.height) + car_sp.y,0,0,XPM_8_8_8_8);
    add_animate_sprite(&claw_sp,(xpm_map_t)claw_close, XPM_8_8_8_8);

    claw_sp2 = *create_sprite((xpm_map_t)claw_open,(car_sp.width/2) - 21 ,(car_sp2.height) + car_sp.y,0,0,XPM_8_8_8_8);
    add_animate_sprite(&claw_sp2,(xpm_map_t)claw_close, XPM_8_8_8_8);

    rope_sp = *create_sprite((xpm_map_t)rope,(car_sp.width/2) - 3,(car_sp.height)/2 + car_sp.y,0,0,XPM_8_8_8_8);

    cursor_sp = *create_cursor_sprite((xpm_map_t)cursor_pick,500,500,XPM_8_8_8_8);
    add_animate_sprite_cursor(&cursor_sp,(xpm_map_t)cursor_pick2,XPM_8_8_8_8);
    
    // MENU SPRITES
    zero_sp = *create_menu_sprite((xpm_map_t)zero,XPM_8_8_8_8);
    one_sp = *create_menu_sprite((xpm_map_t)one,XPM_8_8_8_8);
    two_sp = *create_menu_sprite((xpm_map_t)two,XPM_8_8_8_8);
    three_sp = *create_menu_sprite((xpm_map_t)three,XPM_8_8_8_8);
    four_sp = *create_menu_sprite((xpm_map_t)four,XPM_8_8_8_8);
    five_sp = *create_menu_sprite((xpm_map_t)five,XPM_8_8_8_8);
    six_sp = *create_menu_sprite((xpm_map_t)six,XPM_8_8_8_8);
    seven_sp = *create_menu_sprite((xpm_map_t)seven,XPM_8_8_8_8);
    eight_sp = *create_menu_sprite((xpm_map_t)eight,XPM_8_8_8_8);
    nine_sp = *create_menu_sprite((xpm_map_t)nine,XPM_8_8_8_8); 
    slash_sp = *create_menu_sprite((xpm_map_t)slash,XPM_8_8_8_8); 
    twopoints_sp = *create_menu_sprite((xpm_map_t)twopoints,XPM_8_8_8_8); 

    leaderboard_menu_sp = *create_menu_sprite((xpm_map_t)leaderboard_menu,XPM_8_8_8_8); 
    score_sp = *create_menu_sprite((xpm_map_t)score_menu,XPM_8_8_8_8); 
    menu_sp = *create_menu_sprite((xpm_map_t)menu,XPM_8_8_8_8); 
    multiplayer_sp = *create_menu_sprite((xpm_map_t)multiplayer,XPM_8_8_8_8);
    singleplayer_sp = *create_menu_sprite((xpm_map_t)singleplayer,XPM_8_8_8_8);
    leaderboard_sp = *create_menu_sprite((xpm_map_t)leaderboard,XPM_8_8_8_8);
    exit_sp = *create_menu_sprite((xpm_map_t)exit_botton,XPM_8_8_8_8);
    loading_sp = *create_menu_sprite((xpm_map_t)loading,XPM_8_8_8_8);
    player1_sp = *create_menu_sprite((xpm_map_t)player1,XPM_8_8_8_8);
    player2_sp = *create_menu_sprite((xpm_map_t)player2,XPM_8_8_8_8);

    return 0;
}

void destroy_sprites(){
    destroy_sprite(&car_sp);
    destroy_sprite(&car_sp2);

    for(int n = 0; n < N_GOLDS; n++){
        destroy_sprite(&golds_arr[n]);
    }

    destroy_sprite(&claw_sp);
    destroy_sprite(&claw_sp2);
    destroy_sprite(&rope_sp);
    
    free(cursor_sp.map_animates);
    free(cursor_sp.map);

    destroy_menu_sprite(&zero_sp);
    destroy_menu_sprite(&one_sp);
    destroy_menu_sprite(&two_sp);
    destroy_menu_sprite(&three_sp);
    destroy_menu_sprite(&four_sp);
    destroy_menu_sprite(&five_sp);
    destroy_menu_sprite(&six_sp);
    destroy_menu_sprite(&seven_sp);
    destroy_menu_sprite(&eight_sp);
    destroy_menu_sprite(&nine_sp);
    destroy_menu_sprite(&slash_sp);
    destroy_menu_sprite(&twopoints_sp);

    destroy_menu_sprite(&leaderboard_menu_sp);
    destroy_menu_sprite(&score_sp);
    destroy_menu_sprite(&menu_sp);
    destroy_menu_sprite(&multiplayer_sp);
    destroy_menu_sprite(&singleplayer_sp);
    destroy_menu_sprite(&leaderboard_sp);
    destroy_menu_sprite(&exit_sp);
    destroy_menu_sprite(&player1_sp);
    destroy_menu_sprite(&player2_sp);
    destroy_menu_sprite(&loading_sp);
}

void add_animate_sprite(Sprite *sp, xpm_map_t xpm,enum xpm_image_type enumtype){

    xpm_image_t img;
    img.bytes = xpm_load(xpm, enumtype, &img);
    sp->n_animations++;
    sp->map_animates = (uint32_t**) realloc(sp->map_animates,sizeof(img.size)*sp->n_animations);
    sp->map_animates[sp->n_animations-1] = (uint32_t*)img.bytes;

}

void add_animate_sprite_cursor(CursorSprite *sp, xpm_map_t xpm,enum xpm_image_type enumtype){

    xpm_image_t img;
    img.bytes = xpm_load(xpm, enumtype, &img);
    sp->n_animations++;
    sp->map_animates = (uint32_t**) realloc(sp->map_animates,sizeof(img.size)*sp->n_animations);
    sp->map_animates[sp->n_animations-1] = (uint32_t*)img.bytes;

}

void change_sprite_animation(Sprite *sp){
    sp->actual_animation_idx = (sp->actual_animation_idx + 1) % sp->n_animations;
    sp->map = sp->map_animates[sp->actual_animation_idx];
}

void change_sprite_animation_cursor(){
    cursor_sp.actual_animation_idx = (cursor_sp.actual_animation_idx + 1) % cursor_sp.n_animations;
    cursor_sp.map = cursor_sp.map_animates[cursor_sp.actual_animation_idx];
}

int check_collision(int x, int y, int x_size, int y_size , Sprite * sp) {
    if(x >= sp->x && y >= sp->y && x <= sp->x + sp->width && y <= sp->y + sp->height){
        if(x + x_size <= sp->x + sp->width && y + y_size <= sp->y + sp->height ){
            return 2;
        }
    }

    if(x >= sp->x && y >= sp->y && x <= sp->x + sp->width && y <= sp->y + sp->height){
        return 1;
    }
    else if(x + x_size >= sp->x && y >= sp->y && x + x_size <= sp->x + sp->width && y <= sp->y + sp->height){
        return 1;
    }
    else if(x >= sp->x && y + y_size >= sp->y && x <= sp->x + sp->width && y + y_size <= sp->y + sp->height){
        return 1;
    }
    else if(x + x_size >= sp->x && y + y_size >= sp->y && x + x_size <= sp->x + sp->width && y + y_size <= sp->y + sp->height){
        return 1;
    }
    else if(x + x_size/2 >= sp->x && y + y_size/2 >= sp->y && x + x_size/2 <= sp->x + sp->width && y + y_size/2 <= sp->y + sp->height){
        return 1;
    }


    return 0;
}

void reset_sprites(bool useSeed){
    car_sp.x = 500;
    car_sp.y = 282;
    car_sp.xspeed = -BACKGROUND_SPEED*2;
    car_sp.yspeed = 0;

    car_sp2.x = 500;
    car_sp2.y = 282;
    car_sp2.xspeed = -BACKGROUND_SPEED*2;
    car_sp2.yspeed = 0;

    claw_sp.x = car_sp.x + (car_sp.width/2) - 21;
    claw_sp.y = (car_sp.height) + car_sp.y;
    claw_sp.xspeed = car_sp.xspeed;
    claw_sp.yspeed = 0;
    while(claw_sp.actual_animation_idx != 0){
        change_sprite_animation(&claw_sp);
    }

    claw_sp2.x = car_sp2.x + (car_sp2.width/2) - 21;
    claw_sp2.y = (car_sp2.height) + car_sp2.y;
    claw_sp2.xspeed = car_sp2.xspeed;
    claw_sp2.yspeed = 0;
    while(claw_sp2.actual_animation_idx != 0){
        change_sprite_animation(&claw_sp2);
    }

    for(int i = 0; i < N_GOLDS; i++){
        int new_gold_x;
        int new_gold_y; 
        bool collide = true;
        while(collide){
            collide = false;
            if(useSeed){
                new_gold_x = myRand() % (get_x_max() - GOLD_WIDTH- GOLD_WIDTH) +  GOLD_WIDTH/2;
                new_gold_y = myRand() % (get_y_max() - GOLD_HEIGHT - GOLD_MIN_Y) + GOLD_MIN_Y;
            }
            else{
                new_gold_x = rand() % (get_x_max() - GOLD_WIDTH- GOLD_WIDTH) +  GOLD_WIDTH/2;
                new_gold_y =  rand() % (get_y_max() - GOLD_HEIGHT - GOLD_MIN_Y) + GOLD_MIN_Y;
            }
            for(int n = 0; n < i; n++){
                int coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&golds_arr[n]);
                if (coll == 1 || coll == 2){
                    collide = true;
                    break;
                }
            }
        }
        golds_arr[i].x = new_gold_x;
        golds_arr[i].y = new_gold_y;
        golds_arr[i].actual_animation_idx = 0;
        golds_arr[i].map = golds_arr[i].map_animates[golds_arr[i].actual_animation_idx];
    }

    int new_clock_x;
    int new_clock_y; 
    bool collide = true;
    while(collide){
        collide = false;
        if(useSeed){
            new_clock_x = myRand() % (get_x_max() - clock_sp.width - GOLD_WIDTH) +  GOLD_WIDTH/2;
            new_clock_y = myRand() % (get_y_max() - clock_sp.height - GOLD_MIN_Y) + GOLD_MIN_Y;
        }
        else{
            new_clock_x = rand() % (get_x_max() - clock_sp.width - GOLD_WIDTH) +  GOLD_WIDTH/2;
            new_clock_y =  rand() % (get_y_max() - clock_sp.height - GOLD_MIN_Y) + GOLD_MIN_Y;
            
        }
        for(int n = 0; n < N_GOLDS; n++){
            int coll = check_collision(new_clock_x,new_clock_y,clock_sp.width,clock_sp.height,&golds_arr[n]);
            if (coll == 1 || coll == 2){
                collide = true;
                break;
            }
        }
    }
    clock_sp.x = new_clock_x;
    clock_sp.y = new_clock_y;

    cursor_sp.x = 500;
    cursor_sp.x = 500;
}

/**
 * state: 0 -> catch state  __  1-> drop state __ 2 -> DROP1 state  __  3-> DROP2 state2
 * type: 0-> indefined __  1-> gold  __  2-> clock
 * idx: indexed of the element in the specified vector
 **/
void handle_multiplayer_sprite_changes(int state,int type,int idx,int type2,int idx2,long long unsigned int* score,long long unsigned int* score2, int * clock_counter, int * clock_counter2){
    move_sprite(&car_sp,false);
    move_sprite(&car_sp2,false);

    animate_tick = (animate_tick + 1) % ANIMATION_SPEED; 
    if(car_sp.xspeed!=-2 && animate_tick == 0){
        change_sprite_animation(&car_sp);
    }
    if(car_sp2.xspeed!=-2 && animate_tick == 0){
        change_sprite_animation(&car_sp2);
    }

    //depend from car changes
    check_claw_changes();

    //depend from car2 changes
    check_claw2_changes();

    //catch state
    if(state == 0){
        int coll_idx = -1;
        int coll_type = 0;
        int coll_idx2 = -1;
        int coll_type2 = 0;
        for(int n = 0; n < N_GOLDS; n++){
            if (check_collision(claw_sp.x + 17,claw_sp.y+CLAW_SIZE,claw_sp.width-18-17,10,&golds_arr[n]) == 2 && *clock_counter > 0){ // claw collison_gold
                coll_idx = n;
                coll_type = 1;
            }
            if (check_collision(claw_sp2.x + 17,claw_sp2.y+CLAW_SIZE,claw_sp2.width-18-17,10,&golds_arr[n]) == 2 && *clock_counter2 > 0){ // claw2 collison_gold
                coll_idx2 = n;
                coll_type2 = 1;
            }
        }
        if(check_collision(claw_sp.x + 17,claw_sp.y+CLAW_SIZE,claw_sp.width-18-17,10,&clock_sp) == 2 && *clock_counter > 0){ // claw collision clock
            coll_idx = 0;
            coll_type = 2;
        }
        if(check_collision(claw_sp2.x + 17,claw_sp2.y+CLAW_SIZE,claw_sp2.width-18-17,10,&clock_sp) == 2 && *clock_counter2 > 0){ // claw collision clock
            coll_idx2 = 0;
            coll_type2 = 2;
        }

        if(coll_idx != -1){
            //changes claw to closed
            change_sprite_animation(&claw_sp);
            if(coll_type == 1){
                golds_arr[coll_idx].xspeed = claw_sp.xspeed;
                golds_arr[coll_idx].yspeed = claw_sp.yspeed;
            }
            else if(coll_type == 2){
                clock_sp.xspeed = claw_sp.xspeed;
                clock_sp.yspeed = claw_sp.yspeed;
            }
            set_multiplayer_drop_state(1,coll_type,coll_idx);
        }
        if(coll_idx2 != -1){
            //changes claw to closed
            change_sprite_animation(&claw_sp2);
            if(coll_type2 == 1){
                golds_arr[coll_idx2].xspeed = claw_sp2.xspeed;
                golds_arr[coll_idx2].yspeed = claw_sp2.yspeed;
            }
            else if(coll_type2 == 2){
                clock_sp.xspeed = claw_sp2.xspeed;
                clock_sp.yspeed = claw_sp2.yspeed;
            }
            set_multiplayer_drop_state(2,coll_type2,coll_idx2);
        }
    }
    //DROP state 
    else if(state == 1){
        if(type == 1){
            golds_arr[idx].xspeed = claw_sp.xspeed;
            golds_arr[idx].yspeed = claw_sp.yspeed;
        }
        else if(type == 2){
            clock_sp.xspeed = claw_sp.xspeed;
            clock_sp.yspeed = claw_sp.yspeed;
        }

        if(type2 == 1){
            golds_arr[idx2].xspeed = claw_sp2.xspeed;
            golds_arr[idx2].yspeed = claw_sp2.yspeed;
        }
        else if(type2 == 2){
            clock_sp.xspeed = claw_sp2.xspeed;
            clock_sp.yspeed = claw_sp2.yspeed;
        }

        //collect item and generate new pos
        if(claw_sp.y <= car_sp.y + car_sp.height && (claw_sp.x <= car_sp.x + car_sp.width || claw_sp.x + claw_sp.width <= car_sp.x)){
            if(type == 1){ // collect gold
                int new_gold_x;
                int new_gold_y; 
                bool collide = true;
                while(collide){
                    int coll = 0;
                    collide = false;
                    new_gold_x = myRand() % (get_x_max() - GOLD_WIDTH - GOLD_WIDTH);
                    new_gold_y =  myRand() % (get_y_max() - GOLD_HEIGHT - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        if(n != idx){
                            coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&golds_arr[n]);
                            if (coll == 1 || coll == 2){
                                collide = true;
                                //change_sprite_animation(&golds_arr[idx]);
                                break;
                            }
                        }
                    }
                    coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&clock_sp);
                    if (coll == 1 || coll == 2){
                        collide = true;
                        break;
                    }
                }
                golds_arr[idx].x = new_gold_x;
                golds_arr[idx].y = new_gold_y;
                golds_arr[idx].xspeed= -BACKGROUND_SPEED*2;
                golds_arr[idx].yspeed=0;
                *score = *score + SCORE_INCREMENT;
            }
            else if(type == 2){ // collect clock
                int new_clock_x;
                int new_clock_y; 
                bool collide = true;
                while(collide){
                    collide = false;
                    new_clock_x = myRand() % (get_x_max() - clock_sp.width - GOLD_WIDTH);
                    new_clock_y =  myRand() % (get_y_max() - clock_sp.height - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        int coll = check_collision(new_clock_x,new_clock_y,clock_sp.width,clock_sp.height,&golds_arr[n]);
                        if (coll == 1 || coll == 2){
                            collide = true;
                            break;
                        }
                    }
                }
                clock_sp.x = new_clock_x;
                clock_sp.y = new_clock_y;
                clock_sp.xspeed=-BACKGROUND_SPEED*2;
                clock_sp.yspeed=0;
                *clock_counter += CLOCKINCREMENT;
            }
            //opens claw
            change_sprite_animation(&claw_sp);
            set_multiplayer_catch_state(1);
        }

        //collect item and generate new pos 2
        if(claw_sp2.y <= car_sp2.y + car_sp2.height && (claw_sp2.x <= car_sp2.x + car_sp2.width || claw_sp2.x + claw_sp2.width <= car_sp2.x)){
            if(type2 == 1){ // collect gold
                int new_gold_x;
                int new_gold_y; 
                bool collide = true;
                while(collide){
                    int coll = 0;
                    collide = false;
                    new_gold_x = myRand() % (get_x_max() - GOLD_WIDTH - GOLD_WIDTH);
                    new_gold_y =  myRand() % (get_y_max() - GOLD_HEIGHT - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        if(n != idx2){
                            coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&golds_arr[n]);
                            if (coll == 1 || coll == 2){
                                collide = true;
                                //change_sprite_animation(&golds_arr[idx]);
                                break;
                            }
                        }
                    }
                    coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&clock_sp);
                    if (coll == 1 || coll == 2){
                        collide = true;
                        break;
                    }
                }
                golds_arr[idx2].x = new_gold_x;
                golds_arr[idx2].y = new_gold_y;
                golds_arr[idx2].xspeed= -BACKGROUND_SPEED*2;
                golds_arr[idx2].yspeed=0;
                *score2 = *score2 + SCORE_INCREMENT;
            }
            else if(type2 == 2){ // collect clock
                int new_clock_x;
                int new_clock_y; 
                bool collide = true;
                while(collide){
                    collide = false;
                    new_clock_x = myRand() % (get_x_max() - clock_sp.width - GOLD_WIDTH);
                    new_clock_y =  myRand() % (get_y_max() - clock_sp.height - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        int coll = check_collision(new_clock_x,new_clock_y,clock_sp.width,clock_sp.height,&golds_arr[n]);
                        if (coll == 1 || coll == 2){
                            collide = true;
                            break;
                        }
                    }
                }
                clock_sp.x = new_clock_x;
                clock_sp.y = new_clock_y;
                clock_sp.xspeed=-BACKGROUND_SPEED*2;
                clock_sp.yspeed=0;
                *clock_counter2 += CLOCKINCREMENT;
            }
            //opens claw
            change_sprite_animation(&claw_sp2);
            set_multiplayer_catch_state(2);
        }
    }
    //DROP1 state
    else if(state == 2){
        if(type == 1){
            golds_arr[idx].xspeed = claw_sp.xspeed;
            golds_arr[idx].yspeed = claw_sp.yspeed;
            //changes to gold
        }
        else if(type == 2){
            clock_sp.xspeed = claw_sp.xspeed;
            clock_sp.yspeed = claw_sp.yspeed;
        }
        //collect item and generate new pos
        if(claw_sp.y <= car_sp.y + car_sp.height && (claw_sp.x <= car_sp.x + car_sp.width || claw_sp.x + claw_sp.width <= car_sp.x)){
            if(type == 1){ // collect gold
                int new_gold_x;
                int new_gold_y; 
                bool collide = true;
                while(collide){
                    int coll = 0;
                    collide = false;
                    new_gold_x = myRand() % (get_x_max() - GOLD_WIDTH - GOLD_WIDTH);
                    new_gold_y =  myRand() % (get_y_max() - GOLD_HEIGHT - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        if(n != idx){
                            coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&golds_arr[n]);
                            if (coll == 1 || coll == 2){
                                collide = true;
                                //change_sprite_animation(&golds_arr[idx]);
                                break;
                            }
                        }
                    }
                    coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&clock_sp);
                    if (coll == 1 || coll == 2){
                        collide = true;
                        break;
                    }
                }
                golds_arr[idx].x = new_gold_x;
                golds_arr[idx].y = new_gold_y;
                golds_arr[idx].xspeed= -BACKGROUND_SPEED*2;
                golds_arr[idx].yspeed=0;
                *score = *score + SCORE_INCREMENT;
            }
            else if(type == 2){ // collect clock
                int new_clock_x;
                int new_clock_y; 
                bool collide = true;
                while(collide){
                    collide = false;
                    new_clock_x = myRand() % (get_x_max() - clock_sp.width - GOLD_WIDTH);
                    new_clock_y =  myRand() % (get_y_max() - clock_sp.height - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        int coll = check_collision(new_clock_x,new_clock_y,clock_sp.width,clock_sp.height,&golds_arr[n]);
                        if (coll == 1 || coll == 2){
                            collide = true;
                            break;
                        }
                    }
                }
                clock_sp.x = new_clock_x;
                clock_sp.y = new_clock_y;
                clock_sp.xspeed=-BACKGROUND_SPEED*2;
                clock_sp.yspeed=0;
                *clock_counter += CLOCKINCREMENT;
            }
            //opens claw
            change_sprite_animation(&claw_sp);
            set_multiplayer_catch_state(1);
        }

        //check catch 2
        int coll_idx2 = -1;
        int coll_type2 = 0;
        for(int n = 0; n < N_GOLDS; n++){
            if (check_collision(claw_sp2.x + 17,claw_sp2.y+CLAW_SIZE,claw_sp2.width-18-17,10,&golds_arr[n]) == 2 
                && ((n != idx && type == 1) || type != 1)  && *clock_counter2 > 0){ // claw2 collison_gold
                coll_idx2 = n;
                coll_type2 = 1;
            }

        }
        if(check_collision(claw_sp2.x + 17,claw_sp2.y+CLAW_SIZE,claw_sp2.width-18-17,10,&clock_sp) == 2 && type != 2 && *clock_counter2 > 0){ // claw collision clock
            coll_idx2 = 0;
            coll_type2 = 2;
        }

        if(coll_idx2 != -1){
            //changes claw to closed
            change_sprite_animation(&claw_sp2);
            if(coll_type2 == 1){
                golds_arr[coll_idx2].xspeed = claw_sp2.xspeed;
                golds_arr[coll_idx2].yspeed = claw_sp2.yspeed;
            }
            else if(coll_type2 == 2){
                clock_sp.xspeed = claw_sp2.xspeed;
                clock_sp.yspeed = claw_sp2.yspeed;
            }
            set_multiplayer_drop_state(2,coll_type2,coll_idx2);
        }

    }
    //DROP2 state
    else if(state == 3){

        int coll_idx = -1;
        int coll_type = 0;
        for(int n = 0; n < N_GOLDS; n++){
            if (check_collision(claw_sp.x + 17,claw_sp.y+CLAW_SIZE,claw_sp.width-18-17,10,&golds_arr[n]) == 2 
                && ((n != idx2 && type2 == 1) || type2 != 1) && *clock_counter > 0){ // claw collison_gold
                coll_idx = n;
                coll_type = 1;
            }
        }
        if(check_collision(claw_sp.x + 17,claw_sp.y+CLAW_SIZE,claw_sp.width-18-17,10,&clock_sp) == 2 && type2 != 2 && *clock_counter > 0){ // claw collision clock
            coll_idx = 0;
            coll_type = 2;
        }

        if(coll_idx != -1){
            //changes claw to closed
            change_sprite_animation(&claw_sp);
            if(coll_type == 1){
                golds_arr[coll_idx].xspeed = claw_sp.xspeed;
                golds_arr[coll_idx].yspeed = claw_sp.yspeed;
            }
            else if(coll_type == 2){
                clock_sp.xspeed = claw_sp.xspeed;
                clock_sp.yspeed = claw_sp.yspeed;
            }
            set_multiplayer_drop_state(1,coll_type,coll_idx);
        }


        if(type2 == 1){
            golds_arr[idx2].xspeed = claw_sp2.xspeed;
            golds_arr[idx2].yspeed = claw_sp2.yspeed;
            //changes to gold
        }
        else if(type2 == 2){
            clock_sp.xspeed = claw_sp2.xspeed;
            clock_sp.yspeed = claw_sp2.yspeed;
        }

        //collect item and generate new pos 2
        if(claw_sp2.y <= car_sp2.y + car_sp2.height && (claw_sp2.x <= car_sp2.x + car_sp2.width || claw_sp2.x + claw_sp2.width <= car_sp2.x)){
            if(type2 == 1){ // collect gold
                int new_gold_x;
                int new_gold_y; 
                bool collide = true;
                while(collide){
                    int coll = 0;
                    collide = false;
                    new_gold_x = myRand() % (get_x_max() - GOLD_WIDTH - GOLD_WIDTH);
                    new_gold_y =  myRand() % (get_y_max() - GOLD_HEIGHT - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        if(n != idx2){
                            coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&golds_arr[n]);
                            if (coll == 1 || coll == 2){
                                collide = true;
                                //change_sprite_animation(&golds_arr[idx]);
                                break;
                            }
                        }
                    }
                    coll = check_collision(new_gold_x,new_gold_y,GOLD_WIDTH,GOLD_HEIGHT,&clock_sp);
                    if (coll == 1 || coll == 2){
                        collide = true;
                        break;
                    }
                }
                golds_arr[idx2].x = new_gold_x;
                golds_arr[idx2].y = new_gold_y;
                golds_arr[idx2].xspeed= -BACKGROUND_SPEED*2;
                golds_arr[idx2].yspeed=0;
                *score2 = *score2 + SCORE_INCREMENT;
            }
            else if(type2 == 2){ // collect clock
                int new_clock_x;
                int new_clock_y; 
                bool collide = true;
                while(collide){
                    collide = false;
                    new_clock_x = myRand() % (get_x_max() - clock_sp.width - GOLD_WIDTH);
                    new_clock_y =  myRand() % (get_y_max() - clock_sp.height - GOLD_MIN_Y) + GOLD_MIN_Y;
                    for(int n = 0; n < N_GOLDS; n++){
                        int coll = check_collision(new_clock_x,new_clock_y,clock_sp.width,clock_sp.height,&golds_arr[n]);
                        if (coll == 1 || coll == 2){
                            collide = true;
                            break;
                        }
                    }
                }
                clock_sp.x = new_clock_x;
                clock_sp.y = new_clock_y;
                clock_sp.xspeed=-BACKGROUND_SPEED*2;
                clock_sp.yspeed=0;
                *clock_counter2 += CLOCKINCREMENT;
            }
            //opens claw
            change_sprite_animation(&claw_sp2);
            set_multiplayer_catch_state(2);
        }
    }
    
    for(int n = 0; n < N_GOLDS; n++){
        move_sprite(&golds_arr[n],true);
        while(golds_arr[n].actual_animation_idx != 1){
             change_sprite_animation(&golds_arr[n]);
        }
    }
    move_sprite(&clock_sp,true);
    move_sprite(&claw_sp,false);
    move_sprite(&claw_sp2,false);
}

void draw_multiplayer_game_screen(int clock_counter, long long unsigned int score,int clock_counter2, long long unsigned int score2){
    copy_background_buffer();

    draw_sprite(&car_sp);
    draw_sprite(&car_sp2);

    for(int i = 0; i < N_GOLDS; i++){
        draw_sprite(&(golds_arr[i]));
    }

    draw_sprite(&clock_sp);
    if(clock_counter > 0){
        draw_sprite(&claw_sp);
        for(int i = claw_sp.y - rope_sp.height + 1; i >= (car_sp.height)/2 + (car_sp.height)/4 + car_sp.y - 5; i-= rope_sp.height){
        rope_sp.x = car_sp.x + (car_sp.width/2) - rope_sp.width/2;
        rope_sp.y = i;
        draw_sprite(&rope_sp);
        }
    }
    if(clock_counter2 > 0){
        draw_sprite(&claw_sp2);
        for(int i = claw_sp2.y - rope_sp.height + 1; i >= (car_sp2.height)/2 + (car_sp2.height)/4 + car_sp2.y - 5; i-= rope_sp.height){
        rope_sp.x = car_sp2.x + (car_sp2.width/2) - rope_sp.width/2;
        rope_sp.y = i;
        draw_sprite(&rope_sp);
        }
    }
    

    // WHEN CLOCK_COUNTER REACHES 0 PROGRAM DONT BREAK
    if(clock_counter <= 0){
        clock_counter = 0;
    }
    if(clock_counter2 <= 0){
        clock_counter2 = 0;
    }

    draw_menu_sprite(SCORE_POS_X,SCORE_POS_Y,&player1_sp);

    // CLOCK1 DRAW
    draw_menu_sprite(SCORE_POS_X + 2*NEXT_POS,4*CLOCK_POS_Y + 2*zero_sp.height,get_sprite_number(clock_counter % 10));
    draw_menu_sprite(SCORE_POS_X + NEXT_POS,4*CLOCK_POS_Y + 2*zero_sp.height,get_sprite_number((clock_counter/10) % 10));
    draw_menu_sprite(SCORE_POS_X,4*CLOCK_POS_Y + 2*zero_sp.height,get_sprite_number((clock_counter/100) % 10));

    // SCORE1 DRAW
    draw_menu_sprite(SCORE_POS_X + 5*NEXT_POS,3*SCORE_POS_Y + zero_sp.height,get_sprite_number(score % 10));
    draw_menu_sprite(SCORE_POS_X + 4*NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score/10) % 10));
    draw_menu_sprite(SCORE_POS_X + 3*NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score/100) % 10));
    draw_menu_sprite(SCORE_POS_X + 2*NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score/1000) % 10));
    draw_menu_sprite(SCORE_POS_X + NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score/10000) % 10));
    draw_menu_sprite(SCORE_POS_X ,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score/100000) % 10));



    draw_menu_sprite(CLOCK_POS_X - player2_sp.width + NEXT_POS/2 , SCORE_POS_Y,&player2_sp);

    // CLOCK2 DRAW
    draw_menu_sprite(CLOCK_POS_X,4*CLOCK_POS_Y + 2*zero_sp.height,get_sprite_number(clock_counter2 % 10));
    draw_menu_sprite(CLOCK_POS_X - NEXT_POS,4*CLOCK_POS_Y + 2*zero_sp.height,get_sprite_number((clock_counter2/10) % 10));
    draw_menu_sprite(CLOCK_POS_X - 2*NEXT_POS,4*CLOCK_POS_Y + 2*zero_sp.height,get_sprite_number((clock_counter2/100) % 10));

    // SCORE2 DRAW
    draw_menu_sprite(CLOCK_POS_X ,3*SCORE_POS_Y + zero_sp.height,get_sprite_number(score2 % 10));
    draw_menu_sprite(CLOCK_POS_X - NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score2/10) % 10));
    draw_menu_sprite(CLOCK_POS_X - 2*NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score2/100) % 10));
    draw_menu_sprite(CLOCK_POS_X - 3*NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score2/1000) % 10));
    draw_menu_sprite(CLOCK_POS_X - 4*NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score2/10000) % 10));
    draw_menu_sprite(CLOCK_POS_X - 5*NEXT_POS,3*SCORE_POS_Y+ zero_sp.height,get_sprite_number((score2/100000) % 10));


    copy_buffer();
}

void draw_loading_screen(){
    copy_background_buffer();
    move_sprite(&car_sp,false);
    move_sprite(&car_sp2,false);

    // animate sprites
    animate_tick = (animate_tick + 1) % ANIMATION_SPEED; 
    if(car_sp.xspeed!=-2 && animate_tick == 0){
        change_sprite_animation(&car_sp);
    }
    if(car_sp2.xspeed!=-2 && animate_tick == 0){
        change_sprite_animation(&car_sp2);
    }

    //keep moving cars while loading
    if(car_sp.x == 0){
        set_car_speed(1);
    }
    else if(car_sp.x == get_x_max() - car_sp.width){
        set_car_speed(-1);
    }
    else if(car_sp.xspeed == 0){
        set_car_speed(1);
    }

    if(car_sp2.x == 0){
        set_car2_speed(1);
    }
    else if(car_sp2.x == get_x_max() - car_sp.width){
        set_car2_speed(-1);
    }
    else if(car_sp2.xspeed == 0){
        set_car2_speed(1);
    }

    draw_sprite(&car_sp);
    draw_sprite(&car_sp2);

    draw_menu_sprite(0,0,&loading_sp);

    copy_buffer();
}
