#ifndef _LCOM_BANNER_H_
#define _LCOM_BANNER_H_

#include <lcom/lcf.h>
#include "../../drivers/drivers.h"
#include "../font/font.h"

/** 
 * @defgroup banner Banner 
 * @ingroup models
 * @{
*/

/**
 * @brief Structure representing a banner that displays a message for a specified timeout period.
 */
typedef struct {
    char        *msg;              /**< The message to be displayed in the banner. */
    uint32_t    timeout;           /**< The duration for which the message should be displayed. */
    uint32_t    current_timeout;   /**< The current elapsed time since the message started displaying. */
} banner_t;

/**
 * @brief Initializes a banner.
 * 
 * @param banner Pointer to the banner to be initialized.
 * @return 0 on success, 1 on failure (if the banner pointer is NULL).
 */
int	banner_init(banner_t *banner);

/**
 * @brief Sets a message for the banner to display with a specified timeout.
 * 
 * @param banner Pointer to the banner.
 * @param msg The message to be displayed.
 * @param timeout The duration for which the message should be displayed.
 * @return 0 on success, 1 on failure (if the banner or message pointer is NULL).
 */
int	banner_set_message(banner_t *banner, char *msg, uint32_t timeout);

/**
 * @brief Updates the banner's timeout, checking if the display duration has elapsed.
 * 
 * @param banner Pointer to the banner.
 * @return 0 on success, 1 on failure (if the banner or message pointer is NULL).
 */
int	banner_update_timeout(banner_t *banner);

/**
 * @brief Draws the banner with the current message using the specified font.
 * 
 * @param banner Pointer to the banner.
 * @param font Pointer to the font to be used for drawing the message.
 * @return 0 on success, 1 on failure (if the banner, message, or font pointer is NULL).
 */
int	banner_draw(banner_t *banner, font_t *font);

/** @} */

#endif /* _LCOM_EV_BANNER_H_ */
