#ifndef _LCOM_VC_MACROS_H
#define _LCOM_VC_MACROS_H
#include <lcom/lcf.h>

char * greenChar;          /**< @brief to store green color bytes in rgb565*/
char * redChar;            /**< @brief to store red color bytes in rgb565 */
char * blackChar;          /**< @brief to store black color bytes in rgb565*/

#define VBE                0x4F00
#define SET_VBE_MODE       0x2    /**< @brief Set VBE mode, VBE call, function 02 */
#define GET_VBE_MODE_INFO  0x1    /**< @brief Return VBE Mode Information, VBE call, function 01 */
#define GET_VBE_CTRL_INFO  0x0    /**< @brief Return VBE Controller Info, VBE call,function 00 */

#define SET_LINEAR_MODE   BIT(14)   /**< @brief set bit 14: linear framebuffer*/
#define VC_INT            0x10      /**<@brief BIOS' video services*/

#endif // _LCOM_VC_MACROS_H
