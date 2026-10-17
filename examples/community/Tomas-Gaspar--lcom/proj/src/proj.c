#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "controllers/keyboard_mouse/keyboard.h"
#include "controllers/keyboard_mouse/mouse.h"
#include "controllers/keyboard_mouse/macro_kb_mouse.h"
#include "controllers/rtc/rtc.h"
#include "controllers/rtc/macro_rtc.h"
#include "controllers/serial_port/serial_port.h"
#include "controllers/video/video_gr.h"
#include "controllers/video/macro_video.h"
#include "controllers/utils/utils.h"

#include "game/game.h"

#include "graphics/paint_window.h"

#include "getters.h"
#include "cursor.h"

#include "img/cursor.xpm"

extern int scancode;
extern int ret_val;
extern uint8_t read_byte;
extern bool change_theme;

static uint8_t sc[2] = {0, 0};
static struct packet pp;
static color_theme_t color_theme;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/proj/src/output.txt");

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
  uint8_t timer_bit, kb_bit, mouse_bit, rtc_bit, sp_bit;
  int ipc_status, kb_i = 0, mouse_i = 0, r;
  uint8_t packets[3];

  uint16_t mode = 0x115;
  
  message msg;

  if (timer_subscribe_int(&timer_bit)) return 1;
  if (kb_subscribe_int(&kb_bit)) return 1;
  if (mouse_subscribe_int(&mouse_bit)) return 1;

  if (rtc_subscribe_int(&rtc_bit)) return 1;
  if (rtc_alarm_interrupts(true)) return 1;

  if (sp_subscribe_int(&sp_bit)) return 1;
  if (sp_init()) return 1;

  if (init_buffers(mode)) return 1;
  
  Cursor cursor;

  if (get_next_alarm() == LIGHTMODE_HOURS){
    color_theme = DARKMODE;
    cursor = create_cursor(800, 600, WHITE, cursor_night, 0);
  } else {
    color_theme = LIGHTMODE;
    cursor = create_cursor(800, 600, 0x121212, cursor_day, 0);
  }

  if ((r = paint_main_menu()) != 0)
    return r;
  
  bool exit = false;
  
  while (!exit) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
            if (msg.m_notify.interrupts & BIT(timer_bit)) {
              timer_int_handler();
              swap_buffers();
              switch (cursor.xpmCode) {
              case 0:
                paint_image_cursor(cursor.cursorMap, cursor.x - 4, cursor.y - 2);
                break;
              case 1:
                paint_image_cursor(cursor.cursorMap, cursor.x - 10, cursor.y - 1);
                break;
              case 2:
                paint_image_cursor(cursor.cursorMap, cursor.x - 12, cursor.y - 12);
                break;
              default:
                break;
              }
              game_state_handler(TIMER, &cursor);
            }

            if (msg.m_notify.interrupts & BIT(kb_bit)) {  //keyboard should have precedence over the mouse
                kbc_ih();
                sc[kb_i] = scancode;
                if (kb_i == 0) {
                    if (sc[kb_i] == TWO_BYTE_SC) kb_i = 1;
                    else {
                      if (game_state_handler(KEYBOARD, &cursor) == 'e')
                        exit = true;
                    }
                } else {
                    if (game_state_handler(KEYBOARD, &cursor) == 'e')
                      exit = true;
                    kb_i = 0;
                }
            }
            
            if (msg.m_notify.interrupts & BIT(mouse_bit)) {
              mouse_ih();
              if (ret_val == 1) {
                  ret_val = 0;
                  mouse_i = 0;
                  continue;
              }

              packets[mouse_i] = read_byte;
              switch (mouse_i) {
                case 0:
                  if (!(read_byte & MS_FIRST_BYTE)) {
                    continue;
                  }
                  mouse_i++;
                  break;
                case 1:
                  mouse_i++;
                  break;
                case 2:
                  mouse_i = 0;
                  pp = mouse_parse_packet(packets);

                  if (game_state_handler(MOUSE, &cursor) == 'e')
                    exit = true;
                  
                  break;
                default:
                  break;
              }
            }

            if (msg.m_notify.interrupts & BIT(rtc_bit)) {
              rtc_ih();

              if (change_theme) {
                if (color_theme == LIGHTMODE) color_theme = DARKMODE;
                else color_theme = LIGHTMODE;

                change_theme = false;
                game_state_handler(RTC, &cursor);
              }
            }

            if (msg.m_notify.interrupts & BIT(sp_bit)) {
              sp_ih();

              game_state_handler(SERIAL_PORT, &cursor);
            }

            break;
        default:
            break;	
        }
     }
     game_state_handler(SERIAL_PORT, &cursor);
   };

  if (vg_exit()) return 1;

  if (sp_unsubscribe_int()) return 1;
  if (sp_cleanup()) return 1;

  if (rtc_alarm_interrupts(false)) return 1;
  if (rtc_unsubscribe_int()) return 1;

  if (mouse_unsubscribe_int()) return 1;
  if (kb_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;

  return 0;
}

const uint8_t* get_scancode() {
  return sc;
}

struct packet get_packet(){
  return pp;
}

color_theme_t get_color_theme(){
  return color_theme;
}
