#include <lcom/lcf.h>

#include "controller/gpu.h"
#include "controller/keyboard.h"
#include "controller/mouse.h"
#include "lcom/timer.h"
#include "model/game.h"

#define SYSTEM_SPEED 5
#define SYSTEM_MODE 0x115

extern GameState game_state;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]

  // COMMENTED BCS IT LAGS THE GAME
  // lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //COMMENTED BCS IT LAGS THE GAME
  // lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))

    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  uint8_t irq_timer, irq_keyboard, irq_mouse;
  // if (timer_set_frequency(0, 60))
  //   return 1;

  if (timer_subscribe_int(&irq_timer))
    return 1;

  if (keyboard_subscribe_int(&irq_keyboard))
    return 1;

  if (mouse_subscribe_int(&irq_mouse))
    return 1;

  if (gpu_map_vram(SYSTEM_MODE))
    return 1;

  if (gpu_set_mode(SYSTEM_MODE))
    return 1;
  
  game_init();

  int ipc_status;
  message msg;
  int r;

  while (game_state == GAME_RUNNING) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_keyboard) {
            printf("keyboard interrupt\n");
            update_keyboard_state();
          }
          if (msg.m_notify.interrupts & irq_timer) {
            update_timer_state();
          }
          if (msg.m_notify.interrupts & irq_mouse) {
            printf("mouse interrupt\n");
            mouse_ih();
          }
          break;
        default:
          break;
      }
    }
  }

  if (timer_unsubscribe_int())
    return 1;

  if (keyboard_unsubscribe_int())
    return 1;
  
  if (mouse_unsubscribe_int())
    return 1;

  if (vg_exit())
    return 1;
  
  return 0;
}
