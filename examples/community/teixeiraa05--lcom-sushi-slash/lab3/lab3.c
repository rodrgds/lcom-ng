#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "kbc.h"
#include "i8042.h"

#include <stdbool.h>
#include <stdint.h>

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
  if(kbc_subscribe_int(&bit_no)) return 1;

  uint32_t irq_set = BIT(bit_no);

  int r;
  int ipc_status;
  message msg;

  //Variables to read scancodes
  bool is_two_byte = false;
  uint8_t scancode[2] = {0};
  int scancode_size = 0; //for the print_scancode function

  while(scancode[0] != ESC_BREAK_CODE || is_two_byte) { //exit on ESC break code, but if it's a two byte scancode, we need to read both bytes before exiting

      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */
                  if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                      kbc_ih();

                      /* If there was an error, skip the scancode processing and wait for the next interrupt */
                      if(kbc_has_error()) {
                        continue;
                      }

                      /* Process the scancode */
                      if(is_two_byte) {
                        scancode[1] = kbc_get_scancode();
                        scancode_size = 2;
                        is_two_byte = false; //reset for the next scancode
                      }
                      else {
                        scancode[0] = kbc_get_scancode();
                        if (scancode[0] == 0xE0) { //first byte of a two-byte scancode
                          is_two_byte = true;
                          continue; //wait for the next interrupt to read the second byte
                        }
                        scancode_size = 1;
                      }
                      
                      /* Build the scancode and print it */
                      bool make = !(scancode[scancode_size - 1] & BIT(7)); //make code if the most significant bit is 0

                      if (kbd_print_scancode(make, scancode_size, scancode)) return 1;
                  }
                  break;
              default:
                  break; /* no other notifications expected: do nothing */
          }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
  }

  if(kbc_unsubscribe_int()) return 1;
  return 0;
}

int(kbd_test_poll)() {
  
  uint8_t scancode = 0; //initialize to 0 to avoid undefined behavior when comparing with ESC_BREAK_CODE, which is 0x81, and to make sure we enter the loop at least once

  uint8_t full_scancode[2] = {0};
  int size = 0;
  bool make;

  while (scancode != ESC_BREAK_CODE) {

    if(kbc_read_output()) continue;
    scancode = kbc_get_scancode();
    make = true;

    //If it's the firts byte of a two byte code
    if (scancode == 0xE0) {
      full_scancode[0] = scancode;
      size++;
    } 
    //If it's the second byte of a two byte  code (size = 1) or a lonely byte (size = 0) 
    else {
      full_scancode[size] = scancode;
      size++;
      //Check if it's a breakcode or a makecode
      if ((BIT(7) & scancode) == BIT(7)) make = false;
      kbd_print_scancode(make, size, full_scancode);
      size = 0;
    }

  }

  if(kbc_int_enable()) return 1;

  return 0;
}

extern int int_counter;

int(kbd_test_timed_scan)(uint8_t n) {
  /* subscribe keyboard interrupts */
  uint8_t bit_no_k;
  if(kbc_subscribe_int(&bit_no_k)) return 1;

  uint32_t irq_set_k = BIT(bit_no_k);

  /* subscribe timer interrupts */
  uint8_t bit_no_t;
  if(timer_subscribe_int(&bit_no_t)){kbc_unsubscribe_int(); return 1;}

  uint32_t irq_set_t = BIT(bit_no_t);

  int r;
  int ipc_status;
  uint8_t time = n;
  message msg;

  //Variables to read scancodes
  bool is_two_byte = false;
  uint8_t scancode[2] = {0};
  int scancode_size = 0; //for the print_scancode function

  while((scancode[0] != ESC_BREAK_CODE || is_two_byte) && time > 0) { //exit on ESC break code, or when time runs out, but if it's a two byte scancode, we need to read both bytes before exiting

      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */
                  if (msg.m_notify.interrupts & irq_set_k) { /* subscribed interrupt */
                      kbc_ih();

                      /* If there was an error, skip the scancode processing and wait for the next interrupt */
                      if(kbc_has_error()) {
                        continue;
                      }

                      /* Process the scancode */
                      if(is_two_byte) {
                        scancode[1] = kbc_get_scancode();
                        scancode_size = 2;
                        is_two_byte = false; //reset for the next scancode
                      }
                      else {
                        scancode[0] = kbc_get_scancode();
                        if (scancode[0] == 0xE0) { //first byte of a two-byte scancode
                          is_two_byte = true;
                          continue; //wait for the next interrupt to read the second byte
                        }
                        scancode_size = 1;
                      }
                      
                      /* Build the scancode and print it */
                      bool make = !(scancode[scancode_size - 1] & BIT(7)); //make code if the most significant bit is 0

                      if (kbd_print_scancode(make, scancode_size, scancode)) {kbc_unsubscribe_int(); timer_unsubscribe_int(); return 1;};
                      time = n;
                      int_counter = 0; //reset timer
                  }
                  if (msg.m_notify.interrupts & irq_set_t) { /* subscribed interrupt */
                      timer_int_handler(); //increase the counter

                      if(int_counter % 60 == 0) {
                        time--;
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

  kbc_unsubscribe_int();
  if(timer_unsubscribe_int()) return 1;

  return 0;

}
