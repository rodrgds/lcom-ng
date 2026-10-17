#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>
#include "kbc.h"
#include "model/colors.h"
#include "i8042.h"

/**
 * @brief Retrieves the byte index.
 *
 * @return The byte index.
 */
int (get_byte_index)();

/**
 * @brief Retrieves the mouse packet.
 *
 * @return Pointer to the mouse packet.
 */
struct packet *(get_mouse_packet)();

/**
 * @brief Subscribes to mouse interrupts.
 *
 * @param bit_no Pointer to store the bit number associated with the mouse interrupts.
 * @return 0 upon success, non-zero otherwise.
 */
int (mouse_subscribe_int)(int *bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Enables or disables data reporting from the mouse.
 *
 * @param enable Set to true to enable data reporting, false to disable.
 * @return 0 upon success, non-zero otherwise.
 */
int (set_data_reporting)(bool enable); 
/**
 * @brief Mouse interrupt handler.
 */
void (mouse_ih)();

/**
 * @brief Parses the mouse packet.
 */
void (parse_mouse_packet)();

/**
 * @brief Updates the mouse location.
 */
void updateMouseLocation();

/**
 * @brief Updates the drawing specifications (color and radius) for the mouse.
 *
 * @param color Pointer to the color value.
 * @param radius Pointer to the radius value.
 */
void updateDrawSpecs(uint32_t *color, int *radius);

#endif /* __MOUSE_H */
