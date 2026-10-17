#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>
/** @defgroup macros macros
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

// Bios Calls
#define VIDEO_CARD 0x10 /**< @brief Video Card Bios Call Macro */
#define PC_CONFIG  0x11 /**< @brief PC Config Bios Call Macro*/
#define MEMORY_CONFIG 0x12 /**< @brief Memory Config Bios Call  Macro */
#define KEYBOARD 0x16 /**< @brief Keyboard Bios Call Macro*/

// Accessing the VBE functions

#define func_call_failed 0x01 /**< @brief VBE function acess failed Macro*/
#define func_not_sup 0x02 /**< @brief  VBE function not supported Macro*/
#define func_invalid_mode 0x03 /**< @brief VBE function invalid mode MACRO*/

// Setting the graphics mode

#define RES_1024x768 0x105 /**< @brief Set Graphics mode to 0x105 1024x768	 INDEXED */
#define RES_640x480 0x110 /**< @brief Set Graphics mode to 0x110 640x480 DIRECT COLOR */
#define RES_800x600 0x115 /**< @brief Set Graphics mode to 0x 115 800x600 DIRECT COLOR */
#define RES_1280x1024 0x11A /**< @brief Set Graphics mode to 0x11A 1280x1024 DIRECT COLOR */
#define RES_1152x864 0x14C /**< @brief Set Graphics mode to 0x14C 1152x864 DIRECT COLOR */