#include "view.h"
extern Time_Irl timeIrl;
extern MenuState menuState ;
extern ProgramState programState;
extern GameState gameState;
extern vbe_mode_info_t modeInfo;
extern int mouse_x;
extern int mouse_y;
extern uint8_t rows ;
extern uint8_t cols ;
extern int8_t entry;
extern int8_t pause_entry;
extern int8_t game_over_entry;
extern int8_t win_entry ;
extern uint8_t timer;
extern uint8_t seconds ;
extern uint8_t minutes ;
extern uint8_t hours ;
extern Sprite *start_button_sprite;
extern Sprite *start_button2_sprite;
extern Sprite *cursor_mouse;
extern Sprite *arrows_sprite;
extern Sprite *controls_button_sprite;
extern Sprite *controls_button2_sprite;
extern Sprite *exit_button_sprite;
extern Sprite *title_sprite;
extern Sprite *control_title_sprite;
extern Sprite *paused_title_sprite;
extern Sprite *exit_button2_sprite;
extern Sprite *dirt_block;
extern Sprite *day_sky_sprite;
extern Sprite *night_sky_sprite;
extern Sprite *afternoon_sky_sprite;
extern Sprite *dyog_sprite;
extern Sprite *dyog2_sprite;
extern Sprite *continue_button_sprite;
extern Sprite *continue_button2_sprite;
extern Sprite *right_click_sprite;
extern Sprite *left_click_sprite;
extern Sprite *esc_sprite;
extern Sprite *f_sprite;
extern Sprite *space_sprite;
extern Sprite *grass_sprite;
extern Sprite *selected_grass_sprite;
extern Sprite *tnt_sprite;
extern Sprite *cobblestone_sprite;
extern Sprite *selected_cobblestone_sprite;
extern Sprite *tile1_sprite;
extern Sprite *selected_tile1_sprite;
extern Sprite *tile2_sprite;
extern Sprite *selected_tile2_sprite;
extern Sprite *tile3_sprite;
extern Sprite *selected_tile3_sprite;
extern Sprite *tile4_sprite;
extern Sprite *selected_tile4_sprite;
extern Sprite *tile5_sprite;
extern Sprite *selected_tile5_sprite;
extern Sprite *tile6_sprite;
extern Sprite *selected_tile6_sprite;
extern Sprite *tile7_sprite;
extern Sprite *selected_tile7_sprite;
extern Sprite *tile8_sprite;
extern Sprite *selected_tile8_sprite;
extern Sprite *flag_sprite;
extern Sprite *selected_flag_sprite;
extern Sprite *game_over_sprite;
extern Sprite *play_again_button_sprite;
extern Sprite *play_again_button2_sprite;
extern Sprite *restart_button_sprite;
extern Sprite *restart_button2_sprite;
extern Sprite *you_won_sprite;
extern Sprite *king_charles;
extern Sprite *counter0_sprite;
extern Sprite *counter1_sprite;
extern Sprite *counter2_sprite;
extern Sprite *counter3_sprite;
extern Sprite *counter4_sprite;
extern Sprite *counter5_sprite;
extern Sprite *counter6_sprite;
extern Sprite *counter7_sprite;
extern Sprite *counter8_sprite;
extern Sprite *counter9_sprite;
extern Sprite *flag_counter_sprite;
extern Block* **grid;

void draw_state(){
    if(gameState != PLAY){
        if(menuState == STARTMENU )draw_main_menu();
        if(menuState == CONTROLLERMENU) draw_controls_menu();
        if(menuState == GAMEMENU )draw_game_menu();
        if(menuState == GAMEOVER)draw_game_over_menu();
        if(menuState == WINMENU)draw_win_menu();
    }
   if(gameState == PLAY)draw_game();
   draw_mouse();
}

void draw_main_menu(){
    draw_sprite(check_time_sprite(),0,0);
    draw_sprite(title_sprite,modeInfo.XResolution/4,50);
    draw_sprite(entry == 0 ? start_button2_sprite:start_button_sprite,modeInfo.XResolution/4,modeInfo.YResolution/5 + 50);
    draw_sprite(entry == 1? controls_button2_sprite:controls_button_sprite,modeInfo.XResolution/4,(modeInfo.YResolution*2)/5+ 50);
    draw_sprite(entry==2?exit_button2_sprite:exit_button_sprite,modeInfo.XResolution/4,(modeInfo.YResolution*3)/5+ 50);
}

void draw_game(){
    draw_sprite(dirt_block,0,0);
    draw_sprite(title_sprite,modeInfo.XResolution/4,20);
    draw_sprite(check_time_sprite2(hours/10),modeInfo.XResolution/2-60,100);
    draw_sprite(check_time_sprite2(hours%10),modeInfo.XResolution/2-45,100);
    draw_sprite(check_time_sprite2(minutes/10),modeInfo.XResolution/2-15,100);
    draw_sprite(check_time_sprite2(minutes%10),modeInfo.XResolution/2,100);
    draw_sprite(check_time_sprite2(seconds/10),modeInfo.XResolution/2+30,100);
    draw_sprite(check_time_sprite2(seconds%10),modeInfo.XResolution/2+45,100);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int x = modeInfo.XResolution/2 -( 4 * grid[i][j]->block_sprite->width) + i * grid[i][j]->block_sprite->width;
            int y = modeInfo.YResolution/2 -( 4 * grid[i][j]->block_sprite->height) + j * grid[i][j]->block_sprite->height + 35;
            if(grid[i][j]->state == Not_Revealed){
                if(grid[i][j]->is_selected == true){
                    draw_sprite(selected_grass_sprite , x, y);
                }
                else{
                    draw_sprite(grass_sprite, x, y);
                }
            }
            else if(grid[i][j]->state == Revealed){
                if(grid[i][j]->is_selected == true){
                    draw_sprite(grid[i][j]->selected_block_sprite, x, y);;
                }
                else{
                    draw_sprite(grid[i][j]->block_sprite, x, y);
                }

            }
            else if(grid[i][j]->state == Flagged){
                if(grid[i][j]->is_selected == true){
                    draw_sprite(selected_flag_sprite, x, y);;
                }
                else{
                    draw_sprite(flag_sprite, x, y);
                }

            }

        }
    }
    draw_sprite(flag_counter_sprite,modeInfo.XResolution/4 +100,550);
    draw_counter_sprite();
}

void draw_game_menu(){
    draw_sprite(dirt_block,0,0);
    draw_sprite(paused_title_sprite,modeInfo.XResolution/4,50);
    draw_sprite(check_time_sprite2(hours/10),modeInfo.XResolution/2-60,150);
    draw_sprite(check_time_sprite2(hours%10),modeInfo.XResolution/2-45,150);
    draw_sprite(check_time_sprite2(minutes/10),modeInfo.XResolution/2-15,150);
    draw_sprite(check_time_sprite2(minutes%10),modeInfo.XResolution/2,150);
    draw_sprite(check_time_sprite2(seconds/10),modeInfo.XResolution/2+30,150);
    draw_sprite(check_time_sprite2(seconds%10),modeInfo.XResolution/2+45,150);

    draw_sprite(pause_entry==0?continue_button2_sprite:continue_button_sprite,modeInfo.XResolution/4,modeInfo.YResolution/5 + 75);
    draw_sprite(pause_entry == 1? restart_button2_sprite:restart_button_sprite,modeInfo.XResolution/4,(modeInfo.YResolution*2)/5+ 75);
    draw_sprite(pause_entry == 2? exit_button2_sprite:exit_button_sprite,modeInfo.XResolution/4,(modeInfo.YResolution*3)/5+ 75);
    draw_sprite(dyog_sprite,modeInfo.XResolution-200,modeInfo.YResolution - 200);
    draw_sprite(dyog2_sprite,0,modeInfo.YResolution - 200);
}

void draw_game_over_menu(){
    draw_sprite(dirt_block,0,0);
    draw_sprite(game_over_sprite,modeInfo.XResolution/4,50);
    draw_sprite(game_over_entry==0?play_again_button2_sprite:play_again_button_sprite,modeInfo.XResolution/4,modeInfo.YResolution/5 + 50);
    draw_sprite(game_over_entry == 1? exit_button2_sprite:exit_button_sprite,modeInfo.XResolution/4,(modeInfo.YResolution*2)/5+ 50);
    draw_sprite(dyog_sprite,modeInfo.XResolution-200,modeInfo.YResolution - 200);
    draw_sprite(dyog2_sprite,0,modeInfo.YResolution - 200);
}
void draw_win_menu(){
    draw_sprite(dirt_block,0,0);
    draw_sprite(you_won_sprite,modeInfo.XResolution/4,50);
    draw_sprite(check_time_sprite2(hours/10),modeInfo.XResolution/2-60,150);
    draw_sprite(check_time_sprite2(hours%10),modeInfo.XResolution/2-45,150);
    draw_sprite(check_time_sprite2(minutes/10),modeInfo.XResolution/2-15,150);
    draw_sprite(check_time_sprite2(minutes%10),modeInfo.XResolution/2,150);
    draw_sprite(check_time_sprite2(seconds/10),modeInfo.XResolution/2+30,150);
    draw_sprite(check_time_sprite2(seconds%10),modeInfo.XResolution/2+45,150);
    draw_sprite(win_entry==0?play_again_button2_sprite:play_again_button_sprite,modeInfo.XResolution/4,modeInfo.YResolution/5 + 75);
    draw_sprite(win_entry == 1? exit_button2_sprite:exit_button_sprite,modeInfo.XResolution/4,(modeInfo.YResolution*2)/5+ 75);
    draw_sprite(king_charles, 30, modeInfo.YResolution - king_charles->height - 30 );
    draw_sprite(king_charles, modeInfo.XResolution - 30 - king_charles->width, modeInfo.YResolution - king_charles->height - 30 );
}
void draw_mouse(){
    draw_sprite(cursor_mouse,mouse_x,mouse_y);
}
void draw_controls_menu() {
    draw_sprite(dirt_block,0,0);
    draw_sprite(control_title_sprite,modeInfo.XResolution/4,50);
    draw_sprite(arrows_sprite,modeInfo.XResolution/8,150);
    draw_sprite(right_click_sprite, modeInfo.XResolution/12 + 20, 300);
    draw_sprite(left_click_sprite, modeInfo.XResolution/12 + 120, 300);
    draw_sprite(f_sprite, (modeInfo.XResolution / 8) *4.5, 150);
    draw_sprite(space_sprite, (modeInfo.XResolution / 8) *4.5, 270);
    draw_sprite(esc_sprite, (modeInfo.XResolution / 8) *4.5, 390);
    draw_sprite(entry==0?exit_button2_sprite:exit_button_sprite,modeInfo.XResolution/4,modeInfo.YResolution - 60);
}

int draw_sprite(Sprite *sprite, int x, int y) {
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    sprite->x = x;
    sprite->y= y;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
        for (int w = 0 ; w < width ; w++) {
            current_color = sprite->map[w + h*width];
            if(current_color == TRANSPARENT)continue;
            if (vg_draw_pixel(x + w, y + h, current_color) != 0) continue;
        }
    }
    return 0;
}

Sprite *check_time_sprite(){
    rtc_update_time();
    if (timeIrl.hours >= 22 || timeIrl.hours < 6)return night_sky_sprite;
    else if(timeIrl.hours >= 6 && timeIrl.hours < 14 )return  day_sky_sprite;
    else if(timeIrl.hours >=  14 && timeIrl.hours < 22)return afternoon_sky_sprite;
    return day_sky_sprite;
}

Sprite *check_time_sprite2(uint8_t timer2){
        switch (timer2) {
        case 0:
            return counter0_sprite;
        case 1:
            return counter1_sprite;
        case 2:
            return counter2_sprite;
        case 3:
            return counter3_sprite;
        case 4:
            return counter4_sprite;
        case 5:
            return counter5_sprite;
        case 6:
            return counter6_sprite;
        case 7:
            return counter7_sprite;
        case 8:
            return counter8_sprite;
        case 9:
            return counter9_sprite;
        default:
            return counter0_sprite;
        }
}


void draw_counter_sprite(){
    uint8_t count_flag = 0;
    flag_counter(&count_flag);
    switch (count_flag) {
        case 0:
            draw_sprite(counter0_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 1:
            draw_sprite(counter1_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 2:
            draw_sprite(counter2_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 3:
            draw_sprite(counter3_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 4:
            draw_sprite(counter4_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 5:
            draw_sprite(counter5_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 6:
            draw_sprite(counter6_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 7:
            draw_sprite(counter7_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 8:
            draw_sprite(counter8_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 9:
            draw_sprite(counter9_sprite,modeInfo.XResolution/4  +265,563);
            break;
        case 10:
            draw_sprite(counter1_sprite,modeInfo.XResolution/4  +265,563);
            draw_sprite(counter0_sprite,modeInfo.XResolution/4  +275,563);
            break;
    }
    return ;
}


