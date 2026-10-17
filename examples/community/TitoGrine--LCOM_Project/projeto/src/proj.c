// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
//#include "MACROS.h"
#include "events.h"
//#include "graphics.h"
#include "i8254.h"
//#include "keyboard.h"
#include "mouse.h"
//#include "queue.h"


// Mouse variables
extern bool error_flag_mouse;
extern struct packet packet_data;
extern uint8_t packet_size;

// Keyboard variables
extern bool error_flag_keyboard;
extern uint8_t scancode[];

// Timer variables
extern int g_counter;

// Events variables
extern bool exit_program;
extern bool mouse_events;
extern bool second_passed;
extern bool alarm_rang;

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

int(proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[])) {

  int ipc_status, r;
  message msg;

  uint8_t bit_no_mouse, bit_no_keyboard, bit_no_timer, bit_no_rtc;


  timer_subscribe_int(&bit_no_timer);
  kbd_subscribe_int(&bit_no_keyboard);
  mouse_subscribe_int(&bit_no_mouse);
  rtc_subscribe_int(&bit_no_rtc);

  uint8_t irq_set_timer = 0 | BIT(bit_no_timer);
  uint8_t irq_set_kbd = 0 | BIT(bit_no_keyboard);
  uint8_t irq_set_mouse = 0 | BIT(bit_no_mouse);
  uint8_t irq_set_rtc = 0 | BIT(bit_no_rtc);

  sys_irqdisable(&hook_id_mouse);
  mouse_cmd(ENABLE_DT_RPT);
  sys_irqenable(&hook_id_mouse);
  rtc_enable_alarm();

  vg_init(0x144);
  set_up_main_menu();
  intialize_states();

  while (!(scancode[0] == ESC_BREAK) && !exit_program) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */

      switch (_ENDPOINT_P(msg.m_source)) {

        case HARDWARE: /* hardware interrupt notification */

          if (msg.m_notify.interrupts & irq_set_kbd) {

            kbc_ih();

            update_keyboard_event(scancode);
          }

          if (msg.m_notify.interrupts & irq_set_mouse) {

            mouse_ih();

            if (packet_size == 0) {
              update_mouse_event(&packet_data);
            }
          }

          if (msg.m_notify.interrupts & irq_set_rtc) {

            if(rtc_handler() == 1){
              alarm_rang = true;
            }
          }

          if (msg.m_notify.interrupts & irq_set_timer) {

            timer_int_handler();

            if (g_counter % 60 == 0){ 
              update_rtc();
              second_passed = true;
            }
        
            update_state();
          }

        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else {

      continue; /* received a standard message, not a notification */
                /* no standard messages expected: do nothing */
    }
  }

  destroy_sprites();

  rtc_disable_alarm();
  sys_irqdisable(&hook_id_mouse);
  mouse_cmd(DISABLE_DT_RPT);
  sys_irqenable(&hook_id_mouse);

  mouse_unsubscribe_int(&hook_id_mouse);
  kbd_unsubscribe_int();
  timer_unsubscribe_int();
  rtc_unsubscribe_int();

  vg_exit();

  return 0;
}
