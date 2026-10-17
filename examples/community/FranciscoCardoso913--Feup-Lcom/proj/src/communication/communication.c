#include <lcom/lcf.h>
#include "drivers/serial_port/ser.h"
#include "drivers/kbc/keyboard.h"
#include "communication.h"

void send_keyboard_key(keyboard_key key) {
    send_queue_push(KEYBOARD_ID);
    send_queue_push(key);
}

void send_destroyed_rope(uint8_t x, uint8_t y) {
    send_queue_push(ROPE_ID);
    send_queue_push(x);
    send_queue_push(y);
}

void send_start_connection() {
    send_queue_push(CONNECT_ID);
    send_queue_push(CONNECT_START);
}

void send_accept_connection() {
    send_queue_push(CONNECT_ID);
    send_queue_push(CONNECT_ACK);
}

void send_ready_connection() {
    send_queue_push(CONNECT_ID);
    send_queue_push(CONNECT_READY);
}

void send_game_over() {
    send_queue_push(CHANGE_ID);
    send_queue_push(GAME_OVER_ID);
}

void send_resume_game() {
    send_queue_push(CHANGE_ID);
    send_queue_push(RESUME_GAME);
}

void send_restart_game() {
    send_queue_push(CHANGE_ID);
    send_queue_push(RESTART_GAME);
}

void send_exit_game() {
    send_queue_push(CHANGE_ID);
    send_queue_push(EXIT_GAME);
}

void send_next_level() {
    send_queue_push(CHANGE_ID);
    send_queue_push(NEXT_LEVEL);
}

void send_pause_game() {
    send_queue_push(CHANGE_ID);
    send_queue_push(PAUSE_GAME);
}

bool check_connection() {
    while (!receive_queue_is_empty()) {
        printf("Queue Front: %x\n", receive_queue_front());
        if (receive_queue_front() == CONNECT_ID) {
            if (receive_queue_size() < 2) return false;
            receive_queue_pop();
            uint8_t connection = receive_queue_pop();

            printf("Connection byte: %x\n", connection);

            if (connection == CONNECT_START) {
                printf("Received!\n");
                send_accept_connection();
                return true;
            }

            else if (connection == CONNECT_ACK) {
                printf("Acknowledged!\n");
                return true;
            }

            else {
                printf("Unknown connection byte\n");
                return false;
            }
        }

        else
            receive_queue_pop();
    }

    return false;
}

bool check_ready() {
    while (!receive_queue_is_empty()) {
        printf("Queue Front: %x\n", receive_queue_front());
        if (receive_queue_front() == CONNECT_ID) {
            if (receive_queue_size() < 2) return false;
            receive_queue_pop();
            if (receive_queue_pop() == CONNECT_READY) {
                printf("Received 2\n");
                return true;
            }
        }

        else
            receive_queue_pop();
    }

    return false;
}
