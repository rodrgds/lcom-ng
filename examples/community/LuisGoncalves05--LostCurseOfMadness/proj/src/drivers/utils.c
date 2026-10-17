#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Gets the least significant byte (LSB) of a 16-bit value.
 *
 * @param val The 16-bit value.
 * @param lsb Pointer to store the LSB.
 * @return 0 on success, 1 on failure.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    if (lsb == NULL) {
        printf("util_get_LSB: NULL pointer provided.\n");
        return 1;
    }
    *lsb = val & 0xFF;
    return 0;
}

/**
 * @brief Gets the most significant byte (MSB) of a 16-bit value.
 *
 * @param val The 16-bit value.
 * @param msb Pointer to store the MSB.
 * @return 0 on success, 1 on failure. 
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if (msb == NULL) {
        printf("util_get_MSB: NULL pointer provided.\n");
        return 1;
    }
    *msb = (val >> 8) & 0xFF;
    return 0;
}

/**
 * @brief Reads a byte from a specified port.
 *
 * @param port The port to read from.
 * @param value Pointer to store the read value.
 * @return 0 on success, 1 on failure.
 */
int(util_sys_inb)(int port, uint8_t *value) {
    if (value == NULL) {
        printf("util_sys_inb: NULL pointer provided.\n");
        return 1;
    }
    uint32_t val;
    if (sys_inb(port, &val) != 0) {
        printf("util_sys_inb: sys_inb failed.\n");
        return 1;
    }

    *value = val & 0xFF;
    return 0;
}
