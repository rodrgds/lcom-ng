// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "i8254.h"
#include "KBC.c"
#include "mouse.h"
#include "timer.c"

extern uint8_t mouse_byte_index;
extern struct packet mouse_packet;
extern int time_counter;

static int16_t draw_x = 0, draw_y = 0;

enum mouse_state {
  MOUSE_INIT,
  MOUSE_DRAW_UP,
  MOUSE_DRAW_DOWN,
  MOUSE_VERTEX,
  MOUSE_END
};

enum mouse_state state = MOUSE_INIT;

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


int (mouse_test_packet)(uint32_t cnt) {
    int ipc_status;
    message msg;

    uint8_t mouse_bit_no;
    if(mouse_issue_cmd(MOUSE_ENABLE_DATA_REPORTING) != 0) return 1;
    if(mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
    
    while(cnt > 0) { /* You may want to use a different condition */
      /* Get a request message. */
       if ( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
           printf("driver_receive failed with: %d");
           continue;
       }
       if (is_ipc_notify(ipc_status)) { /* received notification */
           switch (_ENDPOINT_P(msg.m_source)) {
               case HARDWARE: 	
                 if (msg.m_notify.interrupts & mouse_bit_no) { 
                    mouse_ih();
                    mouse_place_byte();

                    if (mouse_byte_index == 3){
                      mouse_create_packet();
                      mouse_print_packet(&mouse_packet);
                      mouse_byte_index = 0;
                      cnt--;
                    }
                    
                 }
                 break;
               default:
                 break; /* no other notifications expected: do nothing */
           }
       } else { 
           // No action needed
       }
     }

    if(mouse_unsubscribe_int() != 0) return 1;
    if(mouse_issue_cmd(MOUSE_DISABLE_DATA_REPORTING) != 0) return 1;

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status;
  message msg;

  uint8_t mouse_bit_no = 0, timer_bit_no = 0;
  uint8_t timeElapsed = 0;

  if(mouse_issue_cmd(MOUSE_ENABLE_DATA_REPORTING) != 0) return 1;

  if(mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
  if(timer_subscribe_int(&timer_bit_no) != 0) return 1;
  
  while(timeElapsed < idle_time){ /* You may want to use a different condition */
    /* Get a request message. */
     if ( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d");
         continue;
     }
     if (is_ipc_notify(ipc_status)) { /* received notification */
         switch (_ENDPOINT_P(msg.m_source)) {
             case HARDWARE: 

                if (msg.m_notify.interrupts & timer_bit_no) { /* subscribed interrupt */
                  timer_int_handler();
                  if (time_counter % sys_hz() == 0) timeElapsed++; 
                }

               if (msg.m_notify.interrupts & mouse_bit_no) { 
                  mouse_ih();
                  mouse_place_byte();

                  if (mouse_byte_index == 3){
                    mouse_create_packet();
                    mouse_print_packet(&mouse_packet);
                    mouse_byte_index = 0;
                  }
                  
                  time_counter = 0; /* reset timer counter */
                  timeElapsed = 0; /* reset timeElapsed */
               }
               break;
             default:
               break; /* no other notifications expected: do nothing */
         }
     } else { 
         // No action needed
     }
   }

  if (timer_unsubscribe_int() != 0) return 1;

  if(mouse_unsubscribe_int() != 0) return 1;
  if(mouse_issue_cmd(MOUSE_DISABLE_DATA_REPORTING) != 0) return 1;

  return 0;
}

void (mouse_state_machine)(uint8_t tolerance, uint8_t x_len) {
/*   
  printf("State machine: %d\n", state);
  printf("X: %d, Y: %d\n", draw_x, draw_y);
  printf("Dx: %d, Dy: %d\n", mouse_packet.delta_x, mouse_packet.delta_y);
  printf("LB: %d, RB: %d", mouse_packet.lb, mouse_packet.rb);
  printf("\n"); */

  int16_t dx = mouse_packet.delta_x;
  int16_t dy = mouse_packet.delta_y;

  draw_x += dx;
  draw_y += dy;

  switch (state) {
    case MOUSE_INIT:
      draw_x = 0; draw_y = 0;
      if (mouse_packet.lb == 1 && mouse_packet.rb == 0 && mouse_packet.mb == 0) {
        state = MOUSE_DRAW_UP;
      }
      break;

    case MOUSE_DRAW_UP:
      if (mouse_packet.lb == 0 && mouse_packet.rb == 0 && mouse_packet.mb == 0) {
        if (draw_x >= x_len && draw_y > 0 && abs(dx) < tolerance && abs(dy) < tolerance) {
          state = MOUSE_VERTEX;
          draw_x = 0;
          draw_y = 0;
        }
        else {
          state = MOUSE_INIT;
        }
      }
      else if (mouse_packet.lb == 1 && mouse_packet.rb == 0 && mouse_packet.mb == 0) {
        if (abs(dx) > tolerance || abs(dy) > tolerance) {
          state = MOUSE_INIT;
        }
      }
      else state = MOUSE_INIT;

      break;

    case MOUSE_DRAW_DOWN:
      if (mouse_packet.lb == 0 && mouse_packet.rb == 0 && mouse_packet.mb == 0) {
        if (draw_x >= x_len && draw_y < 0 && abs(dx) < tolerance && abs(dy) < tolerance) {
          state = MOUSE_END;
          printf("End of gesture\n");
          draw_x = 0;
          draw_y = 0;
        }
        else {
          state = MOUSE_INIT;
        }
      }
      else if (mouse_packet.lb == 0 && mouse_packet.rb == 1 && mouse_packet.mb == 0) {
        if (abs(dx) > tolerance || abs(dy) > tolerance) {
          state = MOUSE_INIT;
        }
      }
      else state = MOUSE_INIT;

      break;
    
    case MOUSE_VERTEX:
      if (mouse_packet.lb == 0 && mouse_packet.rb == 1 && mouse_packet.mb == 0) {
        state = MOUSE_DRAW_DOWN;
      }
      break;
    case MOUSE_END:
      break;
  }
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  message msg;

  uint8_t mouse_bit_no;
  if(mouse_issue_cmd(MOUSE_ENABLE_DATA_REPORTING) != 0) return 1;
  if(mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
  
  do { /* You may want to use a different condition */
    /* Get a request message. */
     if ( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d");
         continue;
     }
     if (is_ipc_notify(ipc_status)) { /* received notification */
         switch (_ENDPOINT_P(msg.m_source)) {
             case HARDWARE: 	
               if (msg.m_notify.interrupts & mouse_bit_no) { 
                  mouse_ih();
                  mouse_place_byte();

                  if (mouse_byte_index == 3){
                    mouse_create_packet();
                    mouse_print_packet(&mouse_packet);
                    mouse_state_machine(tolerance, x_len);
                    mouse_byte_index = 0;
                  }
                  
               }
               break;
             default:
               break; /* no other notifications expected: do nothing */
         }
     } else { 
         // No action needed
     }
   } while (state != MOUSE_END);

  if(mouse_unsubscribe_int() != 0) return 1;
  if(mouse_issue_cmd(MOUSE_DISABLE_DATA_REPORTING) != 0) return 1;

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
