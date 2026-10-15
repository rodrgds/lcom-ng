
/**
 * @file main.c
 * @brief Entry point and main loop for the Snake Game project.
 *
 * Initializes graphics mode, input handlers, menu and high‐score system,
 * then enters the main event loop handling keyboard and timer interrupts.
 */
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "controllers/video/graphics.h"
#include "controllers/keyboard/keyboard.h"
#include "controllers/keyboard/i8042.h"
#include "controllers/menu/menu.h"
#include "game/game.h"

uint8_t kbd_mask;
static uint8_t timer_mask; 
extern uint8_t scancode;
extern int update_tick_threshold;

/**
 * @brief Program entry point.
 *
 * Configures LCF, parses arguments and hands control to proj_main_loop().
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 on success, non-zero on failure.
 */

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  lcf_cleanup();
  return 0;
}
/**
 * @brief Main loop once graphics mode is set.
 *
 * Initializes VBE mode, VRAM mapping, menu, timer and keyboard subscriptions,
 * then enters the event loop which alternates between menu and game logic.
 *
 * @param argc Ignored.
 * @param argv Ignored.
 * @return 0 on clean exit, 1 on error.
 */
int (proj_main_loop)(int argc, char *argv[]) {
    uint16_t mode = 0x117;  
    if(vbe_get_mode_info(mode, &mode_info) != OK) return 1;
    if(map_vram(mode)) return 1;
    if(set_graphics_mode(mode)) return 1;

    if(menu_init()) return 1;

    uint8_t bit_no_kbd = 1;
    uint8_t bit_no_timer = 0;
    
    kbd_mask = bit_no_kbd;
    timer_mask = bit_no_timer;

    if (kbd_subscribe_int(&bit_no_kbd) != 0) return 1;
    if (timer_subscribe_int(&bit_no_timer) != 0) return 1;
    if (timer_set_frequency(0, 60) != 0) return 1;  
    
    message msg;
    int ipc_status;
    int timer_ticks = 0;
    bool need_redraw = true;
    bool first_game_draw = true;

    while (game_is_running() && scancode != ESC_BREAK_CODE) {
        if (need_redraw) {
            if (menu_is_active()) {
                menu_draw();
                first_game_draw = true;  
            } else {
                if (first_game_draw) {
                    draw_game_static();
                    first_game_draw = false;
                }
                draw_game_dynamic();
            }
            need_redraw = false;
        }
        
        if (driver_receive(ANY, &msg, &ipc_status) != OK) continue;
        
        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & BIT(kbd_mask)) {
                kbc_ih();
                if (!(scancode & 0x80)) {
                    if (menu_is_active()) {
                        menu_handle_key(scancode);
                        need_redraw = true;
                    } else {
                        handle_key(scancode);
                    }
                }
            }
            
            if (!menu_is_active() && (msg.m_notify.interrupts & BIT(timer_mask))) {
                timer_ticks++;
                if (timer_ticks >= update_tick_threshold) {
                    update_game();
                    need_redraw = true;
                    timer_ticks = 0;
                }
            }
        }
    }
    
    kbd_unsubscribe();
    timer_unsubscribe_int();
    vg_exit();
    return 0;
}
