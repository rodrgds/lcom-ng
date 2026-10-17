#ifndef _BUTTON_VIEW_H_
#define _BUTTON_VIEW_H_

#include "../model/button.h"

/** @defgroup button_view button_view
 * @{
 *
 * Functions related to the button view
 */

/**
 * @brief Button drawer
 *
 * @param button pointer to the button to be drawn
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_button(Button *button);

/**@}*/

#endif /* _BUTTON_VIEW_H_ */
