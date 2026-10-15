// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include "video_gr.h"
#include "vesa-bios.h"
#include "../lab3/i8042.h"
#include "../lab3/kbc.h"

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab5/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    
    if (set_video_mode(mode)) return 1;
    sleep(delay);
    vg_exit();
    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

    void* addr = graphics_init(mode);
    if (addr == NULL) return 1;

    if (vg_draw_rectangle(x, y, width, height, color)) {
        vg_exit();
        return 1;
    }

    //----- KEYBOARD (check if we recieve a scancode to exit the program) -----

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

    while(scancode[0] != ESC_BREAK_CODE) { //exit on ESC break code, but if it's a two byte scancode, we need to read both bytes before exiting

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
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    if(vg_exit()) return 1;

    if(kbc_unsubscribe_int()) return 1;


    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    void* addr = graphics_init(MODE_105);
    if (addr == NULL) return 1;

    if(vg_draw_xpm(xpm, x, y, XPM_INDEXED)) {
        vg_exit();
        return 1;
    }


    //----- KEYBOARD (check if we recieve a scancode to exit the program) -----

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

    while(scancode[0] != ESC_BREAK_CODE) { //exit on ESC break code, but if it's a two byte scancode, we need to read both bytes before exiting

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
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    if(vg_exit()) return 1;

    if(kbc_unsubscribe_int()) return 1;


    return 0;
}
