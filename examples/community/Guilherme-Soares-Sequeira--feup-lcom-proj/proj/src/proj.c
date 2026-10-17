/* LCF include */

#include <lcom/lcf.h>

/* device includes */

#include "devices/kbc/kbc.h"
#include "devices/kbc/mouse.h"
#include "devices/rtc/rtc.h"
#include "devices/timer/timer.h"
#include "devices/video/vbe.h"
#include "devices/video/video_gr.h"
#include <lcom/timer.h>
#include <lcom/video_gr.h>

/* utils include */

#include "utils/color.h"
#include "utils/utils.h"

/* other includes */

#include "game/canvas.h"
#include "game/charset.h"
#include "game/cursor.h"
#include "game/draw_clock.h"
#include "game/element.h"
#include "game/text.h"
#include "game/ui.h"
#include "xpm/cursor.xpm"
#include <stdbool.h>

/* global variables */

/* keyboard */

extern uint8_t *scancodes;

#define FPS 60

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

/**
 * @brief Loads the necessary assets
 *
 */
void(load_assets)() {
  load_backbuffer();

  cursor_load();
  canvas_load();
  text_load();

  load_button_xpms();
  load_ie_drawing();
  clock_load();
}

/**
 * @brief Draws the program assets on screen
 *
 */
void(draw_assets)() {
  clear_screen(COLOR_BLUE);

  canvas_draw();
  if (get_is_typing()) {
    buf_draw_text(get_back_buffer(), get_current_text(), get_text_initial_position(), LEFT);
  }
  draw_menu();
  clock_draw();

  cursor_draw();
}

int(proj_main_loop)(int argc, char *argv[]) {
  uint8_t keyboard_bit, mouse_bit, timer_bit, rtc_bit = 0;

  kbc_enable_data_report();

  /* subscribe interrupts */

  if (timer_subscribe_int(&timer_bit) != OK) {
    fprintf(stderr, "There was an error subscribing timer interrupts!\n");
    return EXIT_FAILURE;
  }

  if (rtc_subscribe_int(&rtc_bit) != OK) {
    fprintf(stderr, "There was an error subscribing RTC interrupts!\n");
    return EXIT_FAILURE;
  }

  if (kbc_subscribe_int(&keyboard_bit) != OK) {
    fprintf(stderr, "There was an error subscribing keyboard interrupts!\n");
    return EXIT_FAILURE;
  }

  if (mouse_subscribe_int(&mouse_bit) != OK) {
    fprintf(stderr, "There was an error subscribing mouse interrupts!\n");
    return EXIT_FAILURE;
  }

  /* RTC initialization */

  setup_update_interrupts();

  /* initialize graphics card */

  // vg_init(VBE_MODE_1024x768_INDEXED);
  vg_init(VBE_MODE_1024x768_INDEXED);
  // vg_init(VBE_MODE_1280x1024_FULL_COLOR);

  load_assets();

  int ipc_status, r;
  message msg;

  interactive_element *drawing_ies = get_drawing_ies();
  uint8_t num_ies = get_number_of_drawing_ies();

  bool run = true;

  while (run) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(rtc_bit)) {
            rtc_int_handler();
          }

          if (msg.m_notify.interrupts & BIT(timer_bit)) {
            timer_int_handler();

            draw_assets();

            flip();
          }

          /* mouse interrupts */

          if (msg.m_notify.interrupts & BIT(mouse_bit)) {
            mouse_ih();

            if (mouse_packet_ready()) {

              mouse_packet_t mouse_packet = get_mouse_packet();

              cursor_move(mouse_packet.delta_x, mouse_packet.delta_y);

              if (mouse_packet.lb) {
                if (is_hovered(drawing_ies[0])) {
                  drawing_ies[0].mouse_event_handler(drawing_ies[0].color);
                }
                else if (!cursor_lb_was_pressed()) {
                  if (get_is_typing()) {
                    buf_draw_text(get_canvas_buffer(), get_current_text(), get_text_initial_position(), LEFT);
                    set_is_typing(false);
                    clear_current_text();
                  }
                  for (int i = 1; i < num_ies; i++) {
                    if (is_hovered(drawing_ies[i])) {
                      drawing_ies[i].mouse_event_handler(drawing_ies[i].color);
                      break;
                    }
                  }
                }

                if (!cursor_lb_was_pressed())
                  cursor_set_lb(true);
              }
              else {
                cursor_set_lb(false);
              }
            }
          }

          /* keyboard interrupts */

          if (msg.m_notify.interrupts & BIT(keyboard_bit)) {
            kbc_ih();

            if (get_scancode_size() == 0) {
              mark_scancode_processed();
              continue;
            }

            if (keyboard_ready()) {
              if (scancodes[0] == ESC_KEY_BREAK) { // check early for exit
                run = false;
                free(scancodes);
              }

              mark_scancode_processed();

              if (get_scancode_type() == KBC_SCANCODE_BREAK)
                continue;
              if (get_is_typing()) {
                process_scancode(scancodes, get_scancode_size(), get_scancode_type());
              }
            }
          }
      }
    }
  }

  /* unsubscribe interrupts */

  if (timer_unsubscribe_int() != OK) {
    fprintf(stderr, "There was an error unsubscribing timer interrupts!\n");
    return EXIT_FAILURE;
  }

  if (kbc_unsubscribe_int() != OK) {
    fprintf(stderr, "There was an error unsubscribing keyboard interrupts!\n");
    return EXIT_FAILURE;
  }

  if (mouse_unsubscribe_int() != OK) {
    fprintf(stderr, "There was an error unsubscribing mouse interrupts!\n");
    return EXIT_FAILURE;
  }

  rtc_unsubscribe_int();

  /* disable data reporting */

  reset_kbc();

  /* exit graphics mode */

  vg_exit();

  /*
    free_assets()...
  */

  return EXIT_SUCCESS;
}
