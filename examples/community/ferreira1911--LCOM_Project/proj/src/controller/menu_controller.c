#include <lcom/lcf.h>

#include "menu_controller.h"
#include "model/state.h"

#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "devices/video.h"
#include "view/menu_view.h"
#include "crosshair_controller.h"

#include "xpms/logo.xpm"
#include "xpms/classic.xpm"
#include "xpms/falling.xpm"
#include "xpms/fly.xpm"
#include "xpms/quit.xpm"

MenuItem menu_items[NUM_MENU_ITEMS];

extern uint8_t scancode;
extern uint8_t byte_index;
extern GameState game_state;

extern Crosshair crosshair;

void (menu_controller_init)() {
    create_menu_item(&menu_items[0], 125, 100, (xpm_map_t) logo);
    create_menu_item(&menu_items[1], 229, 310, (xpm_map_t) classic);
    create_menu_item(&menu_items[2], 425, 310, (xpm_map_t) falling);
    create_menu_item(&menu_items[3], 229, 414, (xpm_map_t) fly);
    create_menu_item(&menu_items[4], 425, 414, (xpm_map_t) quit);
}


void (menu_controller_draw)() {
    for (int i = 0; i < NUM_MENU_ITEMS; i++) {
        if (menu_items[i].isVisible)
            draw_menu_item(&menu_items[i]);
    }

}

int (draw_menu_elements)() {
    vg_clear_screen();
    menu_controller_draw();
    crosshair_controller_draw();

    swap_buffers();
    return 0;
}


int menu_controller_check_click(int x, int y) {
    for (int i = 1; i < NUM_MENU_ITEMS; i++) {
        if (!menu_items[i].isVisible) continue;

        int item_x = menu_items[i].x;
        int item_y = menu_items[i].y;
        int item_width = menu_items[i].width;
        int item_height = menu_items[i].height;

        if (x >= item_x && x <= item_x + item_width && y >= item_y && y <= item_y + item_height) {
            return i;
        }
    }
    return 0;
}



int (menu_loop)() {
    menu_controller_init();
    crosshair_controller_init();
    
    draw_menu_elements();


    uint8_t kbd_bit_no = 1;
    uint8_t mouse_bit_no = 2;

    int ipc_status;
    message msg;

    if (kbd_subscribe_int(&kbd_bit_no) != 0) return 1;
    int kbd_irq_set = BIT(kbd_bit_no);

    if (mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
    int mouse_irq_set = BIT(mouse_bit_no);

    if (write_mouse_cmd(MOUSE_DATA_REPORT_ENABLE) != 0) return 1;

    bool start_game = false;

    while (!start_game) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {

            if (msg.m_notify.interrupts & kbd_irq_set) {
                kbc_ih();

                if (scancode == ESC_BREAKCODE) {
                    game_state = GAME_OVER;
                    break;
                }

                if (scancode == 0x02) {
                    start_game = true;
                    game_state = PLAYING_MODE_1;
                    break;
                }

                if (scancode == 0x03) {
                    start_game = true;
                    game_state = PLAYING_MODE_2;
                    break;
                }

                if (scancode == 0x04) {
                    start_game = true;
                    game_state = PLAYING_MODE_3;
                    break;
                }


            }

            if (msg.m_notify.interrupts & mouse_irq_set) {
                mouse_ih();
                sync_bytes();

                if (byte_index == 3) {
                    struct packet pp;

                    parse_packet(&pp);
                    crosshair_controller_update(&pp);

                    if (draw_menu_elements() != 0) return 1;

                    if(pp.lb){
                        int clicked_item = menu_controller_check_click((crosshair.x + crosshair.width / 2), (crosshair.y + crosshair.height / 2));
                        switch (clicked_item) {
                            case 1:
                                start_game = true;
                                game_state = PLAYING_MODE_1;
                                break;
                            case 2:
                                start_game = true;
                                game_state = PLAYING_MODE_2;
                                break;
                            case 3:
                                start_game = true;
                                game_state = PLAYING_MODE_3;
                                break;
                            case 4:
                                game_state = GAME_OVER;
                                start_game = true;
                                break;
                            default:
                                break;
                        }
                    }

                    byte_index = 0;
                }
            }
        }
    }

    if (kbd_unsubscribe_int() != 0) return 1;
    if (write_mouse_cmd(MOUSE_DATA_REPORT_DISABLE) != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;

    return 0;
}
