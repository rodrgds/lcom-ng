#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "devices/graphics.h"
#include "game.h"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char* argv[]){
  extern uint8_t bit_timer;
  extern uint8_t bit_kb;
  extern uint8_t bit_m;

  if(video_set_graphics(0x14C) != 0) {
    printf("Error mapping mem");
    return 1;
  }

  if (mouse_enable_data_reporting() != 0) {
    printf("Error enabling mouse data");
    return 1;
  }

  if(timer_subscribe_int(&bit_timer) != 0) {
    return 1;
  }

  if(kb_subscribe(&bit_kb) != 0) {
    return 1;
  }

  if(mouse_subscribe_int(&bit_m) != 0) {
    return 1;
  }

  gameLoop();


  if(timer_unsubscribe_int() != 0) {
    return 1;
  }

  if(kb_unsubscribe() != 0) {
    return 1;
  }

  if(mouse_unsubscribe_int() != 0) {
    return 1;
  }

  exit_graphics();

  if (mouse_disable_data_reporting() != 0) {
    printf("Error Disabling Data");
    return 1;
  }

  return 0;
}
