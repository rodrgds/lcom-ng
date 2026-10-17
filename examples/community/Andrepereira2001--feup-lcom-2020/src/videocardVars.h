#ifndef _LCOM_VIDEOCARDVARS_H
#define _LCOM_VIDEOCARDVARS_H

#include <lcom/lcf.h>

/** @defgroup videocardVars
 * @{
 *
 * Constants for programming the video card. 
 */

#define VIDEO_CARD           0x10 /**< @brief - iditenfication of video card to make call with reg86*/

// AX -> 16bits     AH -> 8bits        
#define AX_VBE_MODE_INFO     0x4F01     /**< @brief - used in call of reg86 to get mode info*/
#define AX_SET_VBE_MODE      0x4F02     /**< @brief - used in call of reg86 to set mode info*/    
#define AX_VBE_CONTROL_INFO  0x4F00     /**< @brief - used in call of reg86 to get control info*/

/* video grafic valid modes*/
#define MODE1                0x105      /**< @brief - mode 0x105*/
#define MODE2                0x110      /**< @brief - mode 0x110*/
#define MODE3                0x115      /**< @brief - mode 0x115*/
#define MODE4                0x11A      /**< @brief - mode 0x11A*/
#define MODE5                0x14C      /**< @brief - mode 0x14C*/

#define FIVE_BITES           0x1f       /**< @brief - mode set to 1 five followed bits 0001 1111*/
#define SIX_BITES            0x3f       /**< @brief - mode set to 1 six followed bits 0011 1111*/

#endif
