#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Calculates the less significant byte from the value given
 * @param val Value to calculate the lsb
 * @param lsb Variable where the lsb will be stored
 * @return int 1 if an error occurs, 0 otherwise
 */
int (util_get_LSB)(uint16_t val, uint8_t* lsb) {
    if (!lsb) {
        printf("Error in util_get_LSB (nullptr)\n");
        return 1;
    }
    *lsb = (uint8_t)val;
    return 0;
}

/**
 * @brief Calculates the most significant byte from the value given
 * @param val Value to calculate the lsb
 * @param lsb Variable where the lsb will be stored
 * @return int 1 if an error occurs, 0 otherwise
 */
int (util_get_MSB)(uint16_t val, uint8_t* msb) {
    if (!msb) {
        printf("Error in util_get_MSB (nullptr)\n");
        return 1;
    }
    *msb = (uint8_t)(val >> 8);
    return 0;
}

/**
 * @brief Reads the value from a port and stores it value
 * @param port Port where the value will be read from
 * @param vale Variable where the value read will be stored
 * @return int 1 if an error occurs, 0 otherwise
 */
int (util_sys_inb)(int port, uint8_t* value) {
    u_int32_t v = (u_int32_t)*value;

    if (sys_inb(port, &v)) {
        printf("Error in sys_inb\n");
        return 1;
    }

    *value = (uint8_t)v;
    return 0;
}
