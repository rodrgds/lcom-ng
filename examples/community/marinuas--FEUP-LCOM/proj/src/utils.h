#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "i8042.h"

/**
 * @brief Retorna o byte menos significativo de um inteiro de 2 bytes.
 * @param val - inteiro de 2 bytes
 * @param lsb - adereço de onde o byte menos significativo ficará
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);
/**
 * @brief Retorna o byte mais significativo de um inteiro de 2 bytes.
 * @param val - inteiro de 2 bytes
 * @param lsb - adereço de onde o byte menos significativo ficará
 * */
int(util_get_MSB)(uint16_t val, uint8_t *msb);
/**
 * @brief Invoca o sys_inb() mas lê o valor num uint8_t
 * @param val - inteiro de 2 bytes
 * @param lsb - adereço de onde o byte menos significativo ficará
 * */
int util_sys_inb(int port, uint8_t *val);
