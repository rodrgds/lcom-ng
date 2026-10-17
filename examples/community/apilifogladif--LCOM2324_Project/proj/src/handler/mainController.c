#include <lcom/lcf.h>
#include "mainController.h"


extern Sprite *cursor;
extern Sprite *hand_cursor;
extern Sprite *crosshair;

extern struct packet mpacket;
extern bool changeTurn;
extern int global_counter;

extern int x;
extern int y;

uint8_t irq_mouse, irq_kbd, irq_timer, irq_ser;
uint8_t scancode;

enum MODE menu = MAINMENU;

extern enum THISPLAYER player_status;

void controller() {
    int ipc_status;
    message msg;
    bool changed = false;

    drawMainMenu();
    createMatrix(BIG_BOARD_X, BIG_BOARD_Y, BIG_BOARD_SQUARE);
    draw_sprite(cursor);
    swap_buffers();

    subscribe_int();

    serial_clear();

    timer_set_frequency(TIMER_0, 30);

    while (menu != QUIT) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            continue;
        }
        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if(msg.m_notify.interrupts & irq_kbd) {
                if (read_output(OUT_BUF, &scancode, 0) != 0) {
                    printf("Error (read_output)");
                }

                if (menu == MAINMENU) {
                    checkKeyboardInputMainMenu(scancode);
                }
                else if (menu == PLACINGMENU) {
                    checkKeyboardInputPlacing(scancode);
                }
                else if (menu == GAMEREMOTE) {
                    if (checkKeyboardInputGameRemote(scancode) == 1) 
                        menu = MAINMENU;
                }
                else if (menu == GAMELOCAL) {
                    if (checkKeyboardInputGameLocal(scancode) == 1) 
                        menu = MAINMENU;
                }
                else if (menu == HELP) {
                    if (checkKeyboardInputHelp(scancode) == 1) {
                        menu = MAINMENU;
                        changed = true;
                    }
                }
            }
            if(msg.m_notify.interrupts & irq_mouse) {
                mouse_ih();
                if (mouse_output()) {
                    create_mpacket();
                    if (mpacket.delta_x != 0 || mpacket.delta_y != 0) {
                        changeMousePosition();
                        drawMouse();
                    }
                    if (mpacket.lb) 
                        changed = checkClick();
                }
            }
            if(msg.m_notify.interrupts & irq_timer) {
                timer_int_handler();
                if (global_counter % 1 == 0) {
                    swap_buffers();
                }
                if (global_counter == 600 && menu == GAMELOCAL) {
                    changeTurn = true;
                    drawGameLocal();
                    drawMouse();
                    swap_buffers();
                }
                if ((global_counter % 30 == 0) && (menu == GAMELOCAL)) drawGameLocal();
                
            }
            if(msg.m_notify.interrupts & irq_ser) {
                uint8_t val = sp_ih();
                if (val != 1 && received(val) == 1) menu = MAINMENU;
            }
        }
        if (changed) {
            if (menu == MAINMENU) {
                drawMainMenu();
                drawMouse();
            }
            else if (menu == PLACINGMENU) {
                drawPlacingMenu();
            }
            else if (menu == GAMELOCAL) {
                drawGameLocal();
                drawMouse();
            }
            else if (menu == GAMEREMOTE) {
                drawGameRemote();
            }
            else if (menu == HELP) {
                drawHelpMenu();
            }
            changed = false;
        }
    }
    
    unsubscribe_int();

}

void subscribe_int() {
    mouse_subscribe_int(&irq_mouse);
    kbd_subscribe_int(&irq_kbd);
    timer_subscribe_int(&irq_timer);
    mouse_data_report(ENABLE_CMD);
    ser_subscribe_int(&irq_ser);
}

void unsubscribe_int() {
    mouse_data_report(DISABLE_CMD);
    mouse_unsubscribe_int();
    kbd_unsubscribe_int();
    timer_unsubscribe_int();
    ser_unsubscribe_int();
}

void changeMousePosition() {
    cursor->x = x;
    cursor->y = y;
    hand_cursor->x = x;
    hand_cursor->y = y;
    if (y > 127 && y < 811) {
        crosshair->y = y - 33;
    }
    if (x > 85 && x < 770) {
        crosshair->x = x - 33;
    }
}

void drawMouse() {
    if (menu == MAINMENU) {
        drawMainMenu();
        drawMainCursor();
    }
    else if (menu == PLACINGMENU) {
        drawPlacingMenu();
        drawPlacingCursor();
    }
    else if (menu == GAMEREMOTE) {
        if (player_status == MYTURN) drawGameRemote();
    }
    else if (menu == GAMELOCAL) {
        drawGameLocal();
    }
    else if (menu == HELP) {
        drawHelpMenu();
    }
}

bool checkClick() {
    if (menu == MAINMENU) {
        return checkMainClick();
    }
    else if (menu == PLACINGMENU) {
        int aux = checkPlacingClick();
        if (aux == 0) return false;
        else if (aux == -1) {
            menu = MAINMENU;
            return true;
        }
        else if (aux == 1) {
            menu = GAMELOCAL;
            return true;
        }
    }
    else if (menu == GAMELOCAL) {
        checkGameLocalClick();
        return true;
    }
    else if (menu == GAMEREMOTE) {
        checkGameRemoteClick();
        return true;
    }
    return false;
}
