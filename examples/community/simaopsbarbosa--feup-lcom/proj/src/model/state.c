#include "state.h"
#include "../controllers/KBC/mouse.h"

#define MOUSE_SENSITIVITY 1

extern uint8_t scancode;
extern uint8_t byte_index;
extern vbe_mode_info_t mode_info;
extern struct packet pp;
extern uint8_t current_mouse_byte_index;
extern int cursor_x, cursor_y;
extern bool new_packet;
char last_mouse_button[32] = "";

GameState gameState = RUNNING;

int timer_interrupts = 0;

char last_key_pressed[16];

void update_timer_state() {
    swap_buffers();
    timer_interrupts++;
}

void update_keyboard_state() {
    (kbc_ih)();
    char c = 0;
    switch (scancode) {
        case ESC_KEY:
            gameState = EXIT;
            break;
        default:
            c = scancode_to_char(scancode);
            break;
    }
}

void update_mouse_state() {
    mouse_ih();
    mouse_sync();
}

void update_mouse_state_menus() {

    if (!new_packet) return;
    
    int16_t scaled_dx = (int16_t) (pp.delta_x * MOUSE_SENSITIVITY);
    int16_t scaled_dy = (int16_t) (pp.delta_y * MOUSE_SENSITIVITY);
    
    if (scaled_dx == 0 && scaled_dy == 0) {
        return;
    }

    cursor_x += scaled_dx;
    cursor_y -= scaled_dy;
    
    if (cursor_x < 0) cursor_x = 0;
    if (cursor_y < 0) cursor_y = 0;
    if (cursor_x + 16 >= mode_info.XResolution) cursor_x = mode_info.XResolution - 16;
    if (cursor_y + 27 >= mode_info.YResolution) cursor_y = mode_info.YResolution - 27;
}
