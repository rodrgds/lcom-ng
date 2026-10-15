// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include "i8042.h"
#include "mouse.h"
#include <stdint.h>
#include <stdio.h>

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


/**  After talking to the lecturer in charge he told to leave the warnings of the 0x60 
 because if i change the order of the functions calls my mouse_enable_interrupts will not work, since minix steals the ACK byte.
 He also told me that the warning is not a problem just a reminder for the group project the lcf mouse_enable will be used
**/
int (mouse_test_packet)(uint32_t cnt) {
    int r, ipc_status;
    message msg;
    uint8_t bit_no;
    uint32_t irq_set;
    uint32_t packets_read = 0;
    
    
 

    if (mouse_subscribe_int(&bit_no) != 0) return 1;
    irq_set = BIT(bit_no);

    if (mouse_send_command(ENABLE_DATA_REPORTING) != 0) {
        return 1;
    }
  

    while (packets_read < cnt) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        mouse_ih();

                        if (packet_complete()) {
                            mouse_print_packet(get_packet());
                            reset_packet();
                            packets_read++;
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    }
  



    if (mouse_send_command(DISABLE_DATA_REPORTING) != 0) return 1;

    if (mouse_unsubscribe_int() != 0) return 1;
    

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    int r, ipc_status;
    message msg;
    bool running = true;
    uint8_t bit_mouse, bit_timer;
    uint32_t irq_set_timer, irq_set_mouse;
    

    extern uint32_t counter;
    uint32_t mouse_time = 0;
    counter = 0;


    if (timer_subscribe_int(&bit_timer) != 0) return 1;
    if (mouse_subscribe_int(&bit_mouse) != 0) {
        timer_unsubscribe_int();
        return 1;
    }


    if (mouse_send_command(ENABLE_DATA_REPORTING) != 0) {
        return 1;
    }


    irq_set_timer = BIT(bit_timer);
    irq_set_mouse = BIT(bit_mouse);

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        mouse_ih();

                        if (packet_complete()) {
                            mouse_print_packet(get_packet());
                            reset_packet();
                            mouse_time = counter;
                        }
                    }

                    if (msg.m_notify.interrupts & irq_set_timer){
                        timer_int_handler();

                        if ((counter - mouse_time) >= idle_time * sys_hz())
                            running = false;
                    }
                    break;

                default:
                    break;
            }
        }
    }

  
    if (mouse_send_command(DISABLE_DATA_REPORTING) != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;
    if (timer_unsubscribe_int() != 0) return 1;

    return 0;
}
