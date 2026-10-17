#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"

int (timer_subscribe_int)(uint8_t *bit_no);
int (timer_unsubscribe_int)();
void (timer_int_handler)();

extern uint32_t sys_inb_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t bit_no;
  int ipc_status;
  message msg;
  
  uint8_t bytes[2]; 
  int size = 0;

  if (kbd_subscribe_int(&bit_no) != 0) return 1;
  uint32_t irq_set = BIT(bit_no);

  while (kbd_get_scancode() != ESC_BREAKCODE) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            
            kbc_ih(); 

            if (kbd_has_error()) continue; 

            uint8_t current_scancode = kbd_get_scancode();
            
            bytes[size] = current_scancode;
            size++;

            if (current_scancode == TWO_BYTE_CODE) {
              continue; 
            }

            bool make = !(current_scancode & MAKECODE_BIT);

            kbd_print_scancode(make, size, bytes);
            
            size = 0; 
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbd_unsubscribe_int() != 0) return 1;

  kbd_print_no_sysinb(sys_inb_counter); 

  return 0;
}


int (kbd_test_timed_scan)(uint8_t n) {
  uint8_t kbd_bit, timer_bit;
  int ipc_status;
  message msg;
  uint8_t bytes[2];
  int size = 0;
  
  extern uint32_t timer_counter; 
  timer_counter = 0;

  if (kbd_subscribe_int(&kbd_bit) != 0) return 1;
  if (timer_subscribe_int(&timer_bit) != 0) return 1;

  while (kbd_get_scancode() != ESC_BREAKCODE && (timer_counter / 60) < n) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
    
    if (is_ipc_notify(ipc_status)) {
      if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
        
        if (msg.m_notify.interrupts & BIT(kbd_bit)) {
          kbc_ih();
          uint8_t code = kbd_get_scancode();
          bytes[size++] = code;
          
          if (code != TWO_BYTE_CODE) {
            kbd_print_scancode(!(code & MAKECODE_BIT), size, bytes);
            size = 0;
          }
          timer_counter = 0;
        }

        if (msg.m_notify.interrupts & BIT(timer_bit)) {
          timer_int_handler();
        }
      }
    }
  }

  kbd_unsubscribe_int();
  timer_unsubscribe_int();
  
  return kbd_print_no_sysinb(sys_inb_counter);
}

int (kbd_test_poll)() {
  uint8_t bytes[2];
  int size = 0;
  uint8_t status;

  while (kbd_get_scancode() != ESC_BREAKCODE) {
    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;

    if (status & KBC_OBF) {
      if ((status & (KBC_PAR_ERR | KBC_TO_ERR)) == 0) {
        kbc_ih();
        uint8_t code = kbd_get_scancode();
        bytes[size++] = code;

        if (code == TWO_BYTE_CODE) continue;

        kbd_print_scancode(!(code & MAKECODE_BIT), size, bytes);
        size = 0;
      }
    }
    tickdelay(micros_to_ticks(20000)); 
  }

  kbc_restore_interrupts();
  return kbd_print_no_sysinb(sys_inb_counter);
}
