/**
 * @file kbd_handler.c
 * @brief Handles keyboard input events and updates game state accordingly.
 */

#include "xpms/selected1.xpm"
#include "xpms/selected2.xpm"
#include "xpms/selected3.xpm"

#include <lcom/lcf.h>
#include <lcom/xpm.h>

#include "kbd_handler.h"
#include "gameState.h"
#include "manager.h"
#include "menu.h"
#include "characters.h"
#include "controllers/video/graphics.h"

extern vbe_mode_info_t mode_info;
int kbd_state = 1;

/**
 * @brief Handles a keyboard event by processing the given scan code.
 *
 * Interprets the scan code (1-byte or 2-byte), determines whether it is a make or break code,
 * and updates the game state accordingly. The function currently only handles break codes.
 * In the PLAYING state, it allows switching control between different characters using keys 1–3.
 *
 * @param scan_code A 2-byte array representing the scan code received from the keyboard.
 *                  For single-byte scan codes, only scan_code[0] is used.
 */
void kbd_event_handler(uint8_t scan_code[2]) {
    uint16_t full_scancode;

    // Check for two-byte scancode and build full scancode
    if (scan_code[0] == TWO_BYTE_CODE)
        full_scancode = (scan_code[0] << 8) | scan_code[1];
    else
        full_scancode = scan_code[0];

    // Determine if it's a break code (key released)
    bool is_break = (full_scancode & 0x80) != 0;
    printf("Scancode: 0x%X (%s code)\n", full_scancode, is_break ? "break" : "make");

    // Only handle break codes
    if (!is_break)
        return;

    // Handle based on current game state
    switch (state) {
        case MAIN_MENU:
            if (full_scancode == ESC_BREAK)
                state = EXIT;
            break;

        case INSTRUCTIONS:
            if (full_scancode == ESC_BREAK)
                state = MAIN_MENU;
            break;

        case PLAYING:
            if (full_scancode == ESC_BREAK) {
                state = MAIN_MENU;
            }
            else if (full_scancode == ONE_T) {
                kbd_state = 1;
            }
            else if (full_scancode == TWO_T) {
                kbd_state = 2;
            }
            else if (full_scancode == THREE_T) {
                kbd_state = 3;
            }
            break;

        default:
            break;
    }
}

