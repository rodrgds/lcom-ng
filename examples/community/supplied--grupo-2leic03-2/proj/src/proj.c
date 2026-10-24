#include <lcom/lcf.h>

#include "./config.h"
#include "./game/game.h"
#include "./graphics/graphics.h"
#include "./devices/keyboard.h"
#include "./devices/mouse.h"
#include "./devices/timer.h"
#include "./devices/rtc.h"
#include "./input/input.h"

#define ESC_MAKE_CODE  0x01
#define ESC_BREAK_CODE 0x81

int(proj_main_loop)(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");

  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  if (lcf_start(argc, argv)) {
    return 1;
  }

  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  (void) argc;
  (void) argv;

  message msg;
  int ipc_status;

  uint32_t timer_irq_set = 0;
  uint32_t keyboard_irq_set = 0;
  uint32_t mouse_irq_set = 0;

  int running = 1;
  int mouse_enabled = 0;

  Game game;
  InputState input;

  if (graphics_init(VIDEO_MODE) != 0) {
    printf("Error: could not initialize graphics mode.\n");
    return 1;
  }

  if (project_timer_subscribe(&timer_irq_set) != 0) {
    printf("Error: could not subscribe timer interrupts.\n");
    graphics_exit();
    return 1;
  }

  /*
   * To prevent Minix's default PS/2 driver from stealing the ACK bytes
   * when we enable data reporting, we must subscribe to the mouse interrupts
   * first, using IRQ_EXCLUSIVE.
   */
  if (project_mouse_subscribe(&mouse_irq_set) == 0) {
    mouse_enabled = 1;
    project_mouse_set_sample_rate(20);
    project_mouse_set_resolution(3);
    
    if (project_mouse_enable_data_reporting() != 0) {
      printf("Warning: could not enable mouse data reporting. Continuing without mouse.\n");
      project_mouse_disable_data_reporting();
      project_mouse_unsubscribe();
      mouse_enabled = 0;
    }
  } else {
    printf("Warning: could not subscribe mouse interrupts.\n");
  }

  if (keyboard_subscribe(&keyboard_irq_set) != 0) {
    printf("Error: could not subscribe keyboard interrupts.\n");
    if (mouse_enabled) {
      project_mouse_disable_data_reporting();
      project_mouse_unsubscribe();
    }
    project_timer_unsubscribe();
    graphics_exit();
    return 1;
  }

  rtc_init();
  game_init(&game);
  input_init(&input);

  while (running && game.state != STATE_EXIT) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & keyboard_irq_set) {
            keyboard_ih();

            uint8_t scancode = keyboard_get_scancode();

            input_handle_keyboard(&input, scancode);
          }

          if (mouse_enabled && (msg.m_notify.interrupts & mouse_irq_set)) {
            project_mouse_ih();

            if (project_mouse_packet_ready()) {
              input_handle_mouse(&input, project_mouse_get_packet());
            }
          }

          if (msg.m_notify.interrupts & timer_irq_set) {
            game_update(&game, &input);
            /*
             * Throttle drawing to 30 FPS (every 2 timer ticks) to prevent starvation.
             * If game_draw is too slow, it blocks driver_receive, which causes mouse
             * bytes to back up in the KBC FIFO, resulting in massive input delay.
             * By drawing less often, we give the loop plenty of free time to rapidly
             * process all pending mouse interrupts.
             */
            static int draw_counter = 0;
            draw_counter++;
            if (draw_counter >= 2) {
              game_draw(&game, &input);
              draw_counter = 0;
            }
            
            input_end_frame(&input);
          }

          break;

        default:
          break;
      }
    }
  }

  keyboard_unsubscribe();

  /*
   * Disable data reporting BEFORE unsubscribing from mouse interrupts.
   * If we unsubscribe first, Minix's default driver might steal the ACK
   * byte from disable_data_reporting.
   */
  if (mouse_enabled) {
    project_mouse_disable_data_reporting();
    project_mouse_unsubscribe();
  }

  project_timer_unsubscribe();
  game_destroy();
  graphics_exit();

  return 0;
}
