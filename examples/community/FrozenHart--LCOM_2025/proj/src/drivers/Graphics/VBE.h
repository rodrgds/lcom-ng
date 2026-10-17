#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/** @defgroup vbe
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller.
 */

// Standard BIOS services Interrupt vectores
#define VIDEO_CARD 0x10;
#define PC_CONFIG 0x11;
#define MEMORY_CONFIG 0x12;
#define KEYBOARD 0x16;

typedef struct {
  uint8_t comp[3];
} rgb_8_8_8_t;

typedef struct {
  uint8_t comp[4];
} rgb_8_8_8_8_t;

/**@}*/

#endif /* _LCOM_VBE_H */
