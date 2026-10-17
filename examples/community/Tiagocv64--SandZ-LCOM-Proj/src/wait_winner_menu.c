#include <lcom/lcf.h>
#include "libs/video.h"
#include "libs/rtc.h"
#include "Bitmap.h"
#include "enum.h"
#include "over_menu.h"
#include "button.h"
#include "program.h"
#include "mouse_proj.h"
#include "UI_game.h"
#include "game.h"
#include "text.h"
#include "wait_winner_menu.h"

struct Button but_leave;
extern enum program_state p_state;
extern enum online_state o_state;
extern int points_game;
extern int points_adv;

void func_button_leave(){
  p_state = END_GAME;
  destroy_wait_winner_menu();
  init_over_menu();
}

void init_wait_winner_menu(){
  init_mouse();
  init_button();
  but_leave = create_button(h_res/2, v_res/2 + 250, "Leave", 5, func_button_leave);
}

void destroy_wait_winner_menu(){
  destroy_mouse();
  destroy_button();
}

void draw_player_two_scores(){
  char tmp2[25];
  uint32_t size_points2 = sprintf(tmp2, "P2:  %010d", points_adv);
  char tmp1[25];
  uint32_t size_points1 = sprintf(tmp1, "YOU: %010d", points_game);
  if (points_game >= points_adv){
    print_string(tmp1, size_points1, v_res/2 - 100, h_res/2, WHITE, LARGE, ALIGN_CENTER);
    print_string(tmp2, size_points2, v_res/2, h_res/2, WHITE, LARGE, ALIGN_CENTER);
  } else {
    print_string(tmp2, size_points2, v_res/2 - 100, h_res/2, WHITE, LARGE, ALIGN_CENTER);
    print_string(tmp1, size_points1, v_res/2, h_res/2, WHITE, LARGE, ALIGN_CENTER);
  }
}

void update_wait_winner_menu(){

  draw_background();
  if (o_state == ON_PLAY){
    print_string("GAME UNDERWAY", 14, v_res/2 - 300, h_res/2, WHITE, LARGE, ALIGN_CENTER);
  } else {
    print_string("GAME FINISHED", 14, v_res/2 - 300, h_res/2, WHITE, LARGE, ALIGN_CENTER);
  }
  draw_button(but_leave);
  draw_player_two_scores();
  draw_mouse();

  update_button(&but_leave);
  
  flip_buff();
}
