#include <lcom/lcf.h>
#include "libs/video.h"
#include "Bitmap.h"
#include "enum.h"
#include "main_menu.h"
#include "button.h"
#include "edit_text.h"
#include "program.h"
#include "mouse_proj.h"
#include "UI_game.h"
#include "leaderboard_menu.h"
#include "pick_name_menu.h"

Bitmap *title_main;

struct Button but_play;
struct Button but_play_two;
struct Button but_leaderboard;
struct Button but_exit;

extern struct EditText edit_text;
extern enum program_state p_state;
extern bool two_player;
extern int h_res;
extern int v_res;

void func_button_play(){
  p_state = PICK_USER_NAME;
  two_player = false;
  edit_text.focused = true;
  destroy_main_menu();
  init_pick_name_menu();
}

void func_button_play_two(){
  p_state = PICK_USER_NAME;
  two_player = true;
  edit_text.focused = true;
  destroy_main_menu();
  init_pick_name_menu();
}

void func_button_leaderboard(){
  p_state = LEADERBOARDS;
  destroy_main_menu();
  init_leaderboard_menu();
}

void func_button_exit(){
  p_state = QUIT_PROGRAM;
  destroy_main_menu();
}

void init_main_menu(){
  init_mouse();
  init_button();
  title_main = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/title_main.bmp");
  but_play = create_button(h_res/2, v_res/2 - 100, "Play solo", 9, func_button_play);
  but_play_two = create_button(h_res/2, v_res/2 + 50, "Play vs", 7, func_button_play_two);
  but_leaderboard = create_button(h_res/2, v_res/2 + 200, "Leaderboard", 11, func_button_leaderboard);
  but_exit = create_button(h_res/2, v_res/2 + 350, "Exit", 4, func_button_exit);
}

void destroy_main_menu(){
  destroy_mouse();
  destroy_button();
  deleteBitmap(title_main);
}

void update_main_menu(){

  draw_background();
  draw_title(title_main, 200);
  draw_button(but_play);
  draw_button(but_play_two);
  draw_button(but_leaderboard);
  draw_button(but_exit);
  draw_mouse();

  update_button(&but_play);
  update_button(&but_play_two);
  update_button(&but_leaderboard);
  update_button(&but_exit);
  
  flip_buff();
}
