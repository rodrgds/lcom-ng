/**
 * @file vbe
 * @brief Constants for programming the video card
 */

#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

/** @defgroup VBE VBE
 * @{
 *
 * Constants for programming the video card
 */

#define R1024x768_IC    0x105 /**< @brief vbe mode indexed and resolution 1024x768 */
#define R640x480_DC     0x110 /**< @brief vbe mode direct color and resolution 640x480 */
#define R800x600_DC     0x115 /**< @brief vbe mode direct color and resolution 800x600 */
#define R1280x1024_DC   0x11A /**< @brief vbe mode direct color and resolution 1280x1024 */
#define R1152x864_DC    0x14C /**< @brief vbe mode direct color and resolution 1152x864 */

#define LINEAR_MODE     BIT(14) /**< @brief Bit to set the linear mode */

#define INT_INST        0x10 /**< @brief Interrupt instruction value */

#define SET_VBE_MODE    0x4F02 /**< @brief Value to set vbe mode */
#define VBE_MODE_INFO   0x4F01 /**< @brief Value to get the vbe mode information */
#define VBE_CTRLR_INFO  0x4F00 /**< @brief Value to get vbe controller information */

#define SET_VIDEO_MODE  0x00 /**< @brief Function value to set video mode */
#define SET_TEXT_MODE   0x03 /**< @brief Text mode */

#define TRANSPARENCY    0xFF00FF /**< @brief Color code choosed to not be drawn */

/**@}*/

#endif
