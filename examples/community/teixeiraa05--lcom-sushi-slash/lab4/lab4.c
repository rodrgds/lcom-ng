// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab4.h>
#include <lcom/timer.h>

#include <stdint.h>
#include <stdio.h>

#include "../lab2/timer.h"
#include "i8042.h"
#include "mouse.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  uint8_t bit_no;
  if (mouse_enable_data_report())
    return 1;
  if (mouse_subscribe_int(&bit_no))
    return 1;

  uint32_t irq_set = BIT(bit_no);
  struct packet *p;

  int r;
  int ipc_status;
  message msg;

  while (cnt > 0) { // exit on ESC break code, but if it's a two byte scancode,
                    // we need to read both bytes before exiting

    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
          mouse_ih();
          if (mouse_has_error())
            continue;

          if (mouse_packet_is_ready()) {
            p = handle_packet();
            mouse_print_packet(p);
            cnt--; // decrease the packet counter
          }
        }

        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
             /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int())
    return 1;
  if (mouse_disable_data_report())
    return 1;
  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  uint8_t mouse_bit_no;
  uint8_t timer_bit_no;

  if (mouse_enable_data_report())
    return 1;
  if (mouse_subscribe_int(&mouse_bit_no))
    return 1;
  if (timer_subscribe_int(&timer_bit_no)) {
    mouse_unsubscribe_int();
    mouse_disable_data_report();
    return 1;
  }

  // Mouse variables
  uint32_t mouse_irq_set = BIT(mouse_bit_no);
  struct packet* p;

  // Timer variables
  uint8_t time = idle_time;
  uint32_t timer_irq_set = BIT(timer_bit_no);
  set_int_counter(0);

  int r;
  int ipc_status;
  message msg;

  while (time > 0) { // exit on idle_time seconds without mouse activity

    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & mouse_irq_set) {
          mouse_ih();
          if (mouse_has_error())
            continue;

          if (mouse_packet_is_ready()) {
            p = handle_packet();
            mouse_print_packet(p);

            // Mouse activity resets the idle-time window.
            time = idle_time;
            set_int_counter(0);
          }
        }

        if (msg.m_notify.interrupts & timer_irq_set) {
          timer_int_handler(); // increase the counter
          if (get_int_counter() % sys_hz() ==
              0) {  // if a second has passed since the last mouse activity
            time--; // decrease the idle time counter
          }
        }

        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
             /* no standard messages expected: do nothing */
    }
  }

  if (timer_unsubscribe_int())
    return 1;
  if (mouse_unsubscribe_int())
    return 1;
  if (mouse_disable_data_report())
    return 1;

  return 0;
}
