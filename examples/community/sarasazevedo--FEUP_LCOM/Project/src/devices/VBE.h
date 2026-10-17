/**
 * @file VBE.h
 * 
 * @brief Constants for programming the Videocard
*/

#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/** @defgroup VBE VBE
 * @{
 *
 * @brief Constants for programming the Videocard
 */


#define INT_VIDEO_SERVICES 0x10   /** @brief Interrupt number used by the BIOS' video services */

#define AH_VBE_FUNCTION 0x4f    /** @brief VBE Functions AH */
#define AH_BIOS_FUNCTION 0x00   /** @brief BIOS Set Video Mode AH */
#define AL_SET_VBE_MODE 0x02    /** @brief Set VBE Mode function AL*/
#define SET_DISPLAY_START_CONTROL 0x07 /** @brief Set Display Start Control function AL*/
#define VBE_SET_START_VERTICAL_RETRACE 0x80 /** @brief Set Display Start during Vertical Retrace */


#define INDEXED_SUBMODE 0x105  /** @brief VBE Mode with 1024x768 screen resolution and Indexed color mode*/
#define SUBMODE_110 0x110      /** @brief VBE Mode with 640x480 screen resolution and Direct color mode with 15 bits per pixel*/
#define SUBMODE_115 0x115      /** @brief VBE Mode with 800x600 screen resolution and Direct color mode with 24 bits per pixel*/
#define SUBMODE_11A 0x11A      /** @brief VBE Mode with 1280x1024 screen resolution and Direct color mode with 16 bits per pixel*/
#define SUBMODE_14C 0x14C      /** @brief VBE Mode with 1152x864 screen resolution and Direct color mode with 32 bits per pixel*/

#define LINEAR_MEMORY BIT(14)  /** @brief Set to have Linear Frame Buffer Model */




/**@} end of VBE*/

#endif /* _LCOM_VBE_H */
