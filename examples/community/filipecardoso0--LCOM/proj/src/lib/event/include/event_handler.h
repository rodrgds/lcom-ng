#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include "../../../drivers/kbd/include/key_keyboard.h"
#include "../../../drivers/mouse/include/mouse.h"
#include "../../state/include/state.h"

/**
 * @brief Handles keyboard events
 * Must add params
 */
void kbd_handler(uint8_t* scancode);

/**
 * @brief Handles mouse events
 * Must add params
 */
void mouse_handler(struct packet *mouse_packet);

/**
 * @brief Handles game events
 * Must add params
 */
void game_handler();

#endif
