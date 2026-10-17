// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "game/game.h"
#include "controller/video/vbe.h"


extern StateGame game_st;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");
  /*
  lcf_trace_calls("/home/lcom/proj/debug/trace.txt");
  lcf_log_output("/home/lcom/projdebug/output.txt");
  */
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int (proj_main_loop)() {

    int ipc_status;
    message msg;
    uint8_t irq_set_TIMER, irq_set_KBC, irq_set_MOUSE, irq_set_RTC;

    // Subscrição das interrupções
    if (timer_subscribe_int(&irq_set_TIMER) != 0) return 1;
    if (keyboard_subscribe_int(&irq_set_KBC) != 0) return 1;
    if (mouse_subscribe_int(&irq_set_MOUSE) != 0) return 1;
    rtc_setup();
    if (rtc_subscribe_int(&irq_set_RTC) != 0) return 1;

    // so there's no warnings 
    if (mouse_disable() != 0) return 1;
    if (mouse_write(ENABLE_DATA) != 0) return 1;
    if (mouse_enable() != 0) return 1;

    // Atualiza o sistema para a frame rate dada
    if (timer_set_frequency(0, 60) != 0) return 1;

    // Initialize the graphics mode
    if (graphic_mode(VBE_MODE) != 0) {
        printf("Failed to set graphics mode\n");
        return 1;
    }
    // Initialize the frame buffer
    if (frame_buffer(VBE_MODE) != 0) {
        printf("Failed to initialize frame buffer\n");
        return 1;
    }

    game_init();

    while (game_st != GAME_OVER) {
      if (driver_receive(ANY, &msg, &ipc_status) != 0) {
        printf("Error: driver_receive failed\n");
        continue;
      }
        if (is_ipc_notify(ipc_status)) {
          switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: // Hardware interrupt

              // interrupção do timer
              if (msg.m_notify.interrupts & irq_set_TIMER) {
                update_timer_state();
              }
              // interrupção do keyboard
              if (msg.m_notify.interrupts & irq_set_KBC) {
                update_keyboard_state();
              }
              // interrupção do mouse
              if (msg.m_notify.interrupts & irq_set_MOUSE) {
                update_mouse_state();    
              }
              // interrupção do RTC
              if (msg.m_notify.interrupts & irq_set_RTC) {
                rtc_ih();
              }
              break;
            default:
              break;
          }
        }
      }

    // Cleanup
    snake_free(); // Free the snake body memory
    if (rtc_unsubscribe_int() != 0) printf("rtc_unsubscribe_int() failed\n");
    if (mouse_write(DISABLE_DATA) != 0) printf("Failed to disable mouse data reporting\n");
    if (mouse_disable() != 0) printf("Failed disabling interrupts\n");   
    if (mouse_unsubscribe_int() != 0) printf("mouse_unsubscribe_int() failed\n");
    if (keyboard_unsubscribe_int() != 0) printf("keyboard_unsubscribe_int() failed\n");
    if (timer_unsubscribe_int() != 0) printf("timer_unsubscribe_int() failed\n");
    if (vg_exit() != 0) printf("vg_exit() failed\n");  

    return 0; // Return 0 for successful execution
}
