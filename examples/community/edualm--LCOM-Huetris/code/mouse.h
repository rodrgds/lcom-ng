#ifndef MOUSE_H_
#define MOUSE_H_

#define MOUSE_WRITE_BYTE    0xD4
#define MOUSE_ENABLE        0xF4

/** @defgroup mouse mouse
 * @{
 * Functions for controlling the mouse.
 */

/**
 * @brief Reads from the mouse.
 *
 * @param value The return value.
 * @return 0 on successful execution, undefined if otherwise.
 */

int mouse_read(unsigned long *value);

/**
 * @brief Enables the mouse.
 *
 * @return 0 on successful execution, undefined if otherwise.
 */

int mouse_enable();

/**@}*/

#endif // MOUSE_H_
