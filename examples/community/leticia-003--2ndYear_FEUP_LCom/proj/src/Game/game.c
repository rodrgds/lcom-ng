/**
 * @file game.c
 * @brief Main game loop and event handling for the breakout-style game.
 */

#include "game.h"
#include "menu.h"
#include "game_state.h"
#include "game_logic.h"
#include <lcom/lcf.h>
#include <stdio.h>
#include "Drivers/Keyboard/keyboard.h"
#include "Drivers/Mouse/mouse.h"
#include "Drivers/Timer/timer.h"
#include "Drivers/VBE/vbe.h"
#include "pause_menu.h"

extern uint8_t pkt_bytes[3];
extern uint8_t pkt_index;

/**
 * @brief Runs the main game loop, handling events and updating game state.
 * @return 0 on success, 1 on failure.
 */
int game_run() {
    printf("[game_run] entered  ✅\n"); fflush(stdout);
    int ipc_status;
    message msg;
    uint8_t kbd_bit_no, mouse_bit_no, timer_bit_no;
    int irq_kbd, irq_mouse, irq_timer;

    // Subscribe interrupts
    if (kbd_subscribe_int(&kbd_bit_no)) return 1;
    if (mouse_subscribe_int(&mouse_bit_no)) return 1;
    if (timer_subscribe_int(&timer_bit_no)) return 1;

    irq_kbd = BIT(kbd_bit_no);
    irq_mouse = BIT(mouse_bit_no);
    irq_timer = BIT(timer_bit_no);

    if (mouse_send_cmd_enable()) return 1;

    if (set_graphics_mode(0x11B)) return 1;
    if (map_vram(0x11B)) return 1;
    if (vg_enable_double_buffering()) return 1;

    vbe_mode_info_t mode_info;
    if (vbe_get_mode_info(0x11B, &mode_info) != OK) {
        printf("Failed to get VBE mode info\n");
        return 1;
    }

    if (menu_assets_init(&mode_info)) return 1;
    game_assets_init(&mode_info);
    pause_menu_assets_init();

    GameState state = STATE_MENU;
    bool running = true;

    while (running) {
        while(driver_receive(ANY, &msg, &ipc_status) == 0){
            if (is_ipc_notify(ipc_status)) {
                switch (_ENDPOINT_P(msg.m_source)) {
                    case HARDWARE:

                        if (msg.m_notify.interrupts & irq_kbd) {
                            kbc_ih();
                            if (scancode_ready) {
                                scancode_ready = false;

                                bool esc_pressed =
                                    (scancode_size == 1 && scancode[0] == ESC_MAKE_CODE) ||
                                    (scancode_size == 2 && scancode[0] == 0xE0 && scancode[1] == ESC_MAKE_CODE);

                                if (esc_pressed) {
                                    running = false;
                                    break;
                                }

                                if (state == STATE_PLAYING) {
                                    if (scancode_size == 1 && scancode[0] == 0x19) {
                                        bool paused = !pause_menu_is_active();
                                        pause_menu_set_active(paused);
                                        needs_redraw = true;
                                    }
                                    else if (pause_menu_is_active()) {
                                        pause_menu_handle_input(scancode, scancode_size);
                                    }                        
                                    else {
                                        game_handle_keyboard(scancode, scancode_size);
                                    }
                                } else {
                                    menu_update_keyboard(scancode, scancode_size);
                                }
                            }
                        }

                        /* Input can change the shared state within this notification.
                         * Use it before mouse handling and the timer repaint. */
                        state = game_get_state();

                        if (msg.m_notify.interrupts & irq_mouse) {
                            mouse_ih();                       
                            if (pkt_index == 0) {    
                                struct packet p;
                                mouse_parse_packet(&p, pkt_bytes);
                                if (state == STATE_PLAYING && !pause_menu_is_active())  
                                    game_handle_mouse(&p);
                            }
                        }

                        if (msg.m_notify.interrupts & irq_timer) {
                            timer_int_handler();
                            if (state == STATE_PLAYING && !pause_menu_is_active()) {
                                /**
                                 * @brief Updates the game state, including ball movement, collision, and victory/game over checks.
                                 */
                                game_update();
                            } else if (state == STATE_PLAYING && pause_menu_is_active()) {
                            }

                            if ((state == STATE_MENU || state == STATE_INSTRUCTIONS) && needs_redraw) {
                                menu_draw();
                                vg_swap_buffers();
                                needs_redraw = false;
                            }
                            else if (state == STATE_PLAYING && needs_redraw) {
                                if (pause_menu_is_active()) {
                                    pause_menu_draw();
                                } else {
                                    game_draw();
                                }
                                vg_swap_buffers();
                                needs_redraw = false;
                            }
                        }
                        break;
                }
            }

            state = game_get_state();

            if (state == STATE_EXIT) {
                running = false;
            }
        }
    }

    mouse_send_cmd_disable();
    mouse_unsubscribe_int();
    kbd_unsubscribe_int();
    timer_unsubscribe_int();
    menu_assets_destroy();
    game_assets_destroy();
    vg_disable_double_buffering();
    vg_exit();
    printf("Exited game cleanly.\n");

    return 0;
}
