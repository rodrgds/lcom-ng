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
#include "edit_text.h"
#include "pick_name_menu.h"
#include "wait_player_menu.h"
#include "main_menu.h"

Bitmap *title_pick_name;

struct Button but_cancel;
struct Button but_submit;
struct EditText edit_text;

extern enum program_state p_state;
extern bool two_player;
extern char player_name[50];
extern int player_name_size;

void func_button_cancel(){
  p_state = MAIN_MENU;
  edit_text.t_size = 0;
  destroy_pick_name_menu();
  init_main_menu();
}

void func_button_submit(){
  if (edit_text.t_size == 0)
    return;
  destroy_pick_name_menu();
  if (two_player) {
    p_state = WAIT_PLAYER_TWO;
    init_wait_player_menu();
  } else {
    p_state = PLAYING;
    init_game();
  }
  strncpy(player_name, edit_text.text, edit_text.t_size);
  player_name_size = edit_text.t_size;
  edit_text.t_size = 0;
}

void rec_input_user(uint8_t d){
  receive_input(&edit_text, d);
}

void init_pick_name_menu(){
  init_mouse();
  init_button();
  init_edittext();
  title_pick_name = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/title_pick_name.bmp");
  edit_text = create_edittext(h_res/2, v_res/2 - 50, true);
  but_cancel = create_button(h_res/2 - 200, v_res/2 + 250, "Cancel", 6, func_button_cancel);
  but_submit = create_button(h_res/2 + 200, v_res/2 + 250, "Submit", 6, func_button_submit);
}

void destroy_pick_name_menu(){
  destroy_mouse();
  destroy_button();
  destroy_edittext();
  deleteBitmap(title_pick_name);
}

void update_pick_name_menu(){

  draw_background();
  draw_title(title_pick_name, 200);
  draw_button(but_cancel);
  draw_button(but_submit);
  draw_edittext(edit_text);
  draw_mouse();

  update_button(&but_cancel);
  update_button(&but_submit);
  update_edittext(&edit_text);
  
  flip_buff();
}
