#ifndef _LCOM_BIOS_H_
#define _LCOM_BIOS_H_

#include <lcom/lcf.h>

/** @defgroup bios bios
 * @{
 * 
 * Constants for the PC's video card.
 */

#define BIOS_INT       0x10    /**< @brief Interrupt number used by the BIOS' video services */
#define VBE_FUNC_CALL  0x4F    /**< @brief VBE function call (value for AH register) */
#define VBE_LINEAR_BUF BIT(14) /**< @brief Set bit 14: linear frame buffer */

/* VBE Success Codes */

#define VBE_FUNC_SUP  0x4F /**< @brief VBE function is supported (value in AL register) */
#define VBE_FUNC_OK   0x00 /**< @brief VBE function completed successfully (value in AH register) */

/* VBE Unsuccessful Call Return Codes */

#define VBE_FUNC_FAIL 0x01 /**< @brief Function call failed */
#define VBE_FUNC_HW   0x02 /**< @brief Function is not supported in current HW configuration */
#define VBE_FUNC_INV  0x03 /**< @brief Function is invalid in current video mode */

/* VBE Function Codes */

#define VBE_MODE_INFO 0x01 /**< @brief Return VBE mode information */
#define VBE_SET_MODE  0x02 /**< @brief Set VBE mode */

/**@}*/

#endif /* _LCOM_BIOS_H */
