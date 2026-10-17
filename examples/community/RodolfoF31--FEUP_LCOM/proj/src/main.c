#include <lcom/lcf.h>

#include "Modules/states.h"
#include <lcom/xpm.h>

#include "view/draw_functions.h"
#include "controller/game_controller.h"
#include "Modules/keyboard.h"
#include "Modules/mouse.h"
#include "Modules/timer.h"
#include "Modules/vg.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {

  uint8_t timer_irq;
  uint8_t kbc_irq;
  uint8_t mouse_irq;

  if (kbc_subscribe_int(&kbc_irq) != OK) {
    printf("Error subscribing to keyboard interrupts\n");
    return ERROR;
  }

  if (timer_subscribe_int(&timer_irq) != OK) {
    printf("Error subscribing to timer interrupts\n");
    return ERROR;
  }

  if (mouse_subscribe_int(&mouse_irq) != OK) {
    printf("Error subscribing to mouse interrupts\n");
    return ERROR;
  }

  if (vg_init(0x105) == NULL) {
    printf("Error initializing graphics mode\n");
    return ERROR;
  }

  
  if(timer_set_frequency(0, 60) != OK){
    printf("Error setting timer frequency\n");
    return ERROR;
  }

  AppState state = STATE_MENU;
  GameConfig config;

  while (state != STATE_EXIT) {
      if (state == STATE_MENU) {
          MenuResult menu_result = menu_state(kbc_irq, timer_irq);
          state = menu_result.next_state;
          config = menu_result.config;
          
      } else if (state == STATE_GAME) {
          game_state(config, kbc_irq, timer_irq);
          state = STATE_MENU; // After game, return to menu
      }
  }

  if (kbc_unsubscribe_int() != OK) {
    printf("Error unsubscribing from keyboard interrupts\n");
    return ERROR;
  }

  if (timer_unsubscribe_int() != OK) {
    printf("Error unsubscribing from timer interrupts\n");
    return ERROR;
  }

  if (mouse_unsubscribe_int() != OK) {
    printf("Error unsubscribing from mouse interrupts\n");
    return ERROR;
  }

  if (vg_exit() != OK) {
    printf("Error exiting graphics mode\n");
    return ERROR;
  }

  if (free_map_memory() != OK) {
    printf("Error freeing map memory\n");
    return ERROR;
  }

  return 0;
}
