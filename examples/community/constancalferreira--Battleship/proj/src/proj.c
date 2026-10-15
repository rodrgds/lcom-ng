#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "controller/timer.h"
#include "controller/keyboard.h"
#include "controller/mouse.h"
#include "controller/graphics.h"

#include "model/game_macro.h"
#include "model/game.h"
#include "model/arena.h"
#include "model/menu.h"
#include "model/start.h"
#include "model/rules.h"
#include "model/help.h"
#include "model/mode.h"
#include "model/sprite.h"
#include "model/game_over.h"




uint8_t kbd_irq_set;
uint8_t timer_irq_set;
uint32_t mouse_irq_set;
uint8_t rtc_irq_set;

extern int cursor_x;
extern int cursor_y;


int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/grupo_2leic03_5/proj/src/debug/trace.txt");
  lcf_log_output("/home/lcom/labs/grupo_2leic03_5/proj/src/debug/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}




int start_devices() {
  if (timer_set_frequency(TIMER, FREQUENCY) != 0) {return 1;}

  if (timer_subscribe_int(&timer_irq_set) != 0) {return 1;}
  if (kbd_subscribe_int(&kbd_irq_set) != 0) {return 1;}
  if (mouse_subscribe_int(&mouse_irq_set) != 0) {return 1;}
  if (rtc_subscribe_int(&rtc_irq_set) != 0) {return 1;}

  if (mouse_set_stream_mode() != 0) {return 1;}
  if (_mouse_enable_data_reporting() != 0) {return 1;}
  if (rtc_enable_update_int() != 0) {return 1;}

  if (set_frame_buffer(VIDEO_MODE) != 0) {return 1;}
  if (set_graphics_mode(VIDEO_MODE) != 0) {return 1;}

  return 0;
}


int close_devices() {
  if (kbd_unsubscribe_int() != 0) {return 1;}
  if (timer_unsubscribe_int() != 0) {return 1;}
  if (mouse_unsubscribe_int() != 0) {return 1;}

  if (_mouse_disable_data_reporting() != 0) {return 1;}
  
  if (vg_exit() != 0) {return 1;}

  return 0;
}


void init_states(GameState cur_state, GameState prev_state) {

  if ((cur_state == MENU && prev_state != MENU)) {
    init_menu();
  } 
  else if ((cur_state == START && prev_state != START && prev_state != HELP && prev_state != RULES)) {
    init_start();
  } 
  else if ((cur_state == ARENA && prev_state != ARENA && prev_state != HELP && prev_state != RULES)) {
    init_arena();
  }
  else if ((cur_state == GAME_OVER && prev_state != GAME_OVER)) {
    init_game_over();
  }
  else if ((cur_state == MODE && prev_state != MODE && prev_state != HELP && prev_state != RULES)) {
    init_mode();
  }
  else if (cur_state == RULES && prev_state != RULES) {
    init_rules();
  } 
  else if (cur_state == HELP && prev_state != HELP) {
    init_help();
  } 
}




int (proj_main_loop)(int argc, char *argv[]) {

  if (start_devices() != 0) {return close_devices();}
  if (load_sprites() != 0) {return 1;} 

  cursor_x = 410;
  cursor_y = 310;

  int ipc_status, r;
  message msg;

  GameState prev_state = -1;


  while (get_state() != EXIT) { 

    GameState cur_state = get_state();

    init_states(cur_state, prev_state);
    prev_state = cur_state;
    
    
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & kbd_irq_set) { 
            (kbc_ih)();
            game_keyboard_handler();

            //depois temos de dividir os handlers por ecrã
            if (state == MENU) menu_keyboard_handler();
            else if (state == START) start_keyboard_handler();
            else if (state == RULES) rules_keyboard_handler();
            else if (state == HELP) help_keyboard_handler();
            else if (state == MODE) mode_keyboard_handler();
            else if (state == ARENA) arena_keyboard_handler();
            else if (state == GAME_OVER) game_over_keyboard_handler();
            //...
          }

          
          if (msg.m_notify.interrupts & mouse_irq_set) { 
            (mouse_ih)();
            if(mouse_sync_bytes()) create_packet();
            game_mouse_handler(); 
            
            //depois temos de dividir os handlers por ecrã
            if (state == ARENA) arena_mouse_handler();
            if (state == START) start_mouse_handler();
            if (state == GAME_OVER) game_over_mouse_handler();
            

            //...
          }

          if (msg.m_notify.interrupts & timer_irq_set) { 
            init_states(get_state(), prev_state);
            prev_state = get_state();
            if (state == MENU) menu_main_loop();
            if (state == START) start_main_loop();
            if (state == RULES) rules_main_loop();
            if (state == HELP) help_main_loop();
            if (state == MODE) mode_main_loop();
            if (state == ARENA) arena_main_loop();
            if (state == GAME_OVER) game_over_main_loop();
          }

          if (msg.m_notify.interrupts & BIT(rtc_irq_set)) {
            rtc_ih(); 
            if (state == MENU) {
              draw_menu_background(menu_buffer);
            }
          }



          break;

        default:
          break; /* no other notifications expected: do nothing */	
      }
    }  
  }


  free_buffers();
  free_sprites();
  if (close_devices() != 0) {return 1;}

  return 0;
}
