#include <lcom/lcf.h>
#include "libs/i8042.h"
#include "libs/serial_port.h"
#include "libs/keyboard.h"
#include "libs/mouse.h"
#include "program.h"
#include "mouse_proj.h"
#include "main_menu.h"
#include "leaderboard_menu.h"
#include "pick_name_menu.h"
#include "wait_player_menu.h"
#include "game.h"
#include "pause_menu.h"
#include "wait_winner_menu.h"
#include "over_menu.h"
#include "player.h"
#include "UI_game.h"
#include "enum.h"

enum program_state p_state;
enum online_state o_state;
bool two_player = false;
bool game_already_played = false;
extern bool left_press_mouse;
extern bool back_done;
extern unsigned long long counter;
extern bool wasRead_kb;
extern uint8_t data_kb;

void init_program(){
  init_background();
  init_game();
  p_state = MAIN_MENU;
  o_state = OFFLINE;
  init_pause_menu();
  init_over_menu();
  init_pick_name_menu();
  init_main_menu();
  init_leaderboard_menu();
  init_wait_player_menu();
  init_wait_winner_menu();
}

void destroy_program(){
  destroy_game();
  destroy_pause_menu();
  destroy_over_menu();
  destroy_pick_name_menu();
  destroy_main_menu();
  destroy_leaderboard_menu();
  destroy_wait_player_menu();
  destroy_wait_winner_menu();
}

void handle_timer_interrupt_proj(){
  if (p_state == MAIN_MENU){
    update_main_menu();
  } else if (p_state == LEADERBOARDS){
    update_leaderboard_menu();
  } else if (p_state == PICK_USER_NAME){
    update_pick_name_menu();
  } else if (p_state == WAIT_PLAYER_TWO){
    update_wait_player_menu();
  } else if (p_state == PLAYING){
    update_game();
  } else if (p_state == PAUSE_GAME){
    update_pause_menu();
  } else if (p_state == WAIT_WINNER_TWO){
    update_wait_winner_menu();
  } else if (p_state == END_GAME){
    update_over_menu();
  }
  if (counter % 60 == 0){
    if (two_player && p_state == PLAYING)
      uart_send_message("s3S", 3);
    else if (two_player && p_state == WAIT_PLAYER_TWO)
      uart_send_message("s2S", 3);
    else
      uart_send_message("s1S", 3);
  }

  left_press_mouse = false; // Needed in order that one mouse click doesnt activate multiple buttons in different menus
}

void handle_keyboard_interrupt_proj(){
  if (wasRead_kb && data_kb != CHECK_TWO_BYTE) {
    if (p_state == PICK_USER_NAME){
        rec_input_user(data_kb);
    } else if (p_state == PLAYING){
        if (data_kb == PAUSE_KEY){
          if (!two_player){
            p_state = PAUSE_GAME;
            back_done = false;
            pause_game();
            init_pause_menu();
          }
        } else if (data_kb == FIRE_KEY) {
          fire_player();
        } else if (data_kb == KEY_1 || data_kb == KEY_2 || data_kb == KEY_3){
          change_weapon(data_kb);
        } else if (data_kb == RELOAD_KEY){
          reload_player();
        } else if (data_kb == UP_ARROW_MAKE || data_kb == UP_ARROW_BREAK ||
                 data_kb == DOWN_ARROW_MAKE || data_kb == DOWN_ARROW_BREAK ||
                 data_kb == LEFT_ARROW_MAKE || data_kb == LEFT_ARROW_BREAK ||
                 data_kb == RIGHT_ARROW_MAKE || data_kb == RIGHT_ARROW_BREAK)
          change_player_state(data_kb);
    } else if (p_state == PAUSE_GAME){
      if (data_kb == PAUSE_KEY){
        p_state = PLAYING;
        destroy_pause_menu();
        resume_game();
      }
    }
  }
}

void handle_mouse_interrupt_proj(struct packet *mouse_pak){
  if (interpreter_packet(mouse_pak)){
    update_mouse(*mouse_pak);
  }
}
