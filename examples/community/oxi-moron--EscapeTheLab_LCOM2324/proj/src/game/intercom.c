#include "intercom.h"

int (intercom_send_letter) (uint8_t player_no, uint8_t scancode) {
    if (player_no == 1) {
        if (uart_send_data(COM1, scancode) != 0) {
            return 1;
        }

        if (uart_receive_data(COM2, &word[current_index]) != 0) {
            return 1;
        }
        if (word[current_index] == password[current_index]) {
            current_index = (current_index + 1) % PASSWORD_SIZE;
        }
    } else {
        if (uart_send_data(COM2, scancode) != 0) {
            return 1;
        }

        if (uart_receive_data(COM1, &word[current_index]) != 0) {
            return 1;
        }
        if (word[current_index] == password[current_index]) {
            current_index = (current_index + 1) % PASSWORD_SIZE;
        }
    }

    return 0;
}

bool intercom_password_entered() {
    return word[0] == password[0] && word[1] == password[1] && word[2] == password[2] && word[3] == password[3];
}

void (intercom_get_letters) (uint8_t* letters) {
    for (int i = 0; i < PASSWORD_SIZE; i++) {
        letters[i] = word[i];
    }
}

void (intercom_get_password) (uint8_t* pw) {
    for (int i = 0; i < PASSWORD_SIZE; i++) {
        pw[i] = password[i];
    }
}

void (intercom_reset) () {
    for (int i = 0; i < PASSWORD_SIZE; i++) {
        word[i] = 0;
    }
}
