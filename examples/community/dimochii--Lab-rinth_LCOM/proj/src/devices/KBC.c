#include "KBC.h"

/** @defgroup KBC KBC
 * @{
 *
 * Functions for the Keyboard controller.
 */

/**
 * @brief Writes a command to the keyboard controller.
 * 
 * Writes a command to the specified port of the keyboard controller.
 * 
 * @param port The port to which the command will be written.
 * @param command The command to be written.
 * @return int 0 upon success, 1 upon failure
 */
int (write_kbc)(uint8_t port, uint8_t command) {
    uint8_t status;
    int attemps_count = 10;

    while (attemps_count) {
        if (util_sys_inb(STATUS_REG, &status) != 0) {
            printf("Error reading status from KBC\n");
            return 1;
        }
        
        if (!(status & IBF)) {
            if (sys_outb(port, command) != 0) {
                printf("Error writing command to port\n");
                return 1;
            }

            return 0;
        }

        tickdelay(micros_to_ticks(DELAY));
        attemps_count--;
    }

    return 1;
}

/**
 * @brief Reads a byte from the keyboard controller.
 * 
 * Reads a byte from the specified port of the keyboard controller, verifying its status.
 * 
 * @param port The port from which the byte will be read.
 * @param byte Pointer to the variable where the byte read will be stored.
 * @param mouse_aux Flag indicating whether the byte represents mouse data (non-zero) or keyboard data (zero).
 * @return int 0 upon success, 1 upon failure
 */
int (read_kbc)(uint8_t port, uint8_t *byte, uint8_t mouse_aux) {
    uint8_t status;
    int attemps_count = 10;

    while (attemps_count) {
        if (util_sys_inb(STATUS_REG, &status) != 0) {
            printf("Error reading status from KBC\n");
            return 1;
        }

        if (status & OBF) {
            if (util_sys_inb(port, byte) != 0){   
                printf("Error reading byte from port\n");
                return 1;
            }

            if (status & PARITY) {
                printf("Parity error\n");
                return 1;
            }
            if (status & TIMEOUT) {
                printf("Timeout error\n");
                return 1;
            }
            if ((status & AUX) & !mouse_aux) {
                printf("Not keyboard data - error\n");
                return 1;
            }
            if (!(status & AUX) & mouse_aux) {
                printf("Not mouse data - error\n");
                return 1;
            }

            return 0; 
        }

        tickdelay(micros_to_ticks(DELAY));
        attemps_count--;
    }

    return 1;
}
