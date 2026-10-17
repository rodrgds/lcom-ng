#include <lcom/lcf.h>
#include <string.h>

#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/timer/timer.h"
#include "controller/video/graphics.h"

#include "controller/controller.h"
#include "controller/rtc/rtc.h"
#include "model/model.h"
#include "view/view.h"

/** @defgroup main main
 * @{
 * Contains the main(), proj_main_loop() funtions.
 */

extern uint8_t mouse_bytes[3];
extern uint8_t byte_index;
extern uint8_t scancode;
extern uint32_t drawingColor;
int prevX, prevY;
int leftButtonPressed = 0;
extern MouseInfo mouse_info;
extern ToolState tool;
extern unsigned timer_counter;
extern SystemState is_playing;
extern int inst_page;

/**
 * @brief Main function
 * 
 */

int(main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/doc/trace.txt");
  lcf_log_output("/home/lcom/labs/doc/output.txt");
  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();

  return 0;
}

/**
 * @brief Function which contains the driver receive loop and keeps the program running
 * 
 */

int(proj_main_loop)(int argc, char *argv[]) {
  is_playing = RUNNING;

  if (set_frame_buffers(VBE_600p_DC)) {
    return 1;
  }
  if (set_graphic_mode(VBE_600p_DC) != 0) {
    return 1;
  }
  setupsprites();
  draw_frame();
  message msg;
  int ipc_status;
  uint8_t bit_mouse;
  uint8_t bit_keyboard;
  uint8_t bit_timer;
  bool lbwaspressed = false;
  bool draw = false;
  int first_x, first_y;
  // uint8_t bit_rtc;
  if (mouse_write(0xEA) != 0) {
    return 1;
  }
  if (mouse_write(0xF4) != 0) {
    return 1;
  }

  if (timer_subscribe_int(&bit_timer)) {
    return 1;
  }
  if (keyboard_subscribe_interrupts(&bit_keyboard)) {
    return 1;
  }

  if (mouse_subscribe_int(&bit_mouse)) {
    return 1;
  }

  cleanThirdBuffer();
  cleanBackupBuffer();
  while (is_playing == RUNNING) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }
    if (is_ipc_notify(ipc_status) != 0) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & bit_timer) {
            draw_frame();
            if (timer_counter % 60 == 0) {
              rtc_update_time();
              timer_counter = 0;
            }
            cleanSecondaryBuffer();
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & bit_keyboard) {
            kbc_ih();
            if (scancode == 0x81) { // solta tecla ESC
              if (game_state == START) {
                is_playing = EXIT;
              }
              if (game_state == GAME) {
                game_state = START;
                // cleanThirdBuffer();
                draw_frame();
              }
              if (game_state == INSTRUCTIONS) {
                game_state = START;
                draw_frame();
              }
            }
            if (scancode == 0x22) { // solta tecla "G"
              if (game_state == START) {
                // cleanThirdBuffer();
                game_state = GAME;
                draw_frame();
              }
            }
            if (scancode == 0x23) {
              if (game_state == START) {
                game_state = INSTRUCTIONS;
                draw_frame();
              }
            }

            if (game_state == GAME) {
              draw_controls(scancode);
            }
          }

          if (msg.m_notify.interrupts & bit_mouse) {
            mouse_ih();
            mouse_sync_bytes();
            if (byte_index == 3) {
              byte_index = 0;
              mouse_bytes_to_packet();
              leftButtonPressed = mouse_info.left_click;
              int x = getX();
              int y = getY();

              if (game_state == START) {
                if (quit_button_pressed()) {

                  is_playing = EXIT;
                }

                if (draw_button_pressed()) {

                  game_state = GAME;
                  tool = PENCIL;
                  // cleanThirdBuffer();
                  draw_frame();
                }

                if (help_button_pressed()) {
                  game_state = INSTRUCTIONS;
                  draw_frame();
                }
              }

              if (game_state == INSTRUCTIONS) {
                if (arrow_button_pressed()) {
                  if (inst_page == 1) {
                    inst_page = 2;
                  }
                  else if (inst_page == 2)
                    inst_page = 1;

                  draw_frame();
                }
              }

              if (game_state == GAME) {

                pressed_color();

                if (tool == BUCKET && mouse_info.left_click) {
                  invert_backup_draw();
                  paint_bucket();
                }
                if (mouse_info.left_click) {
                  if (tool == PENCIL) {

                    if (processDrawing(x, y, prevX, prevY, drawingColor)) {
                      draw = true;
                    }
                  }
                }
                if (leftButtonPressed && tool == RECTANGLE && lbwaspressed == false) {
                  lbwaspressed = true;
                  first_x = x;
                  first_y = y;
                }
                if (lbwaspressed && !leftButtonPressed && tool == RECTANGLE) {
                  lbwaspressed = false;
                  invert_backup_draw();
                  processRectangle(first_x, first_y, x, y, drawingColor);
                }
                if (!mouse_info.left_click && draw) {
                  draw = false;
                  invert_backup_draw();
                }
              }
              prevX = x;
              prevY = y;

            }
          }

        default:
          break;
      }
    }
  }
  destroy_sprites();
  if (set_text_mode()) {
    return 1;
  }
  if (timer_unsubscribe_int()) {
    return 1;
  }
  if (keyboard_unsubscribe_interrupts()) {
    return 1;
  }

  if (mouse_unsubscribe_int()) {
    return 1;
  }

  if (mouse_write(DISABLE_DATA_REPORTING) != 0) {
    return 1;
  }

  return 0;
}
/**@}*/


