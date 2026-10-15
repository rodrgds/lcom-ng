#ifndef CURSOR_VIEWER_H
#define CURSOR_VIEWER_H

#include "cursor.h"

/**
 * @brief Draws the cursor sprite at the cursor position.
 * @param cursor Cursor model to draw. NULL is accepted.
 */
void cursor_viewer_draw(const Cursor *cursor);

#endif /* CURSOR_VIEWER_H */
