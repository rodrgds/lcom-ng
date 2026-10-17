#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <lcom/lab3.h> // for int handler otherwise it breaks idk why

#define ESC_BREAK_CODE    0x81 // 0x81 is the break code of the ESC key
#define TWO_BYTE_CODE     0xE0 // 0xE0 is the first byte of a two byte code

//! add here the break or/and make code of the keys used in the project

#define UP_ARROW_MAKE_CODE        0x48  /**< @brief Up Arrow Make Code */
#define UP_ARROW_BREAK_CODE       0xC8  /**< @brief Up Arrow Break Code */
#define DOWN_ARROW_MAKE_CODE      0x50  /**< @brief Down Arrow Make Code */
#define DOWN_ARROW_BREAK_CODE     0xD0  /**< @brief Down Arrow Break Code */
#define LEFT_ARROW_MAKE_CODE      0x4B  /**< @brief Left Arrow Make Code */
#define LEFT_ARROW_BREAK_CODE     0xCB  /**< @brief Left Arrow Break Code */
#define RIGHT_ARROW_MAKE_CODE     0x4D  /**< @brief Right Arrow Make Code */
#define RIGHT_ARROW_BREAK_CODE    0xCD  /**< @brief Right Arrow Break Code */

#define ENTER_MAKE_CODE           0x1C  /**< @brief Enter Make Code */
#define ENTER_BREAK_CODE          0x9C  /**< @brief Enter Break Code */
#define SPACE_MAKE_CODE           0x39  /**< @brief Space Bar Make Code */

#define Q_MAKE_CODE               0x10  /**< @brief Q Make Code */
#define Q_BREAK_CODE              0x90  /**< @brief Q Break Code */
#define W_MAKE_CODE               0x11  /**< @brief W Make Code */
#define W_BREAK_CODE              0x91  /**< @brief W Break Code */
#define A_MAKE_CODE               0x1E  /**< @brief A Make Code */
#define A_BREAK_CODE              0x9E  /**< @brief A Break Code */
#define S_MAKE_CODE               0x1F  /**< @brief S Make Code */
#define S_BREAK_CODE              0x9F  /**< @brief S Break Code */
#define D_MAKE_CODE               0x20  /**< @brief D Make Code */
#define D_BREAK_CODE              0xA0  /**< @brief D Break Code */

static uint8_t scancode;

/**
 * @brief Subscribes and enables keyboard interrupts
 * 
 * @param bit_no Bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_int_handler)();

/**
 * @brief Enables keyboard interrupts (used if using polling, maybe not needed for the project)
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_enable_int)();

/**
 * @brief Reads the scancode from the output buffer
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_read_scancode)();

/**
 * @brief Returns the scancode
 * 
 * @return scancode
 */
uint8_t (get_scancode)();

#endif // _LCOM_KEYBOARD_H_
