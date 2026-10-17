// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/sprites/sprite.h"
#include "handler/mainController.h"

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

int(proj_main_loop)(int argc, char *argv[]) {
  if(_vg_init(0x14C) != 0) {vg_exit(); return 1;}

  load_game_sprites();

  controller();

  if (vg_exit() != 0) return 1;
  
  destroy_game_sprites();
  return 0;
}
