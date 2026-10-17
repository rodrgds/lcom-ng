#include <lcom/lcf.h>
#include "devices/timer/timer.h"
#include "devices/video/video.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "model/model.h"
#include "view/view.h"
#include "configuration.h"

extern System sysState;

int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/src/debug/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/src/debug/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int startSession() {
  if (setBuffers(MODE) != 0) return 1;
  if (video_init(MODE) != 0) return 1;

  setupSprites();

  if (timer_set_frequency(0, GAME_FREQUENCY) != 0) return 1;
  if (timer_set_frequency(1, GAME_FREQUENCY) != 0) return 1;
  if (timer_set_frequency(2, GAME_FREQUENCY) != 0) return 1;

  if (timer_subscribe_interrupts() != 0) return 1;
  if (keyboard_subscribe_interrupts() != 0) return 1;
  if (mouse_subscribe_interrupts() != 0) return 1;

  if (mouse_write_command(0xEA) != 0) return 1;
  if (mouse_write_command(0xF4) != 0) return 1;

  return 0;
}

int endSession() {
  destroySprites();

  if (mouse_write_command(0xF5) != 0) return 1;

  if (timer_unsubscribe_interrupts() != 0) return 1;
  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  if (mouse_unsubscribe_interrupts() != 0) return 1;

  if (vg_exit() != 0) return 1;

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

  if (startSession() != 0) return endSession();

  int ipc_status;
  message msg;
  while (sysState == RUNNING) {
    
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & BIT(0)){
            game();
            updateTimerState();
          }    
          if (msg.m_notify.interrupts & BIT(1)){
            updateKeyboardState();
          }
          if (msg.m_notify.interrupts & BIT(2)){
            updateMouseState();
          }
        }
    }
  }
  
  if (endSession() != 0) return 1;

  return 0;
}
