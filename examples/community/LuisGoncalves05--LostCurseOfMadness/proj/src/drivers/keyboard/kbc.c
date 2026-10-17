#include "kbc.h"

int kbc_write_byte(uint8_t port, uint8_t byte) {
    uint8_t attempts = 0;

    while (attempts < READ_ATTEMPT_LIMIT) {
        uint8_t status;
        if (util_sys_inb(KBC_STATUS_REGISTER, &status)) {
            printf("Failed to read KBC status register.\n");
            return 1;
        }

        // Check for timeout or parity errors and if input buffer is empty
        if ((status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT)) == 0 &&
            (status & KBC_INPUT_BUFFER_FULL) == 0) {
            if (sys_outb(port, byte)) {
                printf("Failed to write byte 0x%X to port 0x%X.\n", byte, port);
                return 1;
            }
            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
        attempts++;
    }

    printf("Timed out while trying to write byte 0x%X to port 0x%X.\n", byte, port);
    return 1;
}

int kbc_write_command(uint8_t command) {
    return kbc_write_byte(KBC_COMMAND_REGISTER, command);
}

int kbc_write_command_arguments(uint8_t argument) {
    return kbc_write_byte(KBC_ARGUMENT_REGISTER, argument);
}

int kbc_read_byte(uint8_t port, uint8_t *byte, bool mouse) {
    if (byte == NULL) {
        printf("Invalid argument: byte is NULL.\n");
        return 1;
    }

    uint8_t attempts = 0;

    while (attempts < READ_ATTEMPT_LIMIT) {
        uint8_t status;

        if (util_sys_inb(KBC_STATUS_REGISTER, &status)) {
            printf("Failed to read KBC status register.");
            return 1;
        }

        // Check for timeout or parity errors and if output buffer is full
        if ((status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT)) == 0 &&
            (status & KBC_OUTPUT_BUFFER_FULL)) {

            // Check if request asks for the data from the same device it comes from
            if ((mouse && !(status & KBC_AUX_BIT)) || (!mouse && (status & KBC_AUX_BIT))) {
                printf("Device mismatch: expected %s, got %s.\n",
                       mouse ? "mouse" : "keyboard",
                       mouse ? "keyboard" : "mouse");
                return 1;
            }

            if (util_sys_inb(port, byte)) {
                printf("Failed to read byte from port 0x%X.\n", port);
                return 1;
            }

            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
        attempts++;
    }

    printf("Timed out while trying to read from port 0x%X.\n", port);
    return 1;
}

int kbc_read_command_return(uint8_t *retv, bool mouse) {
    return kbc_read_byte(KBC_RETURN_REGISTER, retv, mouse);
}
