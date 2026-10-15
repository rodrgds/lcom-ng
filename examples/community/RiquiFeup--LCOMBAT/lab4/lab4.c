#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"
#include "timer_ext.h"

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");
    lcf_log_output("/home/lcom/labs/lab4/output.txt");
    if (lcf_start(argc, argv)) return 1;
    lcf_cleanup();
    return 0;
}

// Prints the requested number of mouse packets.
int (mouse_test_packet)(uint32_t cnt) {
    uint8_t mouse_bit_no;
    uint32_t pacotes_lidos = 0;
    int r, ipc_status;
    message msg;

    // Use the framework function for test 1.
    mouse_enable_data_reporting(); 
    
    // Subscribe mouse interrupts.
    if (mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
    uint32_t irq_mouse = BIT(mouse_bit_no);

    // Loop until the requested packets are read.
    while (pacotes_lidos < cnt) {
        // Receive a message.
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;
        // Check whether it is an interrupt notification.
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // Check whether it is a mouse interrupt.
                    if (msg.m_notify.interrupts & irq_mouse) {
                        
                        mouse_ih(); 
                        
                        // Check whether the packet was built correctly.
                        if (mouse_sync_and_build_packet() == true) {
                            mouse_print_packet(mouse_get_packet());
                            pacotes_lidos++;
                        }
                    }
                    break;
                default: 
                    break;
            }
        }
    }

    mouse_unsubscribe_int();
    mouse_write_cmd(MOUSE_DIS_DATA); // Disable with our command helper.

    return 0;
}

// Prints mouse packets until the idle timeout expires.
int (mouse_test_async)(uint8_t idle_time) {
    // Local variables.
    uint8_t mouse_bit, timer_bit;
    int r, ipc_status;
    message msg;

    // Enable with our command helper.
    mouse_write_cmd(MOUSE_EN_DATA); 
    
    if (mouse_subscribe_int(&mouse_bit) != 0) return 1;
    if (timer_subscribe_int(&timer_bit) != 0) return 1;
    
    // Interrupt bitmasks.
    uint32_t irq_mouse = BIT(mouse_bit);
    uint32_t irq_timer = BIT(timer_bit);
        
    timer_reset_counter();
    int max_ticks = idle_time * 60;

    while (timer_get_counter() < max_ticks) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_timer) {
                        // Add one timer tick.
                        timer_int_handler();
                    }

                    if (msg.m_notify.interrupts & irq_mouse) {
                        mouse_ih();
                        
                        // Reset the timeout.
                        timer_reset_counter();

                        // Check whether the packet was built correctly.
                        if (mouse_sync_and_build_packet() == true) {
                            mouse_print_packet(mouse_get_packet());
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    timer_unsubscribe_int();
    mouse_unsubscribe_int();

    // Disable the mouse.
    mouse_write_cmd(MOUSE_DIS_DATA); 

    return 0;
}
