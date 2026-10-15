// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics/video.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "graphics/sprites.h"
#include "core/game.h"
#include "info.h"
#include "graphics/sprites.h"

extern uint8_t scan_code;
extern struct packet pp;
extern int bytes_received;

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");

  // Uncomment if needed
  // lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  // lcf_log_output("/home/lcom/labs/proj/output.txt");

  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();
  return 0;
}

int(proj_main_loop)(int argc, char* argv[]) {

  if (vg_map_vram(0x14C) != 0) return 1;
  if (vg_enter(0x14C) != 0) return 1;

  load_menu_background();
  get_menu();
  draw_mouse();
  load_buffer();

  uint8_t keyboard_irq_set;
  uint8_t mouse_irq_set;
  uint8_t timer_irq_set;
  message msg;
  int ipc_stat, r;

  int tam = 1;

  uint8_t array[2];

  if(timer_subscribe_int(&timer_irq_set) != 0) return 1;

  if (mouse_subscribe_int(&mouse_irq_set) != 0) return 1;
  mouse_irq_set = BIT(mouse_irq_set);

  if (keyboard_subscribe_int(&keyboard_irq_set) != 0) return 1;
  keyboard_irq_set = BIT(keyboard_irq_set);

  while (scan_code != ESC_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_stat)) != 0) {
      printf("Driver receive failed: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_stat)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            if (bytes_received == 3) {
              bytes_received = 0;
              process_mouse_packet();  // Handled in game.c
            }
          }

          if (msg.m_notify.interrupts & keyboard_irq_set) {
            kbc_ih();
            keyboard_handler();
            if (scan_code != SC_TWO_BYTE) {
              if (tam == 1) {
                array[0] = scan_code;
                spaceship_move(array, tam);
              }
              else {
                array[0] = SC_TWO_BYTE;
                array[1] = scan_code;
                spaceship_move(array, tam);
                tam = 1;
              }
            }
            else {
              tam = 2;
            }
          }
          
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            timer_handler();
          }

          break;
        default:
          break;
      }
    }
  }

  if(keyboard_unsubscribe_int()!=0) return 1;
  if(mouse_unsubscribe_int()!=0) return 1;
  if(timer_unsubscribe_int()!=0) return 1;
  
  if(vg_exit()!=0) return 1;
  
  return 0;
}
