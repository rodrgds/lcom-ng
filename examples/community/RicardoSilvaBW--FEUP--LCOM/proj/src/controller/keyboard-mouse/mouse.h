#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdbool.h>
#include <stdint.h>
#include "kbc.h"
#include "i8042.h"
#include "../video/graphics.h"

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (define_mouse_packet)();

int (write_command_to_mouse)(uint8_t buffer_content);

void (mouse_ih)();

void (process_mouse_interrupt)();

/* Helpers to query current mouse state (position in pixels and button bitmask).
 * Buttons bitmask: 1 = left, 2 = right, 4 = middle
 */
void mouse_get_pos(int *x, int *y);
int mouse_get_buttons();

void (set_mouse_positions)(int x, int y);

#endif /* __MOUSE_H */
