#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>
#include "kbc.h"
#include "keyboard.h"
#include "i8042.h"
#include <lcom/timer.h>
#include "timer_ext.h"

uint32_t (util_get_sys_inb_count)(void);

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();
  return 0;
}

// Prints keyboard scancodes using interrupts.
int (kbd_test_scan)() {
  // Variables used as function parameters.
  uint8_t bytes[2];
  uint8_t size = 0;
  bool is_two_bytes = false;
  bool is_esc_pressed = false; 
  bool make = false;
  
  // Subscribe keyboard interrupts.
  uint8_t bit_no;
  if (kbd_subscribe_int(&bit_no) != 0) return 1;

  // Create the interrupt bitmask.
  uint32_t irq_set = BIT(bit_no);

  int r, ipc_status;
  message msg;

  // Loop until ESC is pressed.
  while (!is_esc_pressed) {
    // Receive a message from the kernel.
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    
    // Check whether the message is a notification.
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // Check whether the notification came from the keyboard.
          if (msg.m_notify.interrupts & irq_set) {

            // Handle the keyboard interrupt.
            kbc_ih(); 
            
            // Check for hardware errors.
            if (kbc_get_error()) continue; 
            
            // Process the scancode and check whether ESC was pressed.
            if (kbc_process_scancode(&is_two_bytes, &size, bytes, &make, &is_esc_pressed)) {
              kbd_print_scancode(make, size, bytes);
            }
          }
          break;
        default: break;
      }
    }
  }

  // Unsubscribe keyboard interrupts.
  if (kbd_unsubscribe_int() != 0) return 1;
  
  // Print the number of sys_inb calls.
  kbd_print_no_sysinb(util_get_sys_inb_count());

  return 0;
}


int(kbd_test_poll)() {
  // Variables used as function parameters.
  uint8_t bytes[2];
  uint8_t size = 0;
  bool is_two_bytes = false;
  bool is_esc_pressed = false; 
  bool make = false;

  // Loop until ESC is pressed.
  while (!is_esc_pressed) {
    // Read the scancode in polling mode.
    if (kbc_read_scancode_poll() == 0) {
      
      // Process the scancode and check whether ESC was pressed.
      if (kbc_process_scancode(&is_two_bytes, &size, bytes, &make, &is_esc_pressed)) {
        kbd_print_scancode(make, size, bytes);
      }
    }
  }

  // Restore the keyboard.
  if (kbc_restore_keyboard() != 0) return 1;

  // Print the number of sys_inb calls.
  kbd_print_no_sysinb(util_get_sys_inb_count());

  return 0;
}


int(kbd_test_timed_scan)(uint8_t n) {
  // Variables used as function parameters.
  uint8_t bytes[2];
  uint8_t size = 0;
  bool is_two_bytes = false;
  bool is_esc_pressed = false; 
  bool make = false;
  
  // Subscribe keyboard interrupts.
  uint8_t kbd_bit_no;
  if (kbd_subscribe_int(&kbd_bit_no) != 0) return 1;
  uint32_t kbd_irq_set = BIT(kbd_bit_no);

  // Subscribe timer interrupts.
  uint8_t timer_bit_no; 
  if (timer_subscribe_int(&timer_bit_no) != 0) return 1;
  uint32_t timer_irq_set = BIT(timer_bit_no);

  int r, ipc_status;
  message msg;
  
  // Reset the timer counter.
  timer_reset_counter();
  int max_ticks = n * 60; 

  // Loop until ESC is pressed or the timeout expires.
  while (!is_esc_pressed && (timer_get_counter() < max_ticks)) {
    // Receive a message from the kernel.
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    
    // Check whether the message is a notification.
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          
          // Check whether the notification came from the timer.
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
          }

          // Check whether the notification came from the keyboard.
          if (msg.m_notify.interrupts & kbd_irq_set) {
              
            // Handle the keyboard interrupt.
            kbc_ih(); 
          
            // Reset the timer counter.
            timer_reset_counter();

            // Check for hardware errors.
            if (kbc_get_error()) continue; 

            // Process the scancode and check whether ESC was pressed.
            if (kbc_process_scancode(&is_two_bytes, &size, bytes, &make, &is_esc_pressed)) {
              kbd_print_scancode(make, size, bytes);
            }
          }
          break;
        default: break;
      }
    }
  }

  // Unsubscribe keyboard interrupts.
  if (kbd_unsubscribe_int() != 0) return 1;
  // Unsubscribe timer interrupts.
  if (timer_unsubscribe_int() != 0) return 1;
  
  // Print the number of sys_inb calls.
  kbd_print_no_sysinb(util_get_sys_inb_count());

  return 0;
}
