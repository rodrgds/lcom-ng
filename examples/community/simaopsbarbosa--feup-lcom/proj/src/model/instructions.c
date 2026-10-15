#include "instructions.h"

extern uint8_t scancode;
extern GameMode previousGameMode;
extern GameMode gameMode;

bool instructions_button_hovered = false;

void instructions_init() {
    draw_instructions_frame();
}

void keyboard_instructions() {
    switch (scancode) {
        case ENTER_KEY:  // go back
            change_mode(previousGameMode);
            break;
    }
}

void mouse_instructions() {
    update_mouse_state_menus();

    if (cursor_x >= 268 && cursor_x <= 268 + 200 && cursor_y >= 500 && cursor_y <= 500 + 62) {
        instructions_button_hovered = true;
        if (isLeftButtonPressed()) {
            change_mode(previousGameMode);
        }
    } else {
        instructions_button_hovered = false;
    }
}

void instructions_timer_ih() {
    if (gameMode != INSTRUCTIONS_SCREEN) return;
    draw_instructions_frame();
}
