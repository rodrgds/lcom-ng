/**
 * @file mouse.h
 * @brief Mouse device interface.
 */
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Subscribes mouse interrupts.
 * @param irq_set Pointer to store the IRQ bitmask.
 * @return 0 upon success, non-zero otherwise.
 */
int project_mouse_subscribe(uint32_t *irq_set);

/**
 * @brief Unsubscribes mouse interrupts.
 * @return 0 upon success, non-zero otherwise.
 */
int project_mouse_unsubscribe(void);

/**
 * @brief Enables mouse data reporting.
 * @return 0 upon success, non-zero otherwise.
 */
int project_mouse_enable_data_reporting(void);

/**
 * @brief Disables mouse data reporting.
 * @return 0 upon success, non-zero otherwise.
 */
int project_mouse_disable_data_reporting(void);

/**
 * @brief Mouse interrupt handler.
 */
void project_mouse_ih(void);

/**
 * @brief Checks if a full mouse packet is ready.
 * @return True if ready, false otherwise.
 */
bool project_mouse_packet_ready(void);

/**
 * @brief Gets the constructed mouse packet.
 * @return Pointer to the array of 3 bytes representing the packet.
 */
uint8_t* project_mouse_get_packet(void);

/**
 * @brief Sets the mouse sample rate.
 * @param rate The desired sample rate.
 * @return 0 upon success, non-zero otherwise.
 */
int project_mouse_set_sample_rate(uint8_t rate);

/**
 * @brief Sets the mouse resolution.
 * @param res The desired resolution.
 * @return 0 upon success, non-zero otherwise.
 */
int project_mouse_set_resolution(uint8_t res);

#endif
