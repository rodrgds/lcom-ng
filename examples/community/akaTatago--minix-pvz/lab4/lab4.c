// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"

extern uint8_t mouse_byte;
extern bool mouse_error;

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


int (mouse_test_packet)(uint32_t cnt) {
  uint8_t bit_no;
  uint32_t irq_set;
  int ipc_status;
  message msg;
  
  uint8_t bytes[3];
  int byte_index = 0;
  struct packet pp;

  if (mouse_write_cmd(0xF4) != 0) return 1; // 0xF4 = Enable Data Reporting
  if (mouse_subscribe_int(&bit_no) != 0) return 1;
  irq_set = BIT(bit_no);
  while (cnt > 0) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
    
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
      if (msg.m_notify.interrupts & irq_set) {
        mouse_ih();
        
        if (mouse_error) continue;

        if (byte_index == 0 && (mouse_byte & BIT(3))) {
          bytes[byte_index++] = mouse_byte;
        } else if (byte_index > 0) {
          bytes[byte_index++] = mouse_byte;
        }

        if (byte_index == 3) {
          mouse_parse_packet(&pp, bytes);
          mouse_print_packet(&pp);
          byte_index = 0;
          cnt--;
        }
      }
    }
  }

  if (mouse_unsubscribe_int() != 0) return 1;
  if (mouse_write_cmd(0xF5) != 0) return 1; // 0xF5 = Disable Data Reporting
  return 0;
}

extern uint32_t timer_counter;

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t bit_no;
  uint8_t bit_no_timer;
  uint32_t irq_set;
  int ipc_status;
  message msg;
  
  uint8_t bytes[3];
  int byte_index = 0;
  struct packet pp;

  timer_counter = 0;

  if (timer_subscribe_int(&bit_no_timer) != 0) {
    return 1;
  }

  uint32_t irq_set_timer = BIT(bit_no_timer);

  if (mouse_write_cmd(0xF4) != 0) return 1; // 0xF4 = Enable Data Reporting
  if (mouse_subscribe_int(&bit_no) != 0) return 1;
  irq_set = BIT(bit_no);

  uint32_t freq = sys_hz();

  while (timer_counter/freq < idle_time) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
    
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
      if (msg.m_notify.interrupts & irq_set_timer) { 
        timer_int_handler();
      }
      if (msg.m_notify.interrupts & irq_set) {
        mouse_ih();
        
        if (mouse_error) continue;

        timer_counter = 0;

        if (byte_index == 0 && (mouse_byte & BIT(3))) {
          bytes[byte_index++] = mouse_byte;
        } else if (byte_index > 0) {
          bytes[byte_index++] = mouse_byte;
        }

        if (byte_index == 3) {
          mouse_parse_packet(&pp, bytes);
          mouse_print_packet(&pp);
          byte_index = 0;
        }
      }
    }
  }

  if (timer_unsubscribe_int() != 0) return 1;

  if (mouse_unsubscribe_int() != 0) return 1;
  if (mouse_write_cmd(0xF5) != 0) return 1; // 0xF5 = Disable Data Reporting
  return 0;
}
