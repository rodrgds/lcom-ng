#include <lcom/lcf.h>
#include "game.h"
#include "grid.h"
#include "drivers/video/graphics.h"
#include "drivers/kbc/keyboard.h"
#include "drivers/timer/timer.h"
#include "grid.h"
#include "sprite.h"
#include "levels.h"

extern vbe_mode_info_t mode_info;

int GAME_GRAPHICS_MODE = MODE_115;

int (main)(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/grupo_2leic08_5/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/grupo_2leic08_5/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;

}

int(proj_main_loop)(int argc, char* argv[]) {
  if (map_address_space(GAME_GRAPHICS_MODE) != 0) {
    return 1;
  }

  if (initialize_graphics_mode(GAME_GRAPHICS_MODE) != 0) {
    return 1;
  }

  load_sprites();

  allocate_secondary_buffer();
  clear_secondary_buffer();

  game_loop();

  unload_sprites();

  grid_cleanup();

  free_secondary_buffer();

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}
