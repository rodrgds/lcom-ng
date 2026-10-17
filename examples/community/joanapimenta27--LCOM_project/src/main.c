#include <lcom/lcf.h>

#include "visual/visual_menu.h"
#include "visual/visual.h"

#include "controller/kbc/keyboard.h"
#include "controller/kbc/i8042.h"

#include "controller/timer/i8254.h"

#include "controller/video/VBE.h"
#include "controller/video/graphics.h"

#include "controller/rtc/rtc.h"

#include "game/game_utils.h"
#include "game/game_play.h"
#include "game/scores.h"

#include "sprites/sprites.h"

#include "states/state_menu.h"
#include "states/state_game.h"
#include "states/state_dead.h"
#include "states/state_win.h"

#include "visual/visual.h"
#include "visual/visual_menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


// extern varaible from keyboard
extern uint8_t scancd;

// extern varaible from mouse
extern uint8_t which_byte;

int selected = 0; // selected option in a list of options
int scroller = 0; //scroll mouse

rtc_info time_info;
unsigned int timer_counter;
unsigned int countdown;

unsigned int points;

sys_state_e sys_state;
game_state_e game_state;
level_state_e level_state;

uint32_t* game_color_array;
uint32_t* background_color_array;
uint32_t* game_color_array_backup;
uint32_t* background_color_array_backup;

int ipc_status;
message msg;
uint8_t irq_timer = TIMER0_IRQ;
uint8_t irq_keyboard = IRQ_KEYBOARD;
uint8_t irq_mouse = IRQ_MOUSE;
uint8_t irq_rtc = IRQ_RTC;

/**
 * @brief Main function of the program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return 0 on success, 1 on failure.
 */
int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g2/proj/src/logs/trace.txt");
  lcf_log_output("/home/lcom/labs/g2/proj/src/logs/output.txt");

  if (lcf_start(argc, argv)) {
    return 1;
  }

  lcf_cleanup();
  return 0;
}

/**
 * @brief Setup function that initializes various components of the game.
 *
 * @return 0 on success, 1 on failure.
 */
int (setup)(){
  if(set_graphics(VBE_600p_DC)) return 1;
  if(set_frame_buffer(VBE_600p_DC)) return 1;

  if(mouse_config(0xEA)) return 1;
  if(mouse_config(0xF4)) return 1;

  if(timer_set_frequency(TIMER0_IRQ, GAME_FREQUENCY)) return 1;

  if(timer_subscribe_int(&irq_timer)) return 1;
  if(keyboard_subscribe(&irq_keyboard)) return 1;
  if(mouse_subscribe(&irq_mouse)) return 1;
  if(rtc_subscribe(&irq_rtc)) return 1;

  rtc_set();

  game_color_array = game_create_color_array();
  background_color_array = game_create_color_array();
  game_color_array_backup = game_create_color_array();
  background_color_array_backup = game_create_color_array();
 
  if(game_color_array == NULL) return 1;

  if(prepare_sprites()) return 1;

  load_scores("/home/lcom/labs/g2/proj/src/logs/scoreboard.txt");
  
  return 0;
}

/**
 * @brief Cleanup function that frees resources and performs necessary cleanup operations.
 *
 * @return 0 on success, 1 on failure.
 */
int (destroy)(){
  delete_sprites();
  save_scores("/home/lcom/labs/g2/proj/src/logs/scoreboard.txt");

  game_delete_color_array(game_color_array);
  game_delete_color_array(background_color_array);

  if(timer_unsubscribe_int()) return 1;
  if(keyboard_unsubscribe()) return 1;
  if(mouse_unsubscribe()) return 1;
  if(rtc_unsubscribe()) return 1;

  if (vg_exit()) return 1;
  
  return 0;
}

/**
 * @brief Function that handles interrupts and processes input from various devices.
 *
 * @return 0 on success, 1 on failure.
 */
int interrupt_processor(){

  while (sys_state == GAME_RUNNING){
    if(driver_receive(ANY, &msg, &ipc_status) != 0 ){
        printf("Error");
        continue;
    }
    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:

          //timer interrupts
          if (msg.m_notify.interrupts & irq_timer) {
            switch (game_state){
              case MENU:
                handle_mouse_in_menu();
                break;
              case PRE_LEVEL:
                handle_mouse_in_pre_level();
                break;

              case PAUSED:
                handle_mouse_in_paused();
                break;

              case DEAD:
                handle_mouse_in_dead();
                break;
              
              case SCOREBOARD:
                handle_mouse_in_scoreboard();
                draw_scoreboard_scroller(game_color_array, scroller);
                break;

              case WIN:
                handle_mouse_in_win();
                break;             

              case LEVEL:
                handle_countdown();
                if(target_spawner()) return 1;
                handle_mouse_in_level();
              default:
                break;
            }
            print_mouse(game_color_array);
          }
          //keyboard interrupts
          if (msg.m_notify.interrupts & irq_keyboard) {
            kbc_ih();
            switch (game_state){
              case MENU:
                handle_keyboard_in_menu(scancd);
                break;
              case PRE_LEVEL:
                handle_keyboard_in_pre_level(scancd);
                break;
              case LEVEL:
                handle_keyboard_in_level(scancd);
                break;
              case PAUSED:
                handle_keyboard_in_paused(scancd);
                break;
              case DEAD:
                handle_keyboard_in_dead(scancd);
                break;
              case WIN:
                handle_keyboard_in_win(scancd);
              case SCOREBOARD:
                handle_keyboard_in_scoreboard(scancd);
                break;
              default:
                break;
            }
          }


          //mouse interrupts
          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ihandler();
            mouse_sync_bytes();
            if(which_byte == 3){
              which_byte = 0;
              mouse_bytes_to_package();
              update_mouse_location();
            }
          }

          break;
        
        default:
          break;
      }
    }
  }

  return 0;

}

/**
 * @brief Main loop of the program that runs the game.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return 0 on success, 1 on failure.
 */
int (proj_main_loop)(int argc, char *argv[]) {
  if (setup()) return 1;
  if (menu_draw_header(background_color_array)) return 1;
  if (menu_draw_options(selected, game_color_array)) return 1;
  if (interrupt_processor()) return 1;
  if (destroy()) return 1;  
  return 0;

}
