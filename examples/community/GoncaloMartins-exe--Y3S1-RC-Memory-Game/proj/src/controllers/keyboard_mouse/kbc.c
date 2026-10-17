#include "../utils.h"

/**
 * @brief Reads the Keyboard Controller (KBC) status register.
 * 
 * @param[out] status Pointer to store the status byte.
 * @return 0 if successful, 1 if an error occurs.
 */
int (kbc_read_status)(uint8_t *status) {
    return util_sys_inb(KBC_STATUS_REG, status);
}
/**
 * @brief Reads a byte from the KBC output buffer.
 * 
 * @param port Port to read from (usually OUT_BUF).
 * @param[out] data Pointer to store the read byte.
 * @param is_mouse Set to 1 if expecting mouse data, 0 if keyboard data.
 * 
 * @return 0 on success, 1 on failure or timeout.
 * 
 * Reads the KBC status repeatedly until data is available or max retries are exhausted.
 * Validates data source and checks for parity and timeout errors.
 */
int (kbc_read_output)(uint8_t port, uint8_t *data, uint8_t is_mouse) {
    uint8_t status;
    int retries = MAX_ATTEMPS;
    
    // Loop until retries are used or data is read
    while (retries--) {
        // Read KBC status
        if (kbc_read_status(&status)) {
            printf("KBC: Status read failed.\n");
            return 1;
        }
        // Check if output buffer has data
        if (status & FULL_OUT_BUFFER) {
            // Read data from the port
            if (util_sys_inb(port, data)) {
                printf("KBC: Output buffer read failed.\n");
                return 1;
            }
            // Check for parity or timeout errors
            if (status & PARITY_ERROR || status & TIMEOUT_ERROR) {
                printf("KBC: Communication error.\n");
                return 1;
            }
            // Verify data source (mouse or keyboard)
            if ((is_mouse && !(status & BIT(5))) || (!is_mouse && (status & BIT(5)))) {
                printf("KBC: Wrong source (expected %s).\n", is_mouse ? "mouse" : "keyboard");
                return 1;
            }

            return 0;  // Data read successfully
        }

        tickdelay(micros_to_ticks(WAIT_KBC)); // Wait before retrying
    }
    printf("KBC: Timeout after max retries.\n");
    return 1; // Timeout error
}

/**
 * @brief Sends a command byte to the specified KBC port.
 * 
 * @param port Port to write the command to (e.g., KBC_CMD_REG or OUT_BUF).
 * @param cmd Command byte to send.
 * 
 * @return 0 if command sent successfully, 1 otherwise.
 * 
 * Retries sending the command until the input buffer is free or max retries are reached.
 */
int (kbc_send_cmd)(uint8_t port, uint8_t cmd) {
    uint8_t status;
    int retries = MAX_ATTEMPS;

    while (retries--) {
        // Read KBC status
        if (kbc_read_status(&status)) {
            printf("KBC: Status read failed.\n");
            return 1;
        }
        
        // Check if input buffer is empty
        if (!(status & FULL_IN_BUFFER)) {
            if (sys_outb(port, cmd)) {
                printf("KBC: Command send failed.\n");
                return 1;
            }
            return 0;   // Command sent successfully
        }
        // Wait before retrying
        tickdelay(micros_to_ticks(WAIT_KBC));
    }

    return 1; // Timeout error
}
