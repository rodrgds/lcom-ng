#include <lcom/lcf.h>

#include "controller/ev/ev.h"
#include "controller/graphics/graphics.h"
#include "controller/graphics/vbe.h"
#include "controller/ih/ih.h"
#include "controller/kbc/i8042.h"
#include "controller/kbc/kbc.h"
#include "controller/kbc/kbd.h"
#include "controller/kbc/mouse.h"
#include "controller/timer/i8254.h"
#include "controller/timer/timer.h"
#include "model/board/board.h"
#include "model/entity/entity.h"
#include "model/game/game.h"
#include "model/resources/resources.h"
#include "model/sprite/sprite.h"
#include "utils/utils.h"
#include "view/view.h"

int(main)(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/proj/src/debug/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/debug/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv) != 0) {
    return 1;
  }

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  Game game;
  int ipc_status, r;
  message msg;

  if (create_resources() != 0) {
    fprintf(stderr, "proj_main_loop: failed to create game resources.");
    return 1;
  }
  if (init_game(&game) != 0) {
    fprintf(stderr, "proj_main_loop: failed to initialize game.");
    return 1;
  }

  if (subscribe_interrupts() != 0) {
    fprintf(stderr, "proj_main_loop: failed to subscribe interrupts.");
    return 1;
  }
  if (vbe_map_vram(VBE_MODE_1024x768_8_8_8) != 0) {
    fprintf(stderr, "proj_main_loop: failed to map graphics vram.");
    return 1;
  }
  if (vbe_set_video_mode(VBE_MODE_1024x768_8_8_8) != 0) {
    fprintf(stderr, "proj_main_loop: failed to set video mode.");
    return 1;
  }
  if (init_mouse(518, 384) != 0) {
    fprintf(stderr, "proj_main_loop: failed to initialize mouse.");
    return 1;
  }

  while (game.state != EXIT) {
    /* get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          process_interrupts(msg.m_notify.interrupts, &game);
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (unsubscribe_interrupts() != 0) {
    fprintf(stderr, "proj_main_loop: failed to unsubscribe interrupts.");
    return 1;
  }
  if (vbe_exit() != 0) {
    fprintf(stderr, "proj_main_loop: failed to reset video card.");
    return 1;
  }

  if (reset_game(&game) != 0) {
    fprintf(stderr, "proj_main_loop: failed to destroy game.");
    return 1;
  }
  destroy_resources();
  cleanup_cache();

  return 0;
}
