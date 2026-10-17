#include <lcom/lcf.h>
#include <math.h>
#include "game.h"
#include "video.h"
#include "mainmenu.h"
#include "i8042.h"
#include "highscores.h"

extern Sprite *background;
extern Sprite *cursor;
extern Sprite *man;
extern Sprite *vida_;
extern Sprite *play;
extern Sprite *singleplayer_but;
extern Sprite *instructions_but;
extern Sprite *back_but;
extern Sprite *pause_but;
extern Sprite *continue_but;
extern Sprite *exit_but;
extern Sprite *exit_mini_but;
extern Sprite *menu_mini_but;
extern Sprite *loading_;
Food* fruits_active[30];
extern xpm_image_t fruits_images[10];
extern xpm_image_t player[3];
extern xpm_image_t loading[12];
extern xpm_image_t cursors[2];
extern xpm_image_t backgrounds[9];
extern xpm_image_t buttons[17];
extern xpm_image_t vidas;

extern int timer_interrupt_counter;
extern struct packet pp;
extern uint8_t scancode;
extern bool esq;

int next_cursor_x = 0;
int next_cursor_y = 0;
int next_man_x = 0;
int next_man_y = 0;
int speed = 1;
int generator = 180;
int click_move = 35;
int lives = 3;
int points = 0;
uint16_t count = 0;
bool pause_ = false;
bool ontop = false;
bool ontop1 = false;
bool ontop2 = false;

bool check_click(Sprite* button){
    if(cursor->xi >= button->xi && cursor->xi <= button->xf && cursor->yi >= button->yi && cursor->yi <= button->yf)
        return true;
    else return false;
}

void update_cursor() {
    erase_sprite(cursor,background);
    cursor->xi +=next_cursor_x;
    cursor->yi += next_cursor_y;
    if (cursor->xi < 0) cursor->xi = 0;
    if (cursor->yi < 0) cursor->yi = 0;
    if (cursor->xi> 1152 - cursor->width) cursor->xi = 1152 - cursor->width;
    if (cursor->yi > 864 - cursor->height) cursor->yi = 864 - cursor->height;
    next_cursor_x = 0;
    next_cursor_y = 0;
}

void show_man() {
    draw_sprite(man);
}

void show_background() {
    draw_sprite(background);
    draw_sprite(pause_but);
}

void changecursor(uint8_t num) {
    cursor->map = (uint32_t*)cursors[num].bytes;
    cursor->width = cursors[num].width;
    cursor->height = cursors[num].height;
}

void changebutton(Sprite* but, int num) {
    but->map = (uint32_t*)buttons[num].bytes;
    but->width = buttons[num].width;
    but->height = buttons[num].height;
}

void show_lives(){
    for(int i = 1; i <= lives;i++){
        vida_->xi = 1152- (i*(vida_->width + 5));
        draw_sprite(vida_);
    }
}

Food *choose_food(int fruit_number, int x, int speed){
    bool healthy = false;
    if(fruit_number == 0 || fruit_number == 1 || fruit_number == 2 || fruit_number == 4 || fruit_number == 7) healthy = true;
    Food *f = create_food(&fruits_images[fruit_number],x,-100,0,speed,healthy);
    return f;
}

void draw_fruit(){
    for(int i = 0;i < 30;i++){
        if(fruits_active[i] != NULL)
            draw_sprite(fruits_active[i]->sprite);
    }
}

void changeFruitsPos(){
    for( int i = 0; i < 30;i++){
        if(fruits_active[i] != NULL){
            fruits_active[i]->sprite->yi +=fruits_active[i]->sprite->yspeed;
            if(fruits_active[i]->sprite->yi+fruits_active[i]->sprite->height > man->yi + man->height)
            {
                if(!fruits_active[i]->healthy){
                    lives--;
                }
                destroy_food(fruits_active[i]);
                fruits_active[i] = NULL;
            }
        }
         
    }
}

void clear_fruits(){
    for(int i = 0; i < 30; i++){
        destroy_food(fruits_active[i]);
        fruits_active[i] = NULL;
    }
}

void check_colisions(){
    for (int i = 0; i < 30;i++){
        if (fruits_active[i] != NULL){
            if(((fruits_active[i]->sprite->yi+fruits_active[i]->sprite->height >= man->yi + 10 && fruits_active[i]->sprite->yi+fruits_active[i]->sprite->height <= man->yi + 102) && (fruits_active[i]->sprite->xi + fruits_active[i]->sprite->width >= man->xi + 70 && fruits_active[i]->sprite->xi <= man->xi + man->width - 70)) || ((fruits_active[i]->sprite->yi+fruits_active[i]->sprite->height >= man->yi + 110 && fruits_active[i]->sprite->yi <= man->yi + man->height - 111) && (fruits_active[i]->sprite->xi+fruits_active[i]->sprite->width >=man->xi && fruits_active[i]->sprite->xi<=man->xi + man->width))){
                if(!fruits_active[i]->healthy){
                    lives--;
                }
                else
                    points += 20;
                destroy_food(fruits_active[i]);
                fruits_active[i] = NULL;
            }
        }
    }
}

void check_destruction(){
    for(int i = 0; i< 30;i++){
        bool destroy = false;
        if(fruits_active[i] != NULL){
            if(cursor->xi >= fruits_active[i]->sprite->xi && cursor->xi <= fruits_active[i]->sprite->xi+fruits_active[i]->sprite->width && cursor->yi >= fruits_active[i]->sprite->yi && cursor->yi <= fruits_active[i]->sprite->yi+fruits_active[i]->sprite->height) destroy = true;
            else if(cursor->xi >= fruits_active[i]->sprite->xi && cursor->xi <= fruits_active[i]->sprite->xi+fruits_active[i]->sprite->width && cursor->yi+cursor->height >= fruits_active[i]->sprite->yi && cursor->yi + cursor->height <= fruits_active[i]->sprite->yi+fruits_active[i]->sprite->height) destroy = true;
            else if(cursor->xi + cursor->width >= fruits_active[i]->sprite->xi && cursor->xi + cursor->width <= fruits_active[i]->sprite->xi+fruits_active[i]->sprite->width && cursor->yi >= fruits_active[i]->sprite->yi && cursor->yi <= fruits_active[i]->sprite->yi+fruits_active[i]->sprite->height) destroy = true;
            else if(cursor->xi + cursor->width >= fruits_active[i]->sprite->xi && cursor->xi + cursor->width <= fruits_active[i]->sprite->xi+fruits_active[i]->sprite->width && cursor->yi + cursor->height >= fruits_active[i]->sprite->yi && cursor->yi + cursor->height <= fruits_active[i]->sprite->yi+fruits_active[i]->sprite->height) destroy = true;
            if(destroy == true){
                if(!fruits_active[i]->healthy) points += 5;
                destroy_food(fruits_active[i]);
                fruits_active[i] = NULL;
            }
        }
    }
}

void exit_game(int numBackgound) {                     
    clear_sec_buff();
    background->map = (uint32_t*)backgrounds[numBackgound].bytes;
    draw_sprite(background);
    draw_sprite(loading_);
    refresh();    
}

void handler(enum location* loc, enum type_of_interrupt interrupt){
    switch (*loc){
        case ENTRANCE:
            switch (interrupt){
                case TIMER:
                        update_cursor();
                        show_cursor();
                        refresh();
                    break;
                case KEYBOARD:
                    if (scancode == ESQ) {
                        exit_game(4);
                        *loc = EXIT;
                    }
                    break;
                case MOUSE:
                    next_cursor_x += pp.delta_x;
                    next_cursor_y -= pp.delta_y;
                    if(pp.lb){
                        if (check_click(play)) {
                            clear_sec_buff();
                            show_menu();
                            show_cursor();
                            refresh();
                            *loc = MENU;
                        }
                        memset(pp.bytes,0,3*sizeof(pp.bytes[0]));
                    }
                    break;
                default:
                    break;
            }   
            break;        
        case MENU:
            switch (interrupt){
                case TIMER:
                        update_cursor();
                        show_cursor();
                        refresh();
                    break;
                case KEYBOARD:
                    if (scancode == ESQ) {
                        exit_game(4);
                        *loc = EXIT;
                    }
                    break;                    
                case MOUSE:
                    next_cursor_x += pp.delta_x;
                    next_cursor_y -= pp.delta_y;                
                    if(check_click(instructions_but)) {
                        changebutton(instructions_but, 14);
                        ontop = true;
                        draw_sprite(instructions_but);
                    }
                    else if (!check_click(instructions_but) && ontop == true) {
                        changebutton(instructions_but, 3);
                        ontop = false;
                        draw_sprite(instructions_but);
                    }
                    if(check_click(exit_but)) {
                        changebutton(exit_but, 12);
                        ontop1 = true;
                        draw_sprite(exit_but);
                    }
                    else if (!check_click(exit_but) && ontop1 == true) {
                        changebutton(exit_but, 4);
                        ontop1 = false;
                        draw_sprite(exit_but);
                    }
                    if(check_click(singleplayer_but)) {
                        changebutton(singleplayer_but, 13);
                        ontop2 = true;
                        draw_sprite(singleplayer_but);
                    }
                    else if (!check_click(singleplayer_but) && ontop2 == true) {
                        changebutton(singleplayer_but, 1);
                        ontop2 = false;
                        draw_sprite(singleplayer_but);
                    }          
                    if (pp.lb) {
                        if (check_click(singleplayer_but)) {
                            changebutton(singleplayer_but, 1);
                            clear_sec_buff();
                            background->map = (uint32_t*)backgrounds[3].bytes;
                            changecursor(1);
                            show_background(); 
                            show_man();
                            show_cursor();
                            refresh();
                            *loc = SINGLEPLAYER;
                        }
                        else if (check_click(instructions_but)) {
                            changebutton(instructions_but, 3);
                            clear_sec_buff();
                            show_instructions_menu(2);
                            show_cursor();
                            refresh();
                            *loc = INSTRUCTIONS;
                        }   
                        else if (check_click(exit_but)) { 
                            exit_game(4);
                            *loc = EXIT;
                        } 
                        memset(pp.bytes,0,3*sizeof(pp.bytes[0]));
                    }
                    break;
                default:
                    break; 
        
            }
            break;
        case SINGLEPLAYER:
            switch(interrupt){
                case TIMER:
                    update_cursor();
                    changeFruitsPos();
                    if(timer_interrupt_counter % generator == 0 /*&& speed != 5*/){
                        Food* food = choose_food(rand()%10,rand()%1092,speed);
                        for(int i = 0; i < 30;i++){
                            if (fruits_active[i] == NULL) {
                                fruits_active[i] = food;
                                break;
                            }
                        }
                    }
                    man->xi +=next_man_x;
                    if (man->xi < 0) man->xi = 0;
                    if (man->xi+man->width > 1152) man->xi = 1152-man->width;
                    next_man_x = 0;
                    check_colisions();
                    if(lives == 0){
                        changecursor(0);
                        clear_sec_buff();
                        show_game_over_menu();
                        deal_with_points();
                        show_score();
                        refresh();
                        *loc = GAMEOVER;
                        break;
                    }
                    show_background();
                    draw_fruit();
                    show_man();
                    show_lives();
                    show_points();
                    draw_sprite(pause_but);
                    show_cursor();
                    refresh();
                    break;
                case KEYBOARD:
                    if (scancode == ESQ) {
                        exit_game(6);
                        *loc = EXIT;
                        break;}
                    if (scancode == RIGHT_ARROW_MAKE || scancode == D_MAKE){
                        count++;
                        if (count%2 == 1) man->map = (uint32_t*)player[2].bytes;
                        else man->map = (uint32_t*)player[0].bytes;                        
                        next_man_x += click_move;     
                    }
                    if (scancode == RIGHT_ARROW_BREAK || scancode == D_BREAK) {
                        count = 0;
                        man->map = (uint32_t*)player[0].bytes;  
                    }
                    if (scancode == LEFT_ARROW_MAKE || scancode == A_MAKE){
                        count++;
                        if (count%2 == 1) man->map = (uint32_t*)player[1].bytes;
                        else man->map = (uint32_t*)player[0].bytes;
                        next_man_x-= click_move;
                    }
                    if (scancode == LEFT_ARROW_BREAK || scancode == A_BREAK) {
                        count = 0;
                        man->map = (uint32_t*)player[0].bytes;  
                    }
                    break;
                case MOUSE:
                    next_cursor_x += pp.delta_x;
                    next_cursor_y -= pp.delta_y;
                    if(cursor->xi + cursor->width >= pause_but->xi && cursor->xi + cursor->width <= pause_but->xf && cursor->yi + cursor->height >= pause_but->yi && cursor->yi + cursor->height <= pause_but->yf) {
                        changecursor(0);
                    }
                    else if (!check_click(pause_but)){
                        changecursor(1);
                    }
                    if(check_click(pause_but)) {
                        changebutton(pause_but, 16);
                        ontop = true;
                        draw_sprite(pause_but);
                    }
                    else if (!check_click(pause_but) && ontop == true) {
                        changebutton(pause_but, 6);
                        ontop = false;
                        draw_sprite(pause_but);
                    }
                    if (pp.lb) {
                        if (check_click(pause_but)) {
                            changebutton(pause_but, 6);
                            changecursor(0);
                            clear_sec_buff();
                            show_pause_menu();
                            refresh();
                            *loc = PAUSE;
                        }
                        else{
                            update_cursor();
                            check_destruction();
                            show_background();
                            draw_fruit();
                            show_man();
                            show_lives();
                            show_points();
                            draw_sprite(pause_but);
                            show_cursor();
                            refresh();
                        }
                    }
                
                    break;
                case RTC:
                    if (speed < 5){
                        speed++;
                    }
                    if (generator > 20)  {
                        generator -=10;
                        click_move+=1;
                    }
                    break;
                default:
                    break;
            }
            break;
        case PAUSE:
            switch(interrupt){
                case TIMER:
                    update_cursor();
                    show_cursor();
                    refresh();
                    break;
                case KEYBOARD:
                    if (scancode == ESQ) {
                        exit_game(6);
                        *loc = EXIT;
                    }
                    break;
                case MOUSE:
                    if(check_click(instructions_but)) {
                        changebutton(instructions_but, 14);
                        draw_sprite(instructions_but);
                        ontop = true;   
                    }
                    else if (!check_click(instructions_but) && ontop == true) {
                        changebutton(instructions_but, 3);
                        draw_sprite(instructions_but);
                        ontop = false;   
                    }
                    if(check_click(exit_but)) {
                        changebutton(exit_but, 12);
                        draw_sprite(exit_but);
                        ontop1 = true;           
                    }
                    else if (!check_click(exit_but) && ontop1 == true) {
                        changebutton(exit_but, 4);
                        draw_sprite(exit_but);
                        ontop1 = false;   
                    }
                    if(check_click(continue_but)) {
                        changebutton(continue_but, 15);
                        draw_sprite(continue_but);
                        ontop2 = true;   
                    }
                    else if (!check_click(continue_but) && ontop2 == true) {
                        changebutton(continue_but, 7);
                        draw_sprite(continue_but);
                        ontop2 = false;   
                    }                                  
                    next_cursor_x += pp.delta_x;
                    next_cursor_y -= pp.delta_y;
                    if (pp.lb) {
                        if (check_click(continue_but)) {
                            changebutton(continue_but, 7);
                            clear_sec_buff();
                            background->map = (uint32_t*)backgrounds[3].bytes;
                            changecursor(1);
                            show_background();  
                            show_man();
                            show_cursor();
                            refresh();
                            *loc = SINGLEPLAYER;
                        }
                        else if (check_click(instructions_but)) {
                            changebutton(instructions_but, 3);
                            pause_ = true;
                            clear_sec_buff();
                            show_cursor();
                            show_instructions_menu(7);
                            refresh();
                            *loc = INSTRUCTIONS;
                        }   
                        else if (check_click(exit_but)) { 
                            exit_game(6);
                            *loc = EXIT;
                        } 
                    }
                    break;
                default:
                    break;
            }
            break;     
        case GAMEOVER:
            switch(interrupt){
                case TIMER:
                        show_game_over_menu();
                        show_score();
                        update_cursor();
                        show_cursor();
                        refresh();
                    break;
                case KEYBOARD:
                    if (scancode == ESQ) {
                        exit_game(6);
                        *loc = EXIT;
                    }
                    break;                    
                case MOUSE:
                    next_cursor_x += pp.delta_x;
                    next_cursor_y -= pp.delta_y;                
                    if(check_click(exit_mini_but)) {
                        changebutton(exit_mini_but, 10);
                        ontop = true; 
                        draw_sprite(exit_mini_but);
                    }
                    else if (!check_click(exit_mini_but) && ontop == true) {
                        changebutton(exit_mini_but, 8);
                        draw_sprite(exit_mini_but);
                        ontop = false;
                    }
                    if(check_click(menu_mini_but)) {
                        changebutton(menu_mini_but, 11);
                        ontop1 = true;   
                        draw_sprite(menu_mini_but);

                    }
                    else if (!check_click(menu_mini_but) && ontop1 == true) {
                        changebutton(menu_mini_but, 9);
                        draw_sprite(menu_mini_but);
                        ontop1 = false;
                    }
                    if (pp.lb) {
                        if (check_click(exit_mini_but)){
                            exit_game(6);
                            *loc = EXIT;
                        }
                        if (check_click(menu_mini_but)){
                            changebutton(menu_mini_but, 9);
                            generator = 180;
                            click_move = 35;
                            lives = 3;
                            points = 0;
                            speed = 1;
                            clear_fruits();
                            background->map = (uint32_t*)backgrounds[0].bytes;
                            clear_sec_buff();
                            show_menu();
                            show_cursor();
                            refresh();
                            *loc = MENU;
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        case INSTRUCTIONS:
            switch(interrupt){
                case TIMER:
                        update_cursor();
                        show_cursor();
                        refresh();
                    break;
                case KEYBOARD:
                    if (scancode == ESQ) {
                        if(pause_ == false) exit_game(4);
                        else if (pause_ == true) exit_game(6);
                        *loc = EXIT;
                    }
                    break; 
                case MOUSE:
                    if(check_click(back_but)) {
                        changebutton(back_but, 2);
                        ontop = true;
                        draw_sprite(back_but);
                    }
                    else if (!check_click(back_but) && ontop == true) {
                        changebutton(back_but, 5);
                        draw_sprite(back_but);
                        ontop = false;
                    }
                    next_cursor_x += pp.delta_x;
                    next_cursor_y -= pp.delta_y;
                    if (pp.lb) {
                        if (check_click(back_but)) {
                            changebutton(back_but, 5);
                            clear_sec_buff();
                            if (pause_ == true){
                                show_pause_menu();
                                *loc = PAUSE;
                            }
                            else {
                                show_menu();
                                *loc = MENU;

                            }
                            show_cursor();
                            refresh();
                        }    
                        memset(pp.bytes,0,3*sizeof(pp.bytes[0]));
                    }
                    break;
                default:
                    break;
            }
            break;
        case EXIT:
            switch(interrupt){
                case TIMER:
                    if (count == 24) {
                        esq = true;
                    }  
                    if(timer_interrupt_counter % 10 == 0){
                        loading_->map = (uint32_t*)loading[count%12].bytes;
                        draw_sprite(loading_);
                        refresh(); 
                        count++;
                    }                     
                    break;
                default:
                    break;
            }
            break;
    }
}
