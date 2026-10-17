#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>

#include <stdint.h>
#include "i8042.h"
#include "../../sprites/sprite.h"

typedef struct {
    int16_t byte, x, y;
    int16_t delta_x, delta_y;
    uint8_t bytes[3];
    uint8_t rb, mb, lb, x_ov, y_ov, complete;
} mouse_packet_t;

/**
 * @brief Subscribes to the mouse.
 *
 * @param bit_no Pointer that will be pointed to the mouse hood_id.
 * @return int 1 if an error occurs, 0 otherwise
 */
int mouse_subscribe_int(uint8_t* bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int mouse_unsubscribe_int();

/**
 * @brief Sends a command to the mouse.
 *
 * @param cmd The command to send.
 * @return int 1 if an error occurs, 0 otherwise
 */
int mouse_send_cmd(uint8_t cmd);

/**
 * @brief Reads a byte received from the mouse and updates the packet accordingly.
 *
 * @param b The byte received from the mouse.
 */
void m_read_byte(uint8_t b);

/**
 * @brief Builds the mouse packet from the received bytes.
 */
void mouse_build_packet();

/**
 * @brief Updates the mouse position based on the delta values.
 */
void mouse_update_pos();

/**
 * @brief Mouse interrupt handler. Reads data from the mouse and updates the packet.
 */
void (mouse_ih)();

/**
 * @brief Enables data reporting by the mouse.
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int mouse_enable_dr();

/**
 * @brief Disables data reporting by the mouse.
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int mouse_disable_dr();

/**
 * @brief Sets the mouse to stream mode.
 *
 * @return int 1 if an error occurs, 0 otherwise
 */
int mouse_stream_mode();

/**
 * @brief Checks if the mouse is inside the specified rectangle.
 *
 * @param x      The horizontal of the rectangle.
 * @param y      The vertical of the rectangle.
 * @param width  The width of the rectangle.
 * @param height The height of the rectangle.
 * @return Returns 1 if the mouse is inside the rectangle, 0 otherwise.
 */
int mouse_inside(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Checks if the mouse is over the specified sprite.
 *
 * @param sp The sprite to check.
 * @return Returns 1 if the mouse is over the sprite, 0 otherwise.
 */
int mouse_over_sprite(Sprite* sp);

/**
 * @brief Checks if the left mouse button is clicked while over the specified sprite.
 *
 * @param sp The sprite to check.
 * @return Returns 1 if the left mouse button is clicked while over the sprite, 0 otherwise.
 */
int mouse_lclick_sprite(Sprite* sp);

/**
 * @brief Checks if the right mouse button is clicked while over the specified sprite.
 *
 * @param sp The sprite to check.
 * @return Returns 1 if the right mouse button is clicked while over the sprite, 0 otherwise.
 */
int mouse_rclick_sprite(Sprite* sp);

#endif /* __MOUSE_H */
