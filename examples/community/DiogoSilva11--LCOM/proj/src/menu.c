#include <lcom/lcf.h>

#include <stdint.h>

#include "menu.h"

#include "game.h"

// ----------------------------------------------------------------------------------------

int (main_menu)() {
    if (init()) 
        return 1;
        
    int r, ipc_status;
    message msg;

    uint8_t key_code = 0;
    int process = 1;
    while (process) {
        if (displayMenu())
            return 1;

        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // hardware interrupt notification
                    if (msg.m_notify.interrupts & kbd_irq_set) { // subscribed interrupt
                        kbc_ih();
                        if (!kbd_inc_code()) {
                            key_code = get_key();
                            
                            if (key_code == KEY_1) {
                                if (gameplay())
                                    return 1;
                            }
                            else if (key_code == KEY_0)
                                process = 0;
                        }
                    }
                    break;
                default:
                    break; // no other notifications expected: do nothing
            }
        }
        else { // received a standard message, not a notification
            // no standard messages expected: do nothing
        }
    }

    if (leave())
        return 1;

    return 0;
}

// ----------------------------------------------------------------------------------------

int (displayMenu)() {
    vg_clear_screen();

    for (int i = 0; i < CONSOLE_WIDTH; i++) {
        for (int j = 0; j < CONSOLE_HEIGHT; j++) {
            if (vg_generate_pixel(i, j, 4)) {
                if (leave())
                    return 1;
                return 1;
            }
        }
    }

    if (vg_draw_element(sprite_1, MENU_X_ORIGIN - 80, MENU_Y_ORIGIN)) {
        if (leave())
            return 1;
        return 1;
    }

    if (vg_draw_element(P_sprite, MENU_X_ORIGIN, MENU_Y_ORIGIN)) {
        if (leave())
            return 1;
        return 1;
    } 

    if (vg_draw_element(L_sprite, MENU_X_ORIGIN + 50, MENU_Y_ORIGIN)) {
        if (leave())
            return 1;
        return 1;
    } 

    if (vg_draw_element(A_sprite, MENU_X_ORIGIN + 100, MENU_Y_ORIGIN)) {
        if (leave())
            return 1;
        return 1;
    } 

    if (vg_draw_element(Y_sprite, MENU_X_ORIGIN + 150, MENU_Y_ORIGIN)) {
        if (leave())
            return 1; // ...
        return 1;
    } 

    if (vg_draw_element(sprite_0, MENU_X_ORIGIN - 80, MENU_Y_ORIGIN + 100)) {
        if (leave())
            return 1;
        return 1;
    }

    if (vg_draw_element(E_sprite, MENU_X_ORIGIN, MENU_Y_ORIGIN + 100)) {
        if (leave())
            return 1;
        return 1;
    } 

    if (vg_draw_element(X_sprite, MENU_X_ORIGIN + 50, MENU_Y_ORIGIN + 100)) {
        if (leave())
            return 1;
        return 1;
    } 

    if (vg_draw_element(I_sprite, MENU_X_ORIGIN + 110, MENU_Y_ORIGIN + 100)) {
        if (leave())
            return 1;
        return 1;
    } 

    if (vg_draw_element(T_sprite, MENU_X_ORIGIN + 150, MENU_Y_ORIGIN + 100)) {
        if (leave())
            return 1;
        return 1;
    } 

    vg_draw_content();

    return 0;
}

// ----------------------------------------------------------------------------------------
