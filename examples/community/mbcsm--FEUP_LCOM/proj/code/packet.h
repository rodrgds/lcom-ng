#pragma once
/** @defgroup packet packet
 * @{
 *
 * utils for packet
 */

/**
 * @brief Clears the packet
 * @param pp pointer to the mouse packet
 */
void resetPacket(struct packet *pp);

/**
 * @brief Parses the mouse information into a more usable struct
 * @param byte
 * @param data mouse data * 
 * @param packet pointer to the mouse packet
 */
void parseToPacket (int byte, uint32_t data, struct packet *pp);

/**@}*/
