#include "draw.h"

extern position bullet_pos[6][3];
int ** bullet_enemy_id_ptr;


void load_xpms(){
    // enemy_pixmaps_test = xpm_load(tower_orange_down, XPM_8_8_8, &enemy_imgs_test);

    cursor_pixmap = xpm_load(cursor, XPM_8_8_8, &cursor_img);
    background_pixmap = xpm_load(background_xpm, XPM_8_8_8, &background_img);
    menu_pixmap = xpm_load(menu_xpm, XPM_8_8_8, &menu_img);
    start_game_pixmap = xpm_load(start_game_xpm, XPM_8_8_8, &start_game_img);
    quit_pixmap = xpm_load(quit_xpm, XPM_8_8_8, &quit_img);
    start_game_glow_pixmap = xpm_load(start_game_glow_xpm, XPM_8_8_8, &start_game_glow_img);
    quit_glow_pixmap = xpm_load(quit_glow_xpm, XPM_8_8_8, &quit_glow_img);
    game_over_pixmap = xpm_load(game_over_xpm, XPM_8_8_8, &game_over_img);

    number_0_pixmap = xpm_load(number_0, XPM_8_8_8, &number_0_img);
    number_1_pixmap = xpm_load(number_1, XPM_8_8_8, &number_1_img);
    number_2_pixmap = xpm_load(number_2, XPM_8_8_8, &number_2_img);
    number_3_pixmap = xpm_load(number_3, XPM_8_8_8, &number_3_img);
    number_4_pixmap = xpm_load(number_4, XPM_8_8_8, &number_4_img);
    number_5_pixmap = xpm_load(number_5, XPM_8_8_8, &number_5_img);
    number_6_pixmap = xpm_load(number_6, XPM_8_8_8, &number_6_img);
    number_7_pixmap = xpm_load(number_7, XPM_8_8_8, &number_7_img);
    number_8_pixmap = xpm_load(number_8, XPM_8_8_8, &number_8_img);
    number_9_pixmap = xpm_load(number_9, XPM_8_8_8, &number_9_img);
    two_dots_pixmap = xpm_load(two_dots, XPM_8_8_8, &two_dots_img);

    tower_orange_up_right_pixmap = xpm_load(tower_orange_up_right, XPM_8_8_8, &tower_orange_up_right_img);
    tower_orange_right_pixmap = xpm_load(tower_orange_right, XPM_8_8_8, &tower_orange_right_img);
    tower_orange_down_right_pixmap = xpm_load(tower_orange_down_right, XPM_8_8_8, &tower_orange_down_right_img);
    tower_orange_down_pixmap = xpm_load(tower_orange_down, XPM_8_8_8, &tower_orange_down_img);
    tower_orange_down_left_pixmap = xpm_load(tower_orange_down_left, XPM_8_8_8, &tower_orange_down_left_img);
    tower_orange_left_pixmap = xpm_load(tower_orange_left, XPM_8_8_8, &tower_orange_left_img);
    tower_orange_up_left_pixmap = xpm_load(tower_orange_up_left, XPM_8_8_8, &tower_orange_up_left_img);
    tower_orange_up_pixmap = xpm_load(tower_orange_up, XPM_8_8_8, &tower_orange_up_img);

    tower_purple_up_right_pixmap = xpm_load(tower_purple_up_right, XPM_8_8_8, &tower_purple_up_right_img);
    tower_purple_right_pixmap = xpm_load(tower_purple_right, XPM_8_8_8, &tower_purple_right_img);
    tower_purple_down_right_pixmap = xpm_load(tower_purple_down_right, XPM_8_8_8, &tower_purple_down_right_img);
    tower_purple_down_pixmap = xpm_load(tower_purple_down, XPM_8_8_8, &tower_purple_down_img);
    tower_purple_down_left_pixmap = xpm_load(tower_purple_down_left, XPM_8_8_8, &tower_purple_down_left_img);
    tower_purple_left_pixmap = xpm_load(tower_purple_left, XPM_8_8_8, &tower_purple_left_img);
    tower_purple_up_left_pixmap = xpm_load(tower_purple_up_left, XPM_8_8_8, &tower_purple_up_left_img);
    tower_purple_up_pixmap = xpm_load(tower_purple_up, XPM_8_8_8, &tower_purple_up_img);

    tower_blue_up_right_pixmap = xpm_load(tower_blue_up_right, XPM_8_8_8, &tower_blue_up_right_img);
    tower_blue_right_pixmap = xpm_load(tower_blue_right, XPM_8_8_8, &tower_blue_right_img);
    tower_blue_down_right_pixmap = xpm_load(tower_blue_down_right, XPM_8_8_8, &tower_blue_down_right_img);
    tower_blue_down_pixmap = xpm_load(tower_blue_down, XPM_8_8_8, &tower_blue_down_img);
    tower_blue_down_left_pixmap = xpm_load(tower_blue_down_left, XPM_8_8_8, &tower_blue_down_left_img);
    tower_blue_left_pixmap = xpm_load(tower_blue_left, XPM_8_8_8, &tower_blue_left_img);
    tower_blue_up_left_pixmap = xpm_load(tower_blue_up_left, XPM_8_8_8, &tower_blue_up_left_img);
    tower_blue_up_pixmap = xpm_load(tower_blue_up, XPM_8_8_8, &tower_blue_up_img);

    // enemy_pixmaps[0] = xpm_load(enemy_fire_skull_walking[0], XPM_8_8_8, &(enemy_imgs[0]));

    for(int i = 0; i < 14; i++){
        enemy_pixmaps[i] = xpm_load(enemy_fire_skull_walking[i], XPM_8_8_8, &enemy_imgs[i]);
        // enemy_pixmaps[i] = xpm_load(tower_orange_up_right, XPM_8_8_8, &(enemy_imgs[i]));
    }
}

void draw_lives(int lives) {

    switch (lives) {
        case 1:
            draw_xpm(710, 152, number_1, 0x36FF00);
            break;
        
        case 2:
            draw_xpm(710, 152, number_2, 0x36FF00);
            break;

        case 3:
            draw_xpm(710, 152, number_3, 0x36FF00);
            break;          
        default:
            break;
    }
}

void draw_money(int coins) {

    if(coins < 0) {draw_xpm(720, 112, number_0, 0x36FF00);}

    switch (coins) {
        case 0:
            draw_xpm(720, 112, number_0, 0x36FF00);
            break;
        case 50:
            draw_xpm(720, 112, number_5, 0x36FF00); draw_xpm(735, 112, number_0, 0x36FF00);
            break;
        case 100:
            draw_xpm(720, 112, number_1, 0x36FF00); draw_xpm(730, 112, number_0, 0x36FF00); draw_xpm(745, 112, number_0, 0x36FF00);
            break;
        case 150:
            draw_xpm(720, 112, number_1, 0x36FF00); draw_xpm(730, 112, number_5, 0x36FF00); draw_xpm(745, 112, number_0, 0x36FF00);
            break;
        case 200:
            draw_xpm(720, 112, number_2, 0x36FF00); draw_xpm(735, 112, number_0, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break;           
        case 250:
            draw_xpm(720, 112, number_2, 0x36FF00); draw_xpm(735, 112, number_5, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break;  
        case 300:
            draw_xpm(720, 112, number_3, 0x36FF00); draw_xpm(735, 112, number_0, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break;  
        case 350:
            draw_xpm(720, 112, number_3, 0x36FF00); draw_xpm(735, 112, number_5, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break; 
        case 400:
            draw_xpm(720, 112, number_4, 0x36FF00); draw_xpm(735, 112, number_0, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break; 
        case 450:
            draw_xpm(720, 112, number_4, 0x36FF00); draw_xpm(735, 112, number_5, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break; 
        case 500:
            draw_xpm(720, 112, number_5, 0x36FF00); draw_xpm(735, 112, number_0, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break; 
        case 550:
            draw_xpm(720, 112, number_5, 0x36FF00); draw_xpm(735, 112, number_5, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break; 
        case 600:
            draw_xpm(720, 112, number_6, 0x36FF00); draw_xpm(735, 112, number_0, 0x36FF00); draw_xpm(750, 112, number_0, 0x36FF00);
            break; 

        default:
            break;
    }
}

void draw_number(int number, uint16_t x, uint16_t y) {
        switch (number) {
        case 0:
            draw_xpm(x, y, number_0, 0x36FF00);
            break;
        case 1:
            draw_xpm(x, y, number_1, 0x36FF00);
            break;
        case 2:
            draw_xpm(x, y, number_2, 0x36FF00); 
            break;
        case 3:
            draw_xpm(x, y, number_3, 0x36FF00); 
            break;
        case 4:
            draw_xpm(x, y, number_4, 0x36FF00); 
            break;           
        case 5:
            draw_xpm(x, y, number_5, 0x36FF00); 
            break;  
        case 6:
            draw_xpm(x, y, number_6, 0x36FF00); 
            break;  
        case 7:
            draw_xpm(x, y, number_7, 0x36FF00); 
            break; 
        case 8:
            draw_xpm(x, y, number_8, 0x36FF00); 
            break; 
        case 9:
            draw_xpm(x, y, number_9, 0x36FF00);
            break; 
        default:
            break;
    }
}

void draw_orange_tower_directing_to_enemy(int tower_id, int tower_x, int tower_y, enemy* enemies, int range){
    int enemy_id = -1;

    for(int i = 0; i < 4; i++){
        if(enemies[i].hp <= 0) continue;
        int tower_x_center =  tower_x + 15;
        int tower_y_center = tower_y + 15;
        int enemy_x_center = enemies[i].x + 15;
        int enemy_y_center = enemies[i].y + 15;
        double y_difference = enemy_y_center - tower_y_center;
        double x_difference = enemy_x_center -  tower_x_center;

        if(y_difference*y_difference + x_difference*x_difference <= range*range){
            enemy_id = i;
            break;
        }
    }

    if(enemy_id == -1){
        if(last_position_orange[tower_id] == 0){
            /* draw_xpm(tower_x, tower_y, tower_orange_up_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_orange_up_right_pixmap, tower_orange_up_right_img, 0xFFFFFF);
        } else if(last_position_orange[tower_id] == 1){
            /* draw_xpm(tower_x, tower_y, tower_orange_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_orange_right_pixmap, tower_orange_right_img, 0xFFFFFF);
       } else if(last_position_orange[tower_id] == 2){
            /* draw_xpm(tower_x, tower_y, tower_orange_down_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_orange_down_right_pixmap, tower_orange_down_right_img, 0xFFFFFF);
        } else if(last_position_orange[tower_id] == 3){
            /* draw_xpm(tower_x, tower_y, tower_orange_down, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_orange_down_pixmap, tower_orange_down_img, 0xFFFFFF);
        }else if(last_position_orange[tower_id] == 4){
            /* draw_xpm(tower_x, tower_y, tower_orange_down_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_orange_down_left_pixmap, tower_orange_down_left_img, 0xFFFFFF);
        }else if(last_position_orange[tower_id] == 5){
            /* draw_xpm(tower_x, tower_y, tower_orange_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_orange_left_pixmap, tower_orange_left_img, 0xFFFFFF);
        }else if(last_position_orange[tower_id] == 6){
            /* draw_xpm(tower_x, tower_y, tower_orange_up_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_orange_up_left_pixmap, tower_orange_up_left_img, 0xFFFFFF);
        }else{
            /* draw_xpm(tower_x, tower_y, tower_orange_up, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_orange_up_pixmap, tower_orange_up_img, 0xFFFFFF);
        }
        return;
    }
    
    int delta_x;
    int delta_y;
    delta_x = tower_x - enemies[enemy_id].x;
    delta_y = tower_y - enemies[enemy_id].y;
    int angle = atan2(delta_y, delta_x) * 180 / 3.1416 - 90;
    if(angle > 22.5 && angle < 67.5){
        /* draw_xpm(tower_x, tower_y, tower_orange_up_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_orange_up_right_pixmap, tower_orange_up_right_img, 0xFFFFFF);
        last_position_orange[tower_id] = 0;
    }
    else if(angle >= 67.5 && angle < 112.5){
        /* draw_xpm(tower_x, tower_y, tower_orange_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_orange_right_pixmap, tower_orange_right_img, 0xFFFFFF);
        last_position_orange[tower_id] = 1;
    }
    else if(angle >= 112.5 && angle < 157.5){
        /* draw_xpm(tower_x, tower_y, tower_orange_down_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_orange_down_right_pixmap, tower_orange_down_right_img, 0xFFFFFF);
        last_position_orange[tower_id] = 2;
    }
    else if(angle >= 157.5 || angle < -157.5){
        /* draw_xpm(tower_x, tower_y, tower_orange_down, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_orange_down_pixmap, tower_orange_down_img, 0xFFFFFF);
        last_position_orange[tower_id] = 3;
    }
    else if(angle >= -157.5 && angle < -112.5){
        /* draw_xpm(tower_x, tower_y, tower_orange_down_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_orange_down_left_pixmap, tower_orange_down_left_img, 0xFFFFFF);
        last_position_orange[tower_id] = 4;
    }
    else if(angle >= -112.5 && angle < -67.5){
        /* draw_xpm(tower_x, tower_y, tower_orange_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_orange_left_pixmap, tower_orange_left_img, 0xFFFFFF);
        last_position_orange[tower_id] = 5;
    }
    else if(angle >= -67.5 && angle < -22.5){
        /* draw_xpm(tower_x, tower_y, tower_orange_up_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_orange_up_left_pixmap, tower_orange_up_left_img, 0xFFFFFF);
        last_position_orange[tower_id] = 6;
    }
    else{
        /* draw_xpm(tower_x, tower_y, tower_orange_up, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_orange_up_pixmap, tower_orange_up_img, 0xFFFFFF);
        last_position_orange[tower_id] = 7;
    }
}


void draw_purple_tower_directing_to_enemy(int tower_id, int tower_x, int tower_y, enemy* enemies, int range){
    int enemy_id = -1;

    for(int i = 0; i < 4; i++){
        if(enemies[i].hp <= 0) continue;
        int tower_x_center =  tower_x + 15;
        int tower_y_center = tower_y + 15;
        int enemy_x_center = enemies[i].x + 15;
        int enemy_y_center = enemies[i].y + 15;
        double y_difference = enemy_y_center - tower_y_center;
        double x_difference = enemy_x_center -  tower_x_center;

        if(y_difference*y_difference + x_difference*x_difference <= range*range){
            enemy_id = i;
            break;
        }
    }

    if(enemy_id == -1){
        if(last_position_purple[tower_id] == 0){
            /* draw_xpm(tower_x, tower_y, tower_purple_up_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_purple_up_right_pixmap, tower_purple_up_right_img, 0xFFFFFF);
        } else if(last_position_purple[tower_id] == 1){
            /* draw_xpm(tower_x, tower_y, tower_purple_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_purple_right_pixmap, tower_purple_right_img, 0xFFFFFF);
       } else if(last_position_purple[tower_id] == 2){
            /* draw_xpm(tower_x, tower_y, tower_purple_down_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_purple_down_right_pixmap, tower_purple_down_right_img, 0xFFFFFF);
        } else if(last_position_purple[tower_id] == 3){
            /* draw_xpm(tower_x, tower_y, tower_purple_down, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_purple_down_pixmap, tower_purple_down_img, 0xFFFFFF);
        }else if(last_position_purple[tower_id] == 4){
            /* draw_xpm(tower_x, tower_y, tower_purple_down_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_purple_down_left_pixmap, tower_purple_down_left_img, 0xFFFFFF);
        }else if(last_position_purple[tower_id] == 5){
            /* draw_xpm(tower_x, tower_y, tower_purple_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_purple_left_pixmap, tower_purple_left_img, 0xFFFFFF);
        }else if(last_position_purple[tower_id] == 6){
            /* draw_xpm(tower_x, tower_y, tower_purple_up_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_purple_up_left_pixmap, tower_purple_up_left_img, 0xFFFFFF);
        }else{
            /* draw_xpm(tower_x, tower_y, tower_purple_up, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_purple_up_pixmap, tower_purple_up_img, 0xFFFFFF);
        }
        return;
    }
    
    int delta_x;
    int delta_y;
    delta_x = tower_x - enemies[enemy_id].x;
    delta_y = tower_y - enemies[enemy_id].y;
    int angle = atan2(delta_y, delta_x) * 180 / 3.1416 - 90;
    if(angle > 22.5 && angle < 67.5){
        /* draw_xpm(tower_x, tower_y, tower_purple_up_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_purple_up_right_pixmap, tower_purple_up_right_img, 0xFFFFFF);
        last_position_purple[tower_id] = 0;
    }
    else if(angle >= 67.5 && angle < 112.5){
        /* draw_xpm(tower_x, tower_y, tower_purple_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_purple_right_pixmap, tower_purple_right_img, 0xFFFFFF);
        last_position_purple[tower_id] = 1;
    }
    else if(angle >= 112.5 && angle < 157.5){
        /* draw_xpm(tower_x, tower_y, tower_purple_down_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_purple_down_right_pixmap, tower_purple_down_right_img, 0xFFFFFF);
        last_position_purple[tower_id] = 2;
    }
    else if(angle >= 157.5 || angle < -157.5){
        /* draw_xpm(tower_x, tower_y, tower_purple_down, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_purple_down_pixmap, tower_purple_down_img, 0xFFFFFF);
        last_position_purple[tower_id] = 3;
    }
    else if(angle >= -157.5 && angle < -112.5){
        /* draw_xpm(tower_x, tower_y, tower_purple_down_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_purple_down_left_pixmap, tower_purple_down_left_img, 0xFFFFFF);
        last_position_purple[tower_id] = 4;
    }
    else if(angle >= -112.5 && angle < -67.5){
        /* draw_xpm(tower_x, tower_y, tower_purple_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_purple_left_pixmap, tower_purple_left_img, 0xFFFFFF);
        last_position_purple[tower_id] = 5;
    }
    else if(angle >= -67.5 && angle < -22.5){
        /* draw_xpm(tower_x, tower_y, tower_purple_up_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_purple_up_left_pixmap, tower_purple_up_left_img, 0xFFFFFF);
        last_position_purple[tower_id] = 6;
    }
    else{
        /* draw_xpm(tower_x, tower_y, tower_purple_up, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_purple_up_pixmap, tower_purple_up_img, 0xFFFFFF);
        last_position_purple[tower_id] = 7;
    }
}

void draw_blue_tower_directing_to_enemy(int tower_id, int tower_x, int tower_y, enemy* enemies, int range){
    int enemy_id = -1;

    for(int i = 0; i < 4; i++){
        if(enemies[i].hp <= 0) continue;
        int tower_x_center =  tower_x + 15;
        int tower_y_center = tower_y + 15;
        int enemy_x_center = enemies[i].x + 15;
        int enemy_y_center = enemies[i].y + 15;
        double y_difference = enemy_y_center - tower_y_center;
        double x_difference = enemy_x_center -  tower_x_center;

        if(y_difference*y_difference + x_difference*x_difference <= range*range){
            enemy_id = i;
            break;
        }
    }

    if(enemy_id == -1){
        if(last_position_blue[tower_id] == 0){
            /* draw_xpm(tower_x, tower_y, tower_blue_up_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_blue_up_right_pixmap, tower_blue_up_right_img, 0xFFFFFF);
        } else if(last_position_blue[tower_id] == 1){
            /* draw_xpm(tower_x, tower_y, tower_blue_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);
       } else if(last_position_blue[tower_id] == 2){
            /* draw_xpm(tower_x, tower_y, tower_blue_down_right, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_blue_down_right_pixmap, tower_blue_down_right_img, 0xFFFFFF);
        } else if(last_position_blue[tower_id] == 3){
            /* draw_xpm(tower_x, tower_y, tower_blue_down, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_blue_down_pixmap, tower_blue_down_img, 0xFFFFFF);
        }else if(last_position_blue[tower_id] == 4){
            /* draw_xpm(tower_x, tower_y, tower_blue_down_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_blue_down_left_pixmap, tower_blue_down_left_img, 0xFFFFFF);
        }else if(last_position_blue[tower_id] == 5){
            /* draw_xpm(tower_x, tower_y, tower_blue_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_blue_left_pixmap, tower_blue_left_img, 0xFFFFFF);
        }else if(last_position_blue[tower_id] == 6){
            /* draw_xpm(tower_x, tower_y, tower_blue_up_left, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_blue_up_left_pixmap, tower_blue_up_left_img, 0xFFFFFF);
        }else{
            /* draw_xpm(tower_x, tower_y, tower_blue_up, 0xFFFFFF); */
            draw_xpm_loaded(tower_x, tower_y, tower_blue_up_pixmap, tower_blue_up_img, 0xFFFFFF);
        }
        return;
    }
    
    int delta_x;
    int delta_y;
    delta_x = tower_x - enemies[enemy_id].x;
    delta_y = tower_y - enemies[enemy_id].y;
    int angle = atan2(delta_y, delta_x) * 180 / 3.1416 - 90;
    if(angle > 22.5 && angle < 67.5){
        /* draw_xpm(tower_x, tower_y, tower_blue_up_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_blue_up_right_pixmap, tower_blue_up_right_img, 0xFFFFFF);
        last_position_blue[tower_id] = 0;
    }
    else if(angle >= 67.5 && angle < 112.5){
        /* draw_xpm(tower_x, tower_y, tower_blue_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);
        last_position_blue[tower_id] = 1;
    }
    else if(angle >= 112.5 && angle < 157.5){
        /* draw_xpm(tower_x, tower_y, tower_blue_down_right, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_blue_down_right_pixmap, tower_blue_down_right_img, 0xFFFFFF);
        last_position_blue[tower_id] = 2;
    }
    else if(angle >= 157.5 || angle < -157.5){
        /* draw_xpm(tower_x, tower_y, tower_blue_down, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_blue_down_pixmap, tower_blue_down_img, 0xFFFFFF);
        last_position_blue[tower_id] = 3;
    }
    else if(angle >= -157.5 && angle < -112.5){
        /* draw_xpm(tower_x, tower_y, tower_blue_down_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_blue_down_left_pixmap, tower_blue_down_left_img, 0xFFFFFF);
        last_position_blue[tower_id] = 4;
    }
    else if(angle >= -112.5 && angle < -67.5){
        /* draw_xpm(tower_x, tower_y, tower_blue_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_blue_left_pixmap, tower_blue_left_img, 0xFFFFFF);
        last_position_blue[tower_id] = 5;
    }
    else if(angle >= -67.5 && angle < -22.5){
        /* draw_xpm(tower_x, tower_y, tower_blue_up_left, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_blue_up_left_pixmap, tower_blue_up_left_img, 0xFFFFFF);
        last_position_blue[tower_id] = 6;
    }
    else{
        /* draw_xpm(tower_x, tower_y, tower_blue_up, 0xFFFFFF); */
        draw_xpm_loaded(tower_x, tower_y, tower_blue_up_pixmap, tower_blue_up_img, 0xFFFFFF);
        last_position_blue[tower_id] = 7;
    }
}
