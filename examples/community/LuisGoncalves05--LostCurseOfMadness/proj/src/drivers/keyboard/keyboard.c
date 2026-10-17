#include "keyboard.h"

int32_t kbd_hook_id = KBD_IRQ;     /**< Keyboard subscription ID */
uint8_t scan_code;                 /**< Scan code read from keyboard */
bool two_byte = false;             /**< Whether the scan code is a two-byte code */
extern uint32_t interrupt_counter; /**< Timer interrupt counter. */

int(kbd_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL)
        return 1;
    *bit_no = BIT(kbd_hook_id);
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int(kbd_unsubscribe_int)(void) {
    return sys_irqrmpolicy(&kbd_hook_id);
}

void(kbd_int_handler)(void) {
    uint8_t attempts = 0;
    while (attempts < READ_ATTEMPT_LIMIT) {
        if (kbd_read_scan_code() == 0)
            return;
        attempts++;
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

int(kbd_read_scan_code)(void) {
    uint8_t status;

    if (util_sys_inb(KBC_STATUS_REGISTER, &status))
        return 1;

    if (util_sys_inb(KBC_OUTPUT_BUFFER, &scan_code))
        return 1;

    return !(status & KBC_OUTPUT_BUFFER_FULL) || (status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT));
}

bool(kbd_handle_scan_code)() {
    uint8_t received_scancodes;
    if (two_byte == true) {
        received_scancodes = 2;
    } else {
        if (scan_code == TWO_BYTE_SCANCODE)
            two_byte = true;
        received_scancodes = 1;
    }

    bool complete = (received_scancodes == 2 && two_byte) || (received_scancodes == 1 && !two_byte);
    two_byte = false;
    return complete;
}
