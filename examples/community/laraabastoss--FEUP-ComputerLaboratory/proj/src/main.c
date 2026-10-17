#include <lcom/lcf.h>
#include "devices/timer/timer.h"
#include "devices/video/graphics.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/rtc/rtc.h"
#include "devices/sp/serial_port.h"
#include "handle_int.h"
#include "game/elements.h"
#include "game/draw.h"
#include "game/game.h"

extern int level;

int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

  if (timer_set_frequency(TIMER, 60)) return 1;

  if (timer_subscribe_interrupts()) return 1;
  if (keyboard_subscribe_interrupts()) return 1;
  if (mouse_subscribe_interrupts()) return 1;

  if (set_frame_buffers(0x115)) return 1;
  if (switch_to_graphic_mode(0x115)) return 1;

  if (rtc_subscribe_interrupts()) return 1;
  if (sp_subscribe_interrupts()) return 1;

  if (activate_mouse_stream()) return 1;
  if (activate_mouse_datareport()) return 1;

  load_sprites();
  new_frame();
  alocateLevelArrayMemory();
  updateArrayWithLevel(level);

  fill_rtc_structure();

  //MAIN LOOP
  game_loop();

  destroy_sprites();
  freeLevelArray();

  if (timer_unsubscribe_interrupts()) return 1;
  if (rtc_unsubscribe_interrupts()) return 1;
  if (keyboard_unsubscribe_interrupts()) return 1;
  if (mouse_unsubscribe_interrupts()) return 1;
  if (sp_unsubscribe_interrupts()) return 1;

  if (deactivate_mouse()) return 1;

  vg_exit();

  return 0;
}
