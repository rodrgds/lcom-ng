#include "dispatcher.h"

int subscribe_devices() {
    
    if (keyboard_subscribe_int(&irq_set_keyboard)) {
        printf("Error subscribing keyboard interrupts\n");
        return 1;
    }

    if (timer_subscribe_int(&irq_set_timer)) {
        printf("Error subscribing timer interrupts\n");
        return 1;
    }

    if (mouse_subscribe_int(&irq_set_mouse)) {
        printf("Error subscribing mouse interrupts\n");
        return 1;
    }
    
    if(rtc_subscribe_int(&irq_set_rtc)) {
        printf("Error subscribing rtc interrupts\n");
        return 1;
    }

    return 0;
}

int unsubscribe_devices() {
    if (timer_unsubscribe_int()) {
        printf("Error unsubscribing timer interrupts\n");
        return 1;
    }

    if (keyboard_unsubscribe_int()) {
        printf("Error unsubscribing keyboard interrupts\n");
        return 1;
    }

    if (mouse_unsubscribe_int()) {
        printf("Error unsubscribing mouse interrupts\n");
        return 1;
    }

    if(rtc_unsubscribe_int()) {
        printf("Error unsubscribing rtc interrupts\n");
        return 1;
    }
    
    return 0;
}

int dispatcher() {
    int ipc_status;
    message msg;
    uint8_t r;

    if (timer_set_frequency(0, 30)) return 1;
    
    if (mouse_write_cmd(MOUSE_EN_DATA_REP)) return 1;

    if (subscribe_devices()) return 1;

    while (state != QUIT) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("Error driver_receive failed with: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        timer_handler();
                    }

                    if (msg.m_notify.interrupts & irq_set_keyboard) {
                        keyboard_handler();
                    }

                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        mouse_handler();
                    }

                    if (msg.m_notify.interrupts & irq_set_rtc) {
                        rtc_handler();
                    }

                    break;
                default:
                    break;
            }
        } else {
            printf("Error received message was not notification\n");
        }
    }

    if (unsubscribe_devices()) return 1;

    if (mouse_write_cmd(MOUSE_DIS_DATA_REP)) return 1;

    return 0;
}

void timer_handler() {
    timer_int_handler();

    if (timer_counter % 30 == 0 && state == GAME) {
        clock_update();
        
        if (clock_timeout()) set_game_over();
    }

    if (is_checkmate()) {
        set_game_over();
    }

    if (draw_background()) return;

    if (draw_menu()) return;
    
    if (draw_board()) return;

    if (draw_selected()) return;

    if (draw_pieces()) return;

    if (mouse_draw_cursor()) return;

    if (vg_copy_buffer()) return;
    
}

void keyboard_handler() {
    keyboard_ih();

    menu_handle_keyboard(scancode);

    if (state == GAME) {
        keyboard_handle_input(scancode);
    }
}

void mouse_handler() {
    mouse_ih();
    mouse_sync_bytes();

    if (byte_index == 3) {
        mouse_parse_packet();
        byte_index = 0;
        mouse_update_position();

        menu_handle_mouse();

        if (state == GAME) {
            if (packet.lb) {
                if (is_selected()) mouse_move_piece(mouse_x, mouse_y);
                else mouse_select_piece(mouse_x, mouse_y);
            }

            if (packet.rb) {
                deselect_piece();
            }
        } 
    }
}

void rtc_handler() {
    rtc_ih();
}
