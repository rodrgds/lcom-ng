#include "interrupts.h"
int framerate = 30;
bool lb_was_pressed = false;

extern struct packet mouse_packet;

extern Cursor *cursor;
extern TimeSelector *timeSelector;

int subscribe_devices() {
    if (subscribe_timer(&irq_set_timer) != 0) return 1;
    if (subscribe_keyboard(&irq_set_keyboard) != 0) return 1;
    if (subscribe_mouse(&irq_set_mouse) != 0) return 1;
    return 0;
}

int unsubscribe_devices() {
    if (unsubscribe_timer() != 0) return 1;
    if (unsubscribe_keyboard() != 0) return 1;
    if (unsubscribe_mouse() != 0) return 1;
    return 0;
}

void interrupts_handler(fsblkcnt_t msg_int) {
    // Timer interrupt
    if (msg_int & irq_set_timer) {
        timer_ih(); // Low Level

        if (timer_counter % (sys_hz() / framerate) == 0) {
            draw_cursor(cursor);
            draw_buffer();
        }
        if (timer_counter % sys_hz() == 0 && current_state == TYPING) countdown_tick();
    }
    // Keyboard interrupt
    if (msg_int & irq_set_keyboard) {
        keyboard_ih(); // Low Level
        keyboard_handle_input(); // High Level
    }
    // Mouse interrupt
    if (msg_int & irq_set_mouse) {
        mouse_int_h(); // Low Level

        if (current_mouse_byte == 0) { // Only do something when all 3 packets have been processed
            move_cursor(cursor, mouse_packet.delta_x, mouse_packet.delta_y); // High Level
            mouse_handle_input();
        }
    }
}

void keyboard_handle_input() {
    if (scancode[0] == BKEY_ESC) set_state(END);

    switch (current_state) {
        case HOME:
            if (scancode[0] == BKEY_ENTER) {
                set_state(TYPING);
                start_typing_test(get_selected_time(timeSelector));
            }
            if (scancode[0] == BKEY_1) set_time_selector_state(timeSelector, S30);
            if (scancode[0] == BKEY_2) set_time_selector_state(timeSelector, S60);
            if (scancode[0] == BKEY_3) set_time_selector_state(timeSelector, S120);
            break;
        case TYPING:
        {
            if (scancode[0] == BKEY_PERIOD) {
                set_state(PAUSE);
                break;
            }

            const uint8_t num_keys = sizeof(key_map) / sizeof(KeyMapEntry);

            for (uint8_t i = 0; i < num_keys; i++) {
                if (scancode[0] == key_map[i].make) {
                    char c = key_map[i].character;
                    if (c == '.') continue;
                    type_char(c);
                    draw_keyboard_on_change(c, true);
                    break;
                } else if (scancode[0] == key_map[i].break_) {
                    draw_keyboard_on_change(key_map[i].character, false);
                    break;
                }
            }
            break;
        }            
        case PAUSE:
            if (scancode[0] == BKEY_ENTER) {
                set_state(TYPING);
                resume_typing_test();
            }
            if (scancode[0] == BKEY_BACKSPACE) set_state(HOME);
            break;
        case STATS:
            if (scancode[0] == BKEY_ENTER) {
                set_state(TYPING);
                start_typing_test(get_selected_time(timeSelector));
            }
            if (scancode[0] == BKEY_BACKSPACE) set_state(HOME);
            break;
        default:
            break;
    }
}

void mouse_handle_input() {
    // Handle left button press
    if (mouse_packet.lb && !lb_was_pressed) {
        lb_was_pressed = true; // Flag to notify the press was processed
        switch (current_state) {
            case HOME:
                if (on_start_button(cursor)) {
                    set_state(TYPING);
                    start_typing_test(get_selected_time(timeSelector));
                }
                if (on_top_right_button(cursor)) set_state(END);
                if (on_30s_button(cursor)) set_time_selector_state(timeSelector, S30);
                if (on_60s_button(cursor)) set_time_selector_state(timeSelector, S60);
                if (on_120s_button(cursor)) set_time_selector_state(timeSelector, S120);
                break;
            case TYPING:
                if (on_top_right_button(cursor)) {
                    set_state(PAUSE);
                }
                break;
            case PAUSE:
                if (on_continue_button(cursor)) {
                    set_state(TYPING);
                    resume_typing_test();
                }
                if (on_pause_home(cursor)) set_state(HOME);
                if (on_pause_exit(cursor)) set_state(END);
                break;
            case STATS:
                if (on_continue_button(cursor)) {
                    set_state(TYPING);
                    start_typing_test(get_selected_time(timeSelector));
                }
                if (on_top_right_button(cursor)) set_state(HOME);
                break;
            default:
                break;
        }
    } else if (!mouse_packet.lb) {
        lb_was_pressed = false;
    }
}
