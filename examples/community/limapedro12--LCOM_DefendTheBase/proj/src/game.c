#include "game.h"
#include "our_framework/framework_essencials.h"
#include "draw.h"
#include "xpm/test.xpm"
#include "xpm/tower_purple/tower_purple.h"
#include "xpm/tower_orange/tower_orange.h"
#include "xpm/tower_blue/tower_blue.h"
#include "xpm/numbers/numbers.h"
#include "player.h"
#include "drag.h"
#include "bullet.h"
#include "night_mode.h"

int time_0;
int time_start_round;
bool menu_state = true;

bool game_clock = false;

int lives = 3;
int coins = 100;

tower towers_level_1[6] = {{1, 605, 218, true, false}, {1, 605, 218, false, false}, {1, 605, 218, false, false}, {1, 605, 218, false, false}, {1, 605, 218, false, false}, {1, 605, 218, false, false}};
tower towers_level_2[6] = {{2, 672, 220, true, false}, {2, 672, 220, false, false}, {2, 672, 220, false, false}, {2, 672, 220, false, false}, {2, 672, 220, false, false}, {2, 672, 220, false, false}};
tower towers_level_3[6] = {{3, 740, 218, true, false}, {3, 740, 218, false, false}, {3, 740, 218, false, false}, {3, 740, 218, false, false}, {3, 740, 218, false, false}, {3, 740, 218, false, false}};


enemy enemies[50];
char enemy_directions[50];
int current_enemy_direction[50];
int enemy_animation = 0;

position corners[12] = {{250, 50}, {250, 150}, {100, 150}, {100, 250}, {400, 250}, {400, 50}, {500, 50}, {500, 350}, {400, 350}, {400, 400}, {50, 400}, {50, 500}};
char directions[12] = {'d', 'l', 'd', 'r', 'u', 'r', 'd', 'l', 'd', 'l', 'd', 'r'};
char direction = 'r';

uint round_number = 0;
uint number_of_enemies_by_round[10] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};


int before(){

  turn_on_graphics();
  load_xpms();
  start_night_mode(background_xpm);

  for(unsigned int i = 0; i < sizeof(enemies) / sizeof(enemies[0]); i++){
    enemies[i] = (enemy){-10, 50, 3};
    enemy_directions[i] = 'r';
    current_enemy_direction[i] = 0;
  }

  return 0;
}

bool updated_background = false;
void game(){

  if(!updated_background && get_hours() != 33){
    updated_background = true;
    apply_night_mode_on_xpm(background_pixmap, background_img);
  }
  
  clear_screen();
  verify_mouse_limits(0, 800-10, 0, 600-10);

  if(menu_state) {
    draw_xpm_loaded(0, 0, menu_pixmap, menu_img, NO_BACKGROUND);
    // draw_rectangle(250, 190, 300, 50, 0x00e600);
    // draw_rectangle(250, 290, 300, 50, 0x86592d);
    draw_xpm_loaded(257, 314, start_game_pixmap, start_game_img, 0xFFFFFF); //54, 281
    draw_xpm_loaded(341, 421, quit_pixmap, quit_img, 0xFFFFFF);//54, 114

    if(is_key_pressed(ESC, true)){
      quit();
    }

    if(is_key_pressed(ENTER, false)){
      menu_state = false;
      time_0 = get_time_counter();
    }

    if(get_mouse_position().x >= 257 && get_mouse_position().x <= 257 + 281 && 
       get_mouse_position().y >= 314 && get_mouse_position().y <= 314 + 54){
      draw_xpm_loaded(257, 314, start_game_glow_pixmap, start_game_glow_img, 0xFFFFFF);
      if(is_lb_pressed()){
        menu_state = false;
        time_0 = get_time_counter();
      }
    }
    if(get_mouse_position().x >= 341 && get_mouse_position().x <= 341 + 114&& 
       get_mouse_position().y >= 421 && get_mouse_position().y <= 421 + 54){
      draw_xpm_loaded(341, 421, quit_glow_pixmap, quit_glow_img, 0xFFFFFF);
      if(is_lb_pressed()){
        quit();
      }
    }

    //Draw time
    if((get_hours() / 10) == 1) {
      draw_number(get_hours() / 10, 30, 550);
      draw_number(get_hours() % 10, 40, 550);
    }
    else {
      draw_number(get_hours() / 10, 30, 550);
      draw_number(get_hours() % 10, 45, 550);
    }

    draw_xpm_loaded(59, 553, two_dots_pixmap, two_dots_img, 0x00FF12);

    if((get_minutes() / 10) == 1) {
      draw_number(get_minutes() / 10, 65, 550);
      draw_number(get_minutes() % 10, 75, 550);
    }
    else {
      draw_number(get_minutes() / 10, 65, 550);
      draw_number(get_minutes() % 10, 80, 550);
    }
  } 

  else if(lives <= 0){
    draw_xpm_loaded(0, 0, game_over_pixmap, game_over_img, NO_BACKGROUND);
    if(is_key_pressed(ESC, true) || is_key_pressed(ENTER, true)){
      quit();
    }
  }

  else {
    draw_xpm_loaded(0, 0, background_pixmap, background_img, NO_BACKGROUND);

    if(is_key_pressed(ESC, true)){
      menu_state = true;
    }

    if(is_key_pressed(' ', true)){
      if(!game_clock){
        game_clock = true;
        time_start_round = get_time_counter();
      }
    }

    // if(lives == 0) {
    //   quit();
    // }

    int enemies_alive_2 = 0;

    if(game_clock) {
      int enemies_alive = 6;

      for(unsigned int i = 0; i < number_of_enemies_by_round[round_number]; i++) {
        if(game_clock && is_time_interval_elapsed_milliseconds(time_start_round, (i*2000)/(round_number+1))  && enemies[i].hp > 0){

          if(enemy_directions[i] == 'r') {
            enemies[i].x += 2;
          }
          if(enemy_directions[i] == 'l') {
            enemies[i].x -= 2;
          }
          if(enemy_directions[i] == 'u') {
            enemies[i].y -= 2;
          }
          if(enemy_directions[i] == 'd') {
            enemies[i].y += 2;
          }
              
          if(enemies[i].x == corners[current_enemy_direction[i]].x && 
            enemies[i].y == corners[current_enemy_direction[i]].y) {
            enemy_directions[i] = directions[current_enemy_direction[i]];
            current_enemy_direction[i]++;
          }

          if(enemies[i].x == 11*50-20 && enemies[i].y == 500) {
            lives--;
            enemies[i].hp = 0;
            coins -= 50;
          }
        }

        if(enemies[i].hp <= 0) {
          enemies_alive--;
        } 
        else {
          enemies_alive_2++;
        }
      }

      if(enemies_alive_2 == 0) {
        for(unsigned int i = 0; i < sizeof(enemies) / sizeof(enemies[0]); i++) {
          enemies[i].x = -10;
          enemies[i].y = 50;
          enemies[i].hp = 3;
          enemy_directions[i] = 'r';
          current_enemy_direction[i] = 0;
        }

        coins += 150;

        if(round_number < 9) {
          round_number++;
        } 
        game_clock = false;
      }

      //Draw towerS
      for(unsigned int i = 0; i < sizeof(towers_level_1) / sizeof(towers_level_1[0]); i++) {
        if(towers_level_1[i].placed) {
          int bullet_speed = 5*towers_level_1[i].level;
          int bullet_range = 150*towers_level_1[i].level;
          int bullet_time = 2000/towers_level_1[i].level;

          for(unsigned int j = 0; j < sizeof(enemies) / sizeof(enemies[0]);j++) {
            if(enemies[j].hp > 0 && drawBullet(towers_level_1[i].x, towers_level_1[i].y, enemies[j], i, j, bullet_speed, bullet_range, bullet_time, 1)) {
              enemies[j].hp--;
            }
          }

          if(towers_level_1[i].level == 1) {
            draw_orange_tower_directing_to_enemy(i, towers_level_1[i].x, towers_level_1[i].y, enemies, bullet_range);
          }         
          else if(towers_level_1[i].level == 2) {   
            // draw_xpm_loaded(towers_level_1[i].x, towers_level_1[i].y, tower_purple_right_pixmap, tower_purple_right_img, 0xFFFFFF);        
            draw_purple_tower_directing_to_enemy(i, towers_level_1[i].x, towers_level_1[i].y, enemies, bullet_range);
          }
          else {   
            // draw_xpm_loaded(towers_level_1[i].x, towers_level_1[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);        
            draw_blue_tower_directing_to_enemy(i, towers_level_1[i].x, towers_level_1[i].y, enemies, bullet_range);
          }
        }
      }

      for(unsigned int i = 0; i < sizeof(towers_level_2) / sizeof(towers_level_2[0]); i++) {
        if(towers_level_2[i].placed) {
          int bullet_speed = 5*towers_level_2[i].level;
          int bullet_range = 150*towers_level_2[i].level;
          int bullet_time = 2000/towers_level_2[i].level;

          for(unsigned int j = 0; j < sizeof(enemies) / sizeof(enemies[0]);j++) {
            if(drawBullet(towers_level_2[i].x, towers_level_2[i].y, enemies[j], i, j, bullet_speed, bullet_range, bullet_time, 2) && enemies[j].hp > 0) {           
              enemies[j].hp--;
            }
          }

          if(towers_level_2[i].level == 2) {
            // draw_xpm_loaded(towers_level_2[i].x, towers_level_2[i].y, tower_purple_right_pixmap, tower_purple_right_img, 0xFFFFFF);
            draw_purple_tower_directing_to_enemy(i, towers_level_2[i].x, towers_level_2[i].y, enemies, bullet_range);
          }         
          else {
            // draw_xpm_loaded(towers_level_2[i].x, towers_level_2[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);
            draw_blue_tower_directing_to_enemy(i, towers_level_2[i].x, towers_level_2[i].y, enemies, bullet_range);
          }
        }
      }

      for(unsigned int i = 0; i < sizeof(towers_level_3) / sizeof(towers_level_3[0]); i++) {
        if(towers_level_3[i].placed) {
          int bullet_speed = 5*towers_level_3[i].level;
          int bullet_range = 150*towers_level_3[i].level;
          int bullet_time = 2000/towers_level_3[i].level;

          for(unsigned int j = 0; j < sizeof(enemies) / sizeof(enemies[0]);j++) {
            if(drawBullet(towers_level_3[i].x, towers_level_3[i].y, enemies[j], i, j, bullet_speed, bullet_range, bullet_time, 3) && enemies[j].hp > 0) {
              enemies[j].hp--;
            }
          }

          // draw_xpm_loaded(towers_level_3[i].x, towers_level_3[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);
          draw_blue_tower_directing_to_enemy(i, towers_level_3[i].x, towers_level_3[i].y, enemies, bullet_range);
        }
      }
    }



    //Game paused

    else {

      //Towers Level 1
      for(unsigned int i = 0; i < sizeof(towers_level_1) / sizeof(towers_level_1[0]); i++) {
        if(towers_level_1[i].new) {
          if(towers_level_1[i].level == 1) {
            draw_xpm_loaded(towers_level_1[i].x, towers_level_1[i].y, tower_orange_right_pixmap, tower_orange_right_img, 0xFFFFFF);
          }
          else if(towers_level_1[i].level == 2) {   
            draw_xpm_loaded(towers_level_1[i].x, towers_level_1[i].y, tower_purple_right_pixmap, tower_purple_right_img, 0xFFFFFF);        
          }
          else {   
            draw_xpm_loaded(towers_level_1[i].x, towers_level_1[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);        
          }
          
          if(coins >= 50) {
            verifyDrag(&towers_level_1[i].x, &towers_level_1[i].y, &towers_level_1[i].new, &towers_level_1[i+1].new, &towers_level_1[i].placed, &towers_level_1[i+1].placed, &towers_level_1[i].level, &coins, background_pixmap, background_img, towers_level_1, towers_level_2, towers_level_3);
          }
        }
        else {
          if(towers_level_1[i].placed) {
            if(towers_level_1[i].level == 1) {
              draw_xpm_loaded(towers_level_1[i].x, towers_level_1[i].y, tower_orange_right_pixmap, tower_orange_right_img, 0xFFFFFF);
            }
            else if(towers_level_1[i].level == 2) {   
              draw_xpm_loaded(towers_level_1[i].x, towers_level_1[i].y, tower_purple_right_pixmap, tower_purple_right_img, 0xFFFFFF);        
            }
            else {   
              draw_xpm_loaded(towers_level_1[i].x, towers_level_1[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);        
            }

            if(coins >= 50) {
              verifyUpgrade(&towers_level_1[i].x, &towers_level_1[i].y, &towers_level_1[i].level, &coins);
            }
          }
        }
      }

      //Towers Level 2
      for(unsigned int i = 0; i < sizeof(towers_level_2) / sizeof(towers_level_2[0]); i++) {
        if(towers_level_2[i].new) {
          if(towers_level_2[i].level == 2) { 
            draw_xpm_loaded(towers_level_2[i].x, towers_level_2[i].y, tower_purple_right_pixmap, tower_purple_right_img, 0xFFFFFF);
          }   
          else {   
            draw_xpm_loaded(towers_level_2[i].x, towers_level_2[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);        
          }

          if(coins >= 100) {
            verifyDrag(&towers_level_2[i].x, &towers_level_2[i].y, &towers_level_2[i].new, &towers_level_2[i+1].new, &towers_level_2[i].placed, &towers_level_2[i+1].placed, &towers_level_2[i].level, &coins, background_pixmap, background_img, towers_level_1, towers_level_2, towers_level_3);
          }
        }
        else {
          if(towers_level_2[i].placed) {
            if(towers_level_2[i].level == 2) {
              draw_xpm_loaded(towers_level_2[i].x, towers_level_2[i].y, tower_purple_right_pixmap, tower_purple_right_img, 0xFFFFFF);
            }   
            else {   
              draw_xpm_loaded(towers_level_2[i].x, towers_level_2[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);        
            }

            if(coins >= 50) {
              verifyUpgrade(&towers_level_2[i].x, &towers_level_2[i].y, &towers_level_2[i].level, &coins);
            }
          }
        }
      }

      //Towers Level 3
      for(unsigned int i = 0; i < sizeof(towers_level_3) / sizeof(towers_level_3[0]); i++) {
        if(towers_level_3[i].new) { 
          draw_xpm_loaded(towers_level_3[i].x, towers_level_3[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF); 
          
          if(coins >= 150) {
            verifyDrag(&towers_level_3[i].x, &towers_level_3[i].y, &towers_level_3[i].new, &towers_level_3[i+1].new, &towers_level_3[i].placed, &towers_level_3[i+1].placed, &towers_level_3[i].level, &coins, background_pixmap, background_img, towers_level_1, towers_level_2, towers_level_3);
          }
        }
        else {
          if(towers_level_3[i].placed) {
            draw_xpm_loaded(towers_level_3[i].x, towers_level_3[i].y, tower_blue_right_pixmap, tower_blue_right_img, 0xFFFFFF);
          }
        }
      }
    }

    //Draw enemies
    for(unsigned int i = 0; i < sizeof(enemies) / sizeof(enemies[0]); i++) {
      if(enemies[i].hp > 0 && enemies[i].x < 570 && enemies[i].y < 600) {
        
        if(enemy_animation == 13) enemy_animation = 0;
        draw_xpm_loaded(enemies[i].x, enemies[i].y, enemy_pixmaps[enemy_animation], enemy_imgs[enemy_animation], 0xFFFFFF);
        enemy_animation++;
      }
    }

    draw_lives(lives);
    draw_money(coins);

    // if(get_time_counter() % 5 == 0)
    //   apply_night_mode();
  }
  draw_xpm_loaded(get_mouse_position().x, get_mouse_position().y, cursor_pixmap, cursor_img, 0x2AFF00);
}

int after(){
  double animal = 0.5 * 1000;
  printf("value = %d\n", animal);//get_value_to_multiply());
  return 0;
}
