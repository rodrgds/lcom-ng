#include <lcom/lcf.h>

#include "events/events.h"

#include "devices/timer.h"
#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "devices/vbe.h"
// #include "devices/serial.h"

#include "state/state.h"
#include "graphics/graphics.h"
#include "vector/vector.h"
#include "scheduler/scheduler.h"

#include "game/state.h"
#include "menu/state.h"
// #include "multiplayer/state.h"

int main(int argc, char *argv[]) {
  //sets the language of LCF messages (can be either EN-US or PT-PT)
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

static bool running = true;

void (stop_running)() {
  running = false;
}

int (proj_main_loop)(int argc, char *argv[]) {
  char *wordlist;
  if (argc != 1) {
    printf("lcom_run proj <wordlist>\n");
    return 1;
  }

  wordlist = argv[0];

  srand(time(NULL)); rand();

  event_init();

  vbe_init();
  graphics_init();
  timer_init();
  kbd_init();
  mouse_init(vbe_get_width(), vbe_get_height());
  // serial_init();

  scheduler_init();
  state_init();

  game_init(wordlist);
  menu_init();
  // multiplayer_init();

  event_on(EXIT, stop_running);

  state_set(MENU);

  int ipc_status;
  message msg;

  while (running) {
    if (driver_receive(ANY, &msg, &ipc_status))
      continue;

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          event_dispatch(
            INTERRUPT,
            &msg.m_notify.interrupts,
            sizeof(msg.m_notify.interrupts));

          event_tick();
          break;
        default:
          break;
      }
    }

  }

  // multiplayer_cleanup();
  game_cleanup();

  scheduler_cleanup();

  // serial_cleanup();
  mouse_cleanup();
  kbd_cleanup();
  timer_cleanup();
  graphics_cleanup();
  vbe_cleanup();

  event_cleanup();
  return 0;
}
