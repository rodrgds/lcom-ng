#ifndef CURSOR_CONTROLLER_H
#define CURSOR_CONTROLLER_H

#include "../../engine/event/event_queue.h"
#include "cursor.h"

/**
 * @brief Applies mouse events to cursor state.
 * @param cursor Cursor state to update. NULL is accepted.
 * @param event Event to process. NULL is accepted.
 */
void cursor_controller_handle_event(Cursor *cursor, const GameEvent *event);

#endif /* CURSOR_CONTROLLER_H */
