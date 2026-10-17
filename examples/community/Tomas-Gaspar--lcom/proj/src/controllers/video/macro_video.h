#ifndef _MACRO_VIDEO_H_
#define _MACRO_VIDEO_H_


/**
 * @ingroup Device_Drivers
 * 
 * @defgroup video Video
 * 
 * @brief Device driver module for the graphics card.
 *
 * This module provides the necessary functionality to initialize the graphics card, draw elements on screen and also use double buffering.
 * @{
*/

#define SET_VBE_MODE 0x4F02     /**< @brief VBE mode for the graphics card to be initialized in*/
#define INDEXED_MODE 0x105      /**< @brief code for the indexed mode*/

#define WHITE 0xffffff          /**< @brief white color code*/
#define BLACK 0x000000          /**< @brief black color code*/

/** }@ */

#endif
