#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>

struct mousePacket {
  uint8_t bytes[3]; // mouse packet raw bytes
  bool rb, mb, lb;  // right, middle and left mouse buttons pressed
  int16_t delta_x;  // mouse x-displacement: rightwards is positive
  int16_t delta_y;  // mouse y-displacement: upwards is positive
  bool x_ov, y_ov;  // mouse x-displacement and y-displacement overflows
  int8_t delta_scroll; // scroll displacement: with a touchpad upwards is postive, with a mouse upwards is negative 
};

typedef struct {
  int16_t x;
  int16_t y;
  int8_t target;
  uint16_t score;
} player2_info_t;


/**
 * @brief Returns the LSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param lsb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Returns the MSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param msb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 *
 * @param port the input port that is to be read
 * @param value a pointer to the variable that is to be update with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value);

#endif /* UTILS_H */
