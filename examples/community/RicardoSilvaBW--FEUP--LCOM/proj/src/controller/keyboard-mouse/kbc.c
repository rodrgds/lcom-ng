#include <lcom/lcf.h>
#include <stdint.h>

#include "kbc.h"
#include "i8042.h"

int kb_hook_id = 1;
uint8_t scancode = 0;
extern uint32_t counter;

static bool is_two_byte_scancode = false;
static uint8_t two_byte_scancode[2] = {0, 0};

int (kb_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    *bit_no = BIT(kb_hook_id); 
    if (sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kb_hook_id) != 0) return 1;

    return 0;
}

int (kb_unsubscribe_int)() {
    if (sys_irqrmpolicy(&kb_hook_id) != 0) return 1;

    return 0;
}

int (read_kbc_status)(uint8_t* kbc_status) {
    if (util_sys_inb(STATUS_COMMAND_KBC_PORT, kbc_status) != 0) return 1;

    return 0;
}

int (read_kbc_output)(uint8_t port, bool mouse, uint8_t* buffer_content) {
    uint8_t kbc_status = 0, remaining_tries = MAX_RETRIES, read_value;

    while (remaining_tries > 0) {
        if (read_kbc_status(&kbc_status) != 0) return 1;
        if (kbc_status & KBC_STATUS_CHECK_ERRS || ((kbc_status & KBC_STATUS_AUX_MOUSE) && !mouse) || (!(kbc_status & KBC_STATUS_AUX_MOUSE) && mouse) || !(kbc_status & KBC_STATUS_OUTPUT_BUFFER_FULL)) {
            DELAY_KB;
            remaining_tries--;
            continue;
        }
        else if (util_sys_inb(port, &read_value) != 0) return 1;
        else {
            *buffer_content = read_value;
            return 0;
        }
    }

    return 1;
}

int (write_kbc_input)(uint8_t port, uint8_t buffer_content) {
    uint8_t kbc_status = 0, remaining_tries = MAX_RETRIES;

    while (remaining_tries > 0) {
        if (read_kbc_status(&kbc_status) != 0) return 1;
        if (kbc_status & KBC_STATUS_INPUT_BUFFER_FULL) {
            DELAY_KB;
            remaining_tries--;
            continue;
        }
        else if (sys_outb(port, buffer_content) != 0) return 1;
        else return 0;
    }
    return 1;
}

void (kbc_ih)() {
    uint8_t status;
    if (util_sys_inb(STATUS_COMMAND_KBC_PORT, &status) != 0) return;
    if (!(status & KBC_STATUS_OUTPUT_BUFFER_FULL)) return;
    if (status & KBC_STATUS_CHECK_ERRS) return;
    if (status & KBC_STATUS_AUX_MOUSE) return;
    util_sys_inb(IN_OUT_BUFFER_KBC_PORT, &scancode);
}

int (enable_kbc_interrupts)() {
    uint8_t command;

    if (write_kbc_input(STATUS_COMMAND_KBC_PORT, READ_COMMAND) != 0) return 1;
    if (read_kbc_output(IN_OUT_BUFFER_KBC_PORT, false, &command) != 0) return 1;

    command |= COMMAND_BYTE_INT;

    if (write_kbc_input(STATUS_COMMAND_KBC_PORT, WRITE_COMMAND) != 0) return 1;
    if (write_kbc_input(IN_OUT_BUFFER_KBC_PORT, command) != 0) return 1;

    return 0;
}

int8_t (kbc_get_scan_code)() {
    return scancode;
}

void (process_kb_interrupt)() {
    kbc_ih();
    if (scancode == 0) return;
    if (scancode == SCANCODE_FIRST_BYTE) {
        is_two_byte_scancode = true;
        two_byte_scancode[0] = scancode;
    }
    else if (is_two_byte_scancode) {
        is_two_byte_scancode = false;
        two_byte_scancode[1] = scancode;
        counter = 0;
    }
    else {
        two_byte_scancode[0] = scancode;
        counter = 0;
    }
}
