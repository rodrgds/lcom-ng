#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/kbc/mouse/mouse.h"
#include "../game_controller.h"
#include "../../view/cursor_view/cursor_view.h"
#include "../../view/menu_view/menu_view.h"
#include "mouse_controller.h"

int mouse_x = 0;
int mouse_y = 0;

extern int packet_index;
extern uint8_t packet_bytes[3];
extern xpm_image_t cursor_info;

extern bool menu_is_on;
extern bool game_is_on;
extern bool rules_is_on;
extern bool exited;

void (check_button)(bool clicked) {
    if (mouse_x >= PACMAN_TEXT_X && mouse_x < PACMAN_TEXT_X + PACMAN_TEXT_WIDTH &&
        mouse_y >= PACMAN_TEXT_Y && mouse_y < PACMAN_TEXT_Y + PACMAN_TEXT_HEIGHT && !rules_is_on)
        draw_pacman_text();

    if (mouse_x >= RULES_TEXT_X && mouse_x < RULES_TEXT_X + RULES_TEXT_WIDTH &&
        mouse_y >= RULES_TEXT_Y && mouse_y < RULES_TEXT_Y + RULES_TEXT_HEIGHT && !rules_is_on) {
        if (clicked) {
            rules_is_on = true;
            erase_menu();
            draw_rules_description();
        }
        else
            draw_rules_text();
    }

    if (mouse_x >= PLAY_TEXT_X && mouse_x < PLAY_TEXT_X + PALY_TEXT_WIDTH &&
        mouse_y >= PLAY_TEXT_Y && mouse_y < PLAY_TEXT_Y + PLAY_TEXT_HEIGHT && !rules_is_on) {
        if (clicked) {
            menu_is_on = false;
            game_is_on = true;
        }
        else
            draw_play_text();
    }

    if (mouse_x >= EXIT_TEXT_X && mouse_x < EXIT_TEXT_X + EXIT_TEXT_WIDTH &&
        mouse_y >= EXIT_TEXT_Y && mouse_y < EXIT_TEXT_Y + EXIT_TEXT_HEIGHT && !rules_is_on) {
        if (clicked) {
            exited = true;
            menu_is_on = false;
            game_is_on = false;
        }
        else
            draw_exit_text();
    }

    if (mouse_x >= BACK_TO_MENU_X && mouse_x < BACK_TO_MENU_X + BACK_TO_MENU_WIDTH &&
        mouse_y >= BACK_TO_MENU_Y && mouse_y < BACK_TO_MENU_Y + BACK_TO_MENU_HEIGHT && rules_is_on) {
        if (clicked) {
            rules_is_on = false;
            erase_menu();
            draw_menu();
        }
        else
            draw_rules_description();
    }
    if (((mouse_x >= RULES_DESCRIPTION1_X && mouse_x < RULES_DESCRIPTION1_X + RULES_DESCRIPTION1_WIDTH &&
        mouse_y >= RULES_DESCRIPTION1_Y && mouse_y < RULES_DESCRIPTION1_Y + RULES_DESCRIPTION1_HEIGHT) ||
        (mouse_x >= RULES_DESCRIPTION2_X && mouse_x < RULES_DESCRIPTION2_X + RULES_DESCRIPTION2_WIDTH &&
        mouse_y >= RULES_DESCRIPTION2_Y && mouse_y < RULES_DESCRIPTION2_Y + RULES_DESCRIPTION2_HEIGHT) ||
        (mouse_x >= RULES_DESCRIPTION3_X && mouse_x < RULES_DESCRIPTION3_X + RULES_DESCRIPTION3_WIDTH &&
        mouse_y >= RULES_DESCRIPTION3_Y && mouse_y < RULES_DESCRIPTION3_Y + RULES_DESCRIPTION3_HEIGHT) ||
        (mouse_x >= RULES_DESCRIPTION4_X && mouse_x < RULES_DESCRIPTION4_X + RULES_DESCRIPTION4_WIDTH &&
        mouse_y >= RULES_DESCRIPTION4_Y && mouse_y < RULES_DESCRIPTION4_Y + RULES_DESCRIPTION4_HEIGHT) ||
        (mouse_x >= RULES_DESCRIPTION5_X && mouse_x < RULES_DESCRIPTION5_X + RULES_DESCRIPTION5_WIDTH &&
        mouse_y >= RULES_DESCRIPTION5_Y && mouse_y < RULES_DESCRIPTION5_Y + RULES_DESCRIPTION5_HEIGHT) ||
        (mouse_x >= RULES_DESCRIPTION6_X && mouse_x < RULES_DESCRIPTION6_X + RULES_DESCRIPTION6_WIDTH &&
        mouse_y >= RULES_DESCRIPTION6_Y && mouse_y < RULES_DESCRIPTION6_Y + RULES_DESCRIPTION6_HEIGHT)) && rules_is_on)
        draw_rules_description();
}

void (update_cursor)(struct packet pp) {
    erase_cursor();

    if ((mouse_x + pp.delta_x) < 0)
        mouse_x = 0;
    else if ((mouse_x + cursor_info.width + pp.delta_x) >= MENU_WIDTH)
        mouse_x = MENU_WIDTH - cursor_info.width - 1;
    else
        mouse_x += pp.delta_x;

    if ((mouse_y - pp.delta_y) < 0)
        mouse_y = 0;
    else if ((mouse_y + cursor_info.height - pp.delta_y) >= MENU_HEIGHT)
        mouse_y = MENU_HEIGHT - cursor_info.height - 1;
    else
        mouse_y -= pp.delta_y;

    check_button(pp.lb);
    draw_cursor();
}

void (mouse_interrupt_handler)() {
    mouse_int_handler();
    if (packet_index == 3) {
        struct packet pp;
        parse_mouse_packet(&pp);
        update_cursor(pp);
        packet_index = 0;
    }        
}
