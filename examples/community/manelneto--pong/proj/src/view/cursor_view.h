#ifndef _CURSOR_VIEW_H_
#define _CURSOR_VIEW_H_

#include "../model/cursor.h"

/** @defgroup cursor_view cursor_view
 * @{
 *
 * Functions related to the cursor view
 */

/**
 * @brief Cursor drawer
 *
 * @param cursor pointer to the cursor to be drawn
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_cursor(Cursor *cursor);

/**@}*/

#endif /* _CURSOR_VIEW_H_ */
