#ifndef _VBE_H_
#define _VBE_H_

/** @defgroup VBE VBE
 * @{
 *
 * Constants for using VBE
 */

#define VBE_INTNO 0x10  /**< @brief Software interrupt instruction */
#define VBE_REG_AH 0x4F /**< @brief AH register value */

/* VBE return codes */

#define VBE_FUNCTION_SUPPORTED_AL 0x4F   /**< @brief Value of the AL register if the function called is supported */
#define VBE_FUNCTION_SUCCESS_AH 0x00     /**< @brief Value of the AH register if the function completed successfully */
#define VBE_FUNCTION_FAILED_AH 0x01      /**< @brief Value of the AH register if the function call failed */
#define VBE_FUNCTION_UNSUPPORTED_AH 0x02 /**< @brief Value of the AH register if the function is not supported in current HW configuration */
#define VBE_FUNCTION_INVALID_AH 0x03     /**< @brief Value of the AH register if the function is invalid in current video mode */

/* VBE functions */

#define VBE_RETURN_CONTROLLER_INFO 0x00 /**< @brief VBE function to return the capabilites of the controller - must provide address for returned info in registers ES:DI */
#define VBE_RETURN_MODE_INFO 0x01       /**< @brief VBE function to return VBE mode information - mode is passed on CX and must provide address for returned info in registers ES:DI */
#define VBE_SET_MODE 0x02               /**< @brief VBE function to initialize the controller and set a video mode - mode is passed in register BX, which should have bit 14 set */
#define VBE_SET_GET_DISPLAY_START 0x07  /**< @brief VBE function to set/get pixel to be displayed in the upper left corner of the display */
#define VBE_SET_DISPLAY_START 0x00      /**< @brief VBE function to set the pixel to be displayed in the upper left corner of the display */
#define VBE_GET_DISPLAY_START 0x01      /**< @brief VBE function to get the pixel to be displayed in the upper left corner of the display */

/* Other */

#define VBE_SET_LINEAR_MODE BIT(14)     /**< @brief Set the linear frame buffer mode */
#define VBE_RETURN_TO_TEXT_MODE_AH 0x00 /**< @brief Value to set the AH register to return to text mode */
#define VBE_RETURN_TO_TEXT_MODE_AL 0x03 /**< @brief Value to set the AL register to return to text mode */
#define VBE_INDEXED_COLOR_MODE 0x105    /**< @brief Indexed color mode */
#define VBE_RESERVED 0x00               /**< @brief Reserved fields must be 00h */

/**@}*/

#endif /* _VBE_H_ */
