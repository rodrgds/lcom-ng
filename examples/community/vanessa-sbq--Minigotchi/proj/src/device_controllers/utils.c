#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Returns the LSB of a 2 byte integer.
 * @param val	input 2 byte integer
 * @param lsb	address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    if (lsb == NULL){
        panic("\nError while executing func %s, error in util_get_LSB, found a NULL pointer.\n", __func__);
        return 1; 
    }
    (*lsb) = val & ((uint16_t)0x00FF);
    return 0;
}

/**
 * @brief Returns the MSB of a 2 byte integer.
 * @param val input 2 byte integer
 * @param msb address of memory location to be updated with val's MSB ? ... é como está online...
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if (msb == NULL){
        panic("\nError while executing func %s, error in util_get_MSB, found a NULL pointer.\n", __func__);
        return 1; 
    }
    (*msb) = (val & ((uint16_t)0xFF00)) >> 8;
    return 0;
}

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 * @param port the input port that is to be read
 * @param value address of 8-bit variable to be update with the value read 
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value) {
    if (value == NULL){
        panic("\nError while executing func %s, error in util_sys_inb, found a NULL pointer.\n", __func__);
        return 1; 
    }
    uint32_t dword;
    if (sys_inb(port, &dword) != 0){
        panic("\nError while executing func %s, error while calling sys_inb.\n", __func__);
        return 1;
    }
    *value = (uint8_t)(dword);
    return 0;
}
