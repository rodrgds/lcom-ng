/**
 * @file vbe
 * @brief This file has all defines related to the video graphics.
 */

#ifndef _VBE_H_
#define _VBE_H_

/** @brief VBE mode 1 - 1024x768 resolution, Indexed color depth of 8 */
#define MODE1   0x105	
/** @brief VBE mode 2 - 640x480 resolution, Direct color depth of 15 (1:5:5:5) */
#define MODE2   0x110	
/** @brief VBE mode 3 - 800x600 resolution, Direct color depth of 24 (8:8:8) */
#define MODE3   0x115	
/** @brief VBE mode 4 - 1280x1024 resolution, Direct color depth of 16 (5:6:5) */
#define MODE4   0x11A	
/** @brief VBE mode 5 - 1152x864 resolution, Direct color depth of 32 (8:8:8:8) */
#define MODE5   0x14C	

/** @brief BIOS interrupt call number */
#define INT_10       0x10

/** @brief VBE function write code */
#define WRT_FUNC 0x4F

/** @brief VBE function code for getting current video mode */
#define VBE_GET_MODE 0x01    
/** @brief VBE function code for setting a video mode */
#define VBE_SET_MODE 0x02

/** @brief Bit mask for linear frame buffer mode */
#define LINEAR_FRAME_BUFFER BIT(14)

/** @brief VBE function support check return value */
#define FUNC_SUPPORTED 0x4F
/** @brief VBE function successful execution return value */
#define FUNC_SUCCESS   0x00

/** @brief Color code for transparency */
#define TRANSPARENT 0xFF00FF
/** @brief Pixel width of a letter */
#define LETTER_WIDTH 76

#endif
