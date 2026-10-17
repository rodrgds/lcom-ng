// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include "../../utils/utils.h"

/** @defgroup vbe vbe
 *  @{
 *
 * Additional constants and functions used for the programming of the VBE
 */

#define VBE_INTERFACE 0x10 /**< @brief interrupt number of the vbe call */

#define VBE_SET_MODE 0x4F02 /**< @brief set vbe mode */

#define LINEAR_MODE BIT(14) /**< @brief linear mode bit */

#define VBE_MODE_800x600_INDEXED 0x103  /**< @brief masked used to set the VBE to a 800x600 indexed mode */
#define VBE_MODE_1024x768_INDEXED 0x105 /**< @brief masked used to set the VBE to a 1024x768 indexed mode */

#define VBE_MODE_1280x1024_FULL_COLOR 0x11B /**< @brief masked used to set the VBE to a 1280x1024 full color mode */

/**
 * @brief Separates a color into their red, green and blue components
 *
 * @param color main color
 * @param r red component
 * @param g green component
 * @param b blue component
 */
void getRGB(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b);

/**
 * @brief Given red, green and blue components, return the result color
 *
 * @param r red component
 * @param g green component
 * @param b blue component
 * @return result color
 */
uint32_t makeRGB(uint8_t r, uint8_t g, uint8_t b);

/**@}*/
