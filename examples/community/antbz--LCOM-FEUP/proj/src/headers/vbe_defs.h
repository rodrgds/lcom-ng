#pragma once

/** 
 * @defgroup VBE Macros
 * @{
 * 
 *  Constants for programming the videocard.
 */

#define VIDEOCARD   0x10 /**< @brief Videocard */

#define VBE_FUNC    0x4F /**< @brief VBE function */

#define GET_MODE_INFO 0x01 /**< @brief VBE mode information */
#define SET_VBE_MODE 0x02 /**< @brief Set VBE mode */

#define LINEAR_FB  1<<14 /**< @brief Linear framebuffer */

#define PC_CONFIG   0x11 /**< @brief PC Configuration */
#define MEM_CONFIG  0x12 /**< @brief Memory Configuration */

#define VIDEO_MODE  0x118 /**< @brief Video mode */
#define FRAME_R 30 /**< @brief Frame rate */

/**@}*/
