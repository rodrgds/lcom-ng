#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "proj.h"
#include "game.h"

#include <lcom/timer.h>
#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"
#include "v_card.h"

uint8_t timer_bit_no;
uint8_t kbd_bit_no;
uint8_t mouse_bit_no;
uint8_t rtc_bit_no;

// Mouse packet assembly — the mouse sends 3 bytes per event
static uint8_t mouse_bytes[3];
static uint8_t mouse_byte_idx = 0;


int system_init() {
    if (timer_set_frequency(0, FPS) != 0) {
        printf("Error: Failed to set Timer frequency.\n");
        return 1;
    }
    if (timer_subscribe_int(&timer_bit_no) != 0) {
        printf("Error: Failed to subscribe Timer interrupts.\n");
        return 1;
    }

    if (kbd_subscribe_int(&kbd_bit_no) != 0) {
        printf("Error: Failed to subscribe Keyboard interrupts.\n");
        return 1;
    }

    if (mouse_enable_data_report() != 0) {
        printf("Error: Failed to enable Mouse data reporting.\n");
        return 1;
    }
    if (mouse_subscribe_int(&mouse_bit_no) != 0) {
        printf("Error: Failed to subscribe Mouse interrupts.\n");
        return 1;
    }

    if (rtc_subscribe_int(&rtc_bit_no) != 0) {
        printf("Error: Failed to subscribe RTC interrupts.\n");
        return 1;
    }

    if (vg_init(VIDEO_MODE) == NULL) {
        printf("Error: Failed to initialize video mode.\n");
        return 1;
    }

    return 0;
}

int system_cleanup() {
    int errors = 0;

    
    if (mouse_unsubscribe_int() != 0) {
        printf("Error: Failed to unsubscribe Mouse.\n");
        errors = 1;
    }
    if (mouse_disable_data_report() != 0) {
        printf("Error: Failed to disable Mouse data reporting.\n");
        errors = 1;
    }

    if (kbd_unsubscribe_int() != 0) {
        printf("Error: Failed to unsubscribe Keyboard.\n");
        errors = 1;
    }

    if (timer_unsubscribe_int() != 0) {
        printf("Error: Failed to unsubscribe Timer.\n");
        errors = 1;
    }

    if (rtc_unsubscribe_int() != 0) {
        printf("Error: Failed to unsubscribe RTC.\n");
        errors = 1;
    }

    if (vg_exit() != 0) {
        printf("Error: Failed to exit video mode.\n");
        errors = 1;
    }


    // flush pensing keyboard inputs
    uint8_t status, trash;
    while (1) {
        util_sys_inb(0x64, &status);
        if (status & BIT(0)) { 
            util_sys_inb(0x60, &trash); 
        } else {
            break;
        }
    }

    return errors;
}

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    
    if (lcf_start(argc, argv)) return 1;
    
    lcf_cleanup();
    
    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
    printf("Initializing system\n");

    if (system_init() != 0) {
        printf("System initialization failed. \n");
        system_cleanup();
        return 1;
    }

    game_init();

    int ipc_status;
    message msg;

    while (!game_should_quit()) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // --- Timer interrupt: drive game logic and rendering ---
                    if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
                        timer_int_handler();
                        if (timer_counter % (60 / FPS) == 0) {
                            game_update();   // logic
                            game_render();   // draw
                        }
                    }
                    // --- Keyboard interrupt ---
                    if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
                        kbc_ih();
                        
                        if (!kbd_has_error()) {
                            game_on_key(kbd_get_scancode());
                        }
                    }
                    if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
                        mouse_ih();
 
                        if (!mouse_error) {
                            // The first byte of a packet always has bit 3 set.
                            // Use this to re-sync if we ever lose track.
                            if (mouse_byte_idx == 0 && !(mouse_byte & BIT(3))) {
                                // Out of sync — discard byte and wait for a valid first byte
                                continue;
                            }
            
                            mouse_bytes[mouse_byte_idx++] = mouse_byte;
            
                            if (mouse_byte_idx == 3) {
                                mouse_byte_idx = 0;
            
                                struct packet pp;
                                mouse_parse_packet(&pp, mouse_bytes);
                                game_on_mouse(&pp);
                            }
                        }
                    }
                    break;
            }
        }
    }

    printf("Shutting down...\n");

    if (system_cleanup() != 0) {
        printf("Failed to reset Minix state.\n");
        return 1;
    }

    printf("System shut down successfully.\n");
    return 0;
}
