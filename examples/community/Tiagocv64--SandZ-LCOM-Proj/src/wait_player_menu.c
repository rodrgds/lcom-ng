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
#include "pick_name_menu.h"
#include "edit_text.h"
#include "wait_player_menu.h"
#include "main_menu.h"

struct Button but_cancel_two;

extern enum program_state p_state;
extern enum online_state o_state;
extern bool game_already_played;
extern struct EditText edit_text;

void func_button_cancel_two(){
  p_state = MAIN_MENU;
  edit_text.t_size = 0;
  destroy_wait_player_menu();
  init_main_menu();
}

void init_wait_player_menu(){
  init_mouse();
  init_button();
  but_cancel_two = create_button(h_res/2, v_res/2 + 250, "Cancel", 6, func_button_cancel_two);
}

void destroy_wait_player_menu(){
  destroy_mouse();
  destroy_button();
}

void draw_player_two_state(){
  if (o_state == OFFLINE){
    print_string("Player 2: OFFLINE", 17, v_res/2, h_res/2, WHITE, LARGE, ALIGN_CENTER);
  } else if (o_state == ONLINE){
    print_string("Player 2: ONLINE", 16, v_res/2, h_res/2, WHITE, LARGE, ALIGN_CENTER);
  } else if ( game_already_played ){
    print_string("Player 2 ending last game", 25, v_res/2, h_res/2, WHITE, LARGE, ALIGN_CENTER);
  }
}

void update_wait_player_menu(){
  if ((o_state == ON_WAIT || o_state == ON_PLAY) && !game_already_played){
    game_already_played = true;
    p_state = PLAYING;
    destroy_wait_player_menu();
    init_game();
  }

  draw_background();
  draw_button(but_cancel_two);
  draw_player_two_state();
  draw_mouse();

  update_button(&but_cancel_two);
  
  flip_buff();
}
