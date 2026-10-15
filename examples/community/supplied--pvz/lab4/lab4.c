// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "timer.h"
#include "mouse.h"
#include "i8042.h"

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
// 

int (mouse_test_packet)(uint32_t cnt) {
    uint8_t bit_no;

    if (mouse_subscribe_int(&bit_no) != 0) return 1;
    uint32_t irq_set = BIT(bit_no);
    
    // Enable data reporting from mouse
    if (mouse_enable_data_reporting() != 0) return 1;

    int ipc_status;
    message msg;
    uint32_t packets_read = 0;

    uint8_t packet_bytes[3]; // 1st byte has sync bit (which button was pressed), 2nd and 3rd are for delta x and delta y (movement)
    int byte_index = 0;
    struct packet pp;

    while (packets_read < cnt) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        
                        // Call the Interrupt Handler
                        mouse_ih();

                        // Check for errors
                        if (get_mouse_error()) {
                            byte_index = 0; // Discard the read bytes and restart the packet
                            continue;
                        }

                        uint8_t byte = get_mouse_byte();

                        // if reading 1st byte, bit 3 must be 1
                        if (byte_index == 0 && (byte & BIT(3)) == 0) {
                            continue; // Not the first byte, ignore
                        }

                        // Store the byte in the array and advance the index
                        packet_bytes[byte_index] = byte;
                        byte_index++;

                        // When we have all 3 bytes, build and print the packet struct
                        if (byte_index == 3) {
                            mouse_build_packet(packet_bytes, &pp);
                            packets_read++;
                            byte_index = 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // Always run both cleanup steps, even if one fails
    int ret = 0;

    // Disable data reporting from mouse (must be done before unsubscribing)
    if (mouse_write_command(DIS_DATA_REPORT) != 0) ret = 1;

    // Unsubscribe from mouse interrupts
    if (mouse_unsubscribe_int() != 0) ret = 1;

    return ret;
}

int (mouse_test_async)(uint8_t idle_time) {
    uint8_t bit_no_mouse;
    uint8_t bit_no_timer;

    if (mouse_subscribe_int(&bit_no_mouse) != 0) return 1;
    uint32_t irq_set_mouse = BIT(bit_no_mouse);

    if (timer_subscribe_int(&bit_no_timer) != 0) {
        mouse_unsubscribe_int();
        return 1;
    }
    
    uint32_t irq_set_timer = BIT(bit_no_timer);

    // Enable data report
    if (mouse_write_command(EN_DATA_REPORT) != 0) {
        mouse_unsubscribe_int();
        timer_unsubscribe_int();
        return 1;
    }

    // Get timer 0 frequency to know how many ticks = idle_time seconds
    uint32_t freq = sys_hz(); // default is 60Hz
    uint32_t max_ticks = (uint32_t) idle_time * freq;

    int ipc_status;
    message msg;

    uint8_t packet_bytes[3]; // 1st byte: buttons/sync; 2nd: delta X; 3rd: delta Y
    int byte_index = 0;
    struct packet pp;
    bool running = true;

    while (running) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // Check timer interrupt
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        timer_int_handler();
                        if (get_timer_counter() >= max_ticks) {
                            running = false; // Idle timeout reached
                        }
                    }

                    // Check mouse interrupt
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        mouse_ih();
                        reset_timer_counter(); // Reset idle timer on mouse activity

                        if (get_mouse_error()) {
                            byte_index = 0; // Discard bytes and restart packet
                        } else {
                            uint8_t byte = get_mouse_byte();

                            // if reading 1st byte, bit 3 must be 1
                            if (byte_index == 0 && (byte & BIT(3)) == 0) {
                            break; // Not the first byte, ignore
                        }

                                // Store the byte in the array and advance the index
                                packet_bytes[byte_index] = byte;
                                byte_index++;

                                // When we have all 3 bytes, build and print the packet struct
                                if (byte_index == 3) {
                                    mouse_build_packet(packet_bytes, &pp);
                                    byte_index = 0;
                                }
                            }
                        }
                    break;
                default:
                    break;
                }
            }
        }
    // Always run all cleanup steps, even if one fails
    int ret = 0;
    if (mouse_write_command(DIS_DATA_REPORT) != 0) ret = 1;
    if (mouse_unsubscribe_int() != 0) ret = 1;
    if (timer_unsubscribe_int() != 0) ret = 1;
    return ret;
}
