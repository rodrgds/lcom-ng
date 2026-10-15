#include <lcom/lcf.h>
#include "game.h"
#include "graphics/video.h"
#include "graphics/sprites.h"
#include "drivers/mouse.h"
#include "drivers/keyboard.h"

game_state_t game_state = MENU_STATE;

extern int x_init;
extern int y_init;
extern struct packet pp;

extern uint8_t scan_code;
extern int counter;

int speed = 5;
int bullet_speed = 50;
int asteroid_speed = 15;
int score = 0;
int highest_score = 0;
int x_spaceship = X_RES/2;

int sec_time = 0;

int tam = 1;

int draw_menu_background = 0;

typedef struct {
  int x;
  int y;
  int active;
} bullet;

typedef struct {
  int x;
  int y;
  int active;
} asteroid;

bullet bullets[50];

asteroid asteroids[25];

void (process_mouse_packet)() {
  mouse_print_packet(&pp); // Optional, remove for performance
  mouse_handler();
  memset(&pp.bytes, 0, sizeof(pp.bytes));
}

void (mouse_handler)() {
  if (game_state == MENU_STATE) {
    track_mouse_movement();
    get_menu();
    draw_mouse();
    load_buffer();

    if (play_pressed()) {
      game_state = GAME_STATE;
      clear_all_buffers();
      load_game_background();
      get_game_background();
      draw_all_asteroids();
      draw_spaceship(x_spaceship);
      draw_score(score, X_SCORE_GAME, Y_SCORE_GAME);
      draw_time(sec_time);
      load_buffer();
    }
    else if (help_pressed()) {
      game_state = HELP_STATE;
      clear_all_buffers();
      draw_help_menu();
      load_buffer();
    }
    else if (exit_pressed()) {
      game_state = EXIT_STATE;
      vg_exit();
      return;
    }
  }
}

void (keyboard_handler)() {
  if(game_state == HELP_STATE) {
    if(scan_code == M_KEY_PRESSED) {
      game_state = MENU_STATE;
      clear_all_buffers();
      draw_menu_background = 0;
      load_menu_background();
      get_menu();
      draw_mouse();
      load_buffer();
    }
  }
  if(game_state == GAME_OVER_STATE){
    if(scan_code == M_KEY_PRESSED) {
      game_state = MENU_STATE;
      clear_all_buffers();
      draw_menu_background = 0;
      load_menu_background();
      get_menu();
      draw_mouse();
      load_buffer();
    }
  }
}

void(timer_handler)(){
  if(game_state == MENU_STATE) {
    if (draw_menu_background == 0) {
      draw_menu_background = 1;
      load_menu_background();
      get_menu();
      draw_mouse();
      load_buffer();
    }
  }
  
  if(game_state == GAME_STATE) {
    check_collision();
    if(is_game_over()) {
      game_state = GAME_OVER_STATE;
      clear_all_buffers();
      draw_game_over();
      draw_score(score, X_RES/2 - 50, Y_RES/2 - 110);
      draw_score(highest_score, X_RES/2 - 50, Y_RES/2 + 130);
      score = 0;
      load_buffer();
      sec_time = 0;
    }
    if(counter%30==0){
      move_bullets();
      fire_bullet();
      get_game_background();
      draw_all_bullets();
      draw_all_asteroids();
      draw_spaceship(x_spaceship);
      draw_score(score, X_SCORE_GAME, Y_SCORE_GAME);
      draw_time(sec_time);
      load_buffer();
    }
    if(counter%30==0){
      get_game_background();
      draw_all_bullets();
      move_asteroids();
      draw_all_asteroids();
      draw_spaceship(x_spaceship);
      draw_score(score, X_SCORE_GAME, Y_SCORE_GAME);
      draw_time(sec_time);
      load_buffer();
    }
    if(counter%60==0){
      sec_time++;
      move_asteroids();
      get_game_background();
      create_asteroid();
      draw_all_asteroids();
      draw_spaceship(x_spaceship);
      draw_all_bullets();
      draw_score(score, X_SCORE_GAME, Y_SCORE_GAME);
      draw_time(sec_time);
      load_buffer();
    }
  }
    
}

int (play_pressed)() {
  return x_init >= PLAYBUTTON_X && x_init <= PLAYBUTTON_X + PLAYBUTTON_WIDTH &&
         y_init >= PLAYBUTTON_Y && y_init <= PLAYBUTTON_Y + PLAYBUTTON_HEIGHT &&
         pp.lb == 1;
}

int (exit_pressed)() {
  return x_init >= EXITBUTTON_X && x_init <= EXITBUTTON_X + EXITBUTTON_WIDTH &&
         y_init >= EXITBUTTON_Y && y_init <= EXITBUTTON_Y + EXITBUTTON_HEIGHT &&
         pp.lb == 1;
}

int (help_pressed)(){
  return x_init >= HELPBUTTON_X && x_init <= HELPBUTTON_X + HELPBUTTON_WIDTH &&
         y_init >= HELPBUTTON_Y && y_init <= HELPBUTTON_Y + HELPBUTTON_HEIGHT &&
         pp.lb == 1;
}


void(spaceship_move)(uint8_t array[], int tam){
  if(game_state == GAME_STATE) {
    if (tam == 1) {
      switch (array[0]) {
        case A_KEY_PRESSED:
          x_spaceship -= 30;
          if(x_spaceship < 0) x_spaceship = 0; // Left Limit
          get_game_background();
          draw_spaceship(x_spaceship);
          draw_all_asteroids();
          draw_all_bullets();
          draw_score(score, X_SCORE_GAME, Y_SCORE_GAME);
          draw_time(sec_time);

        case A_KEY_RELEASED:
          load_buffer();
          break;

        case D_KEY_PRESSED:
          x_spaceship += 30;
          if(x_spaceship > (1152-113)) x_spaceship = (1152-113); // Right Limit
          get_game_background();
          draw_spaceship(x_spaceship);
          draw_all_asteroids();
          draw_all_bullets();
          draw_score(score, X_SCORE_GAME, Y_SCORE_GAME);
          draw_time(sec_time);

        case D_KEY_RELEASED:
          load_buffer();
          break;
      }
    }
    else {
      switch (array[1]) {
        case LEFT_ARROW_PRESSED:
          x_spaceship -= 30;
          if(x_spaceship < 0) x_spaceship = 0; // Left Limit
          get_game_background();
          draw_spaceship(x_spaceship);
          draw_all_asteroids();
          draw_all_bullets();
          draw_score(score, X_SCORE_GAME, Y_SCORE_GAME);
          draw_time(sec_time);

        case LEFT_ARROW_RELEASED:
          load_buffer();
          break;

        case RIGHT_ARROW_PRESSED:
          x_spaceship += 30;
          if(x_spaceship > (1152-113)) x_spaceship = (1152-113); // Right Limit
          get_game_background();
          draw_spaceship(x_spaceship);
          draw_all_asteroids();
          draw_all_bullets();
          draw_score(score, X_SCORE_GAME, Y_SCORE_GAME);
          draw_time(sec_time);

        case RIGHT_ARROW_RELEASED:
          load_buffer();
          break;
      }
    }
    
  }
}


void fire_bullet(){
  for(int i=0; i<50; i++){
    if(bullets[i].active == 0){
      bullets[i].x = x_spaceship + 50;
      bullets[i].y = SPACESHIP_Y_AXIS - 10;
      bullets[i].active = 1;
      break;
    }
  }
}

void move_bullets(){
  for(int i=0; i<50; i++){
    if(bullets[i].active == 1){
      bullets[i].y -= bullet_speed;
      if(bullets[i].y < 0){
        bullets[i].active = 0;
      }
    }
  }
}

void draw_all_bullets(){
  for(int i=0; i<50; i++){
    if(bullets[i].active == 1){
      draw_bullet(bullets[i].x, bullets[i].y);
    }
  }
}


void(create_asteroid)(){
  for(int i=0; i<25; i++){
    if(asteroids[i].active == 0){
      asteroids[i].x = rand() % (X_RES - 139) + 65;
      printf("DEBUG: Asteroid %d: %d\n", i, asteroids[i].x);
      asteroids[i].y = 65;
      asteroids[i].active = 1;
      break;
    }
  }
}

void(move_asteroids)(){
  for(int i=0; i<25; i++){
    if(asteroids[i].active == 1){
      asteroids[i].y += asteroid_speed;
      if(asteroids[i].y > Y_RES){
        asteroids[i].active = 0;
      }
    }
  }
}

void(draw_all_asteroids)(){
  for(int i=0; i<25; i++){
    if(asteroids[i].active == 1){
      draw_asteroid(asteroids[i].x, asteroids[i].y);
    }
  }
}


void(check_collision)(){
  for(int i=0; i<25; i++){
    if(asteroids[i].active == 1){
      for(int j=0; j<50; j++){
        if(bullets[j].active == 1){
          if(bullets[j].x >= asteroids[i].x && bullets[j].x <= asteroids[i].x + 60 &&
             bullets[j].y >= asteroids[i].y && bullets[j].y <= asteroids[i].y + 60){
            bullets[j].active = 0;
            asteroids[i].active = 0;
            score+= 10;
          }
        }
      }
    }
  }
}

void(deactivate_all_bullets)(){
  for(int i=0; i<50; i++){
    bullets[i].active = 0;
  }
}

void(deactivate_all_asteroids)(){
  for(int i=0; i<25; i++){
    asteroids[i].active = 0;
  }
}

int(is_game_over)(){
  if(score >= highest_score) {
    highest_score = score;
  }
  for(int i=0; i<25; i++){
    if(asteroids[i].active == 1 && asteroids[i].y >= SPACESHIP_Y_AXIS){
      deactivate_all_bullets();
      deactivate_all_asteroids();
      x_spaceship = X_RES/2;
      return 1;
    }
  }
  return 0;
}

