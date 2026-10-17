#include <lcom/lcf.h>
#include "menu.h"

extern uint8_t *double_buffer;
extern uint8_t *menu_buffer;

extern uint8_t scancode[];
extern int scancode_size;

extern struct packet mPacket;
extern struct mouse_position mPosition;

extern main_state_enum main_state;

extern bool mouse_clickable;

static MenuOption options[] = {
    {330, 422, 490, 50},
    {330, 492, 490, 54},
};
static uint8_t number_of_options = 2;
static uint8_t selected = 0;

void (handle_kbd_menu)() {
    switch (scancode[scancode_size -1]) {
        case 0x81: // handle ESC key
            main_state = EXIT;
            break;
        case 0x1C: // handle ENTER key
            switch (selected) {
                case 0:
                    main_state = CHOOSE_PLAYER;
                    break;
                case 1:
                    main_state = EXIT;
                    break;
            }
            break;
            
        default:
            break;
    }

    menu_move_selector(scancode[scancode_size - 1], &selected, number_of_options);
}

void (handle_mouse_menu)() {
    mouse_clickable = false;

    if (mPacket.lb == 1) {
        if (is_point_on_menu_option(options[0], mPosition.x, mPosition.y)) {
            main_state = CHOOSE_PLAYER;
        } 
        else if (is_point_on_menu_option(options[1], mPosition.x, mPosition.y)) {
            main_state = EXIT;
        }
    }

    // Move o seletor com base na posição do mouse
    for (uint8_t i = 0; i < number_of_options; i++) {
        if (is_point_on_menu_option(options[i], mPosition.x, mPosition.y)) {
            mouse_clickable = true;
            selected = i;
        }
    }
}

void (handle_timer_menu)() {
    // Desenha o menu
    memcpy(double_buffer, menu_buffer, XResolution * YResolution * BytesPerPixel);
    // Desenha o retângulo do seletor
    MenuOption selected_option = options[selected];
    draw_menu_option_selector(selected_option.x, selected_option.y, selected_option.width, selected_option.height, false);
}
