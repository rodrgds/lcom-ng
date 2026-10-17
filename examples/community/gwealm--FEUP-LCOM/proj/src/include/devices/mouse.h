#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#include "events/events.h"

#define MOUSE_IRQ 12
#define MOUSE_SYNC_MASK BIT(3)
#define MOUSE_PACKET_SIZE 3

#define MOUSE_Y_SIGN BIT(5)
#define MOUSE_X_SIGN BIT(4)
#define MOUSE_BTN_LEFT BIT(0)

struct mouse_packet {
  bool lb;
  int16_t delta_x;
  int16_t delta_y;
};

/**
 * @brief Handles operations to inicialize mouse for usage
 *
 * @return 0 if the operation was successful, non-zero otherwise 
 */
int (mouse_init)(uint16_t screen_width, uint16_t screen_height);

/**
 * @brief Handles operations to terminate mouse usage
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (mouse_cleanup)();

/**
 * @brief Subscribes mouse interrupts
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (mouse_subscribe_interrupts)();

/**
 * @brief Unubscribes mouse interrupts
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (mouse_unsubscribe_interrupts)();

/**
 * @brief Parses a packet sent by the mouse
 * 
 */
void (mouse_parse_packet)(struct mouse_packet *packet, uint8_t *bytes);

/**
 * @brief Handles a mouse interrupt
 * 
 */
void (mouse_handle_interrupt)(struct event *ev);

/**
 * @brief Handles a single mouse packet
 * 
 */
void (mouse_handle_packet)(struct mouse_packet *packet);

/**
 * @brief Enables data reporting from the mouse
 * 
 */
int (mouse_enable_data_reporting_custom)();

/**
 * @brief Disables data reporting from the mouse
 * 
 */
int (mouse_disable_data_reporting)();

/**
 * @brief Enables the mouse to work in stream mode
 * 
 */
int (mouse_enable_stream_mode)();

/**
 * @return The mouse horizontal position
 */
uint16_t (mouse_get_x)();

/**
 * @return The mouse vertical position
 */
uint16_t (mouse_get_y)();

/**
 * @brief Checks if the mouse in inside a rectangle
 * 
 * @param min_x The lower horizontal bound
 * @param min_y The lower vertical bound
 * @param max_x The higher horizontal bound
 * @param max_y The nigher vertical bound
 */
int (mouse_is_in_position)(uint16_t min_x, uint16_t min_y, uint16_t max_x, uint16_t max_y);
