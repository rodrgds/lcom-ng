#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>
#include <lcom/lab4.h>

#include "../../utils.h"

/**
  * @brief Subscribe mouse interrupts.
  * 
  * @param bit_no Bit number to be set in the mask.
  * @return Return 0 upon success and non-zero otherwise.
  */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
  * @brief Unsubscribe mouse interrupts.
  * 
  * @return Return 0 upon success and non-zero otherwise.
  */
int (mouse_unsubscribe_int)();

/**
  * @brief Check if the mouse packet is done.
  * 
  * @return true if the packet is done, false otherwise.
  */
bool (mouse_packet_is_done)();

/**
  * @brief Get and parse the mouse packet.
  * 
  * @param pp Pointer to the mouse packet.
  */
void (mouse_get_packet)(struct mousePacket *pp);

/**
  * @brief KBC write command.
  * 
  * @param cmd The command to be written.
  * @return 0 upon success and non-zero otherwise.
  */
int (kbc_write_command)(uint8_t cmd);

/**
  * @brief Write byte to mouse command
  * 
  * @param byte The byte to be written.
  * @return 0 upon success and non-zero otherwise.
  */
int (write_byte_to_mouse)(uint8_t byte);

/**
  * @brief Mouse enable data reporting.
  * 
  * @return 0 upon success and non-zero otherwise.
  */
int (mouse_enable_data_reporting_mine)();

/**
  * @brief Mouse disable data reporting.
  * 
  * @return 0 upon success and non-zero otherwise.
  */
int (mouse_disable_data_reporting)();

/**
  * @brief Mouse set stream mode.
  * 
  * @param rate The rate to be set.
  * @return 0 upon success and non-zero otherwise.
  */
int (mouse_set_sample_rate)(uint8_t rate);

/**
  * @brief Write the get device id command to the mouse.
  * 
  * @param id Pointer to the id. The id will be written here.
  * @return 0 upon success and non-zero otherwise.
  */
int (write_get_device_id_to_mouse)(uint8_t* id);

/**
  * @brief Get the mouse id.
  * 
  * @param id Pointer to the id. The id will be written here.
  * @return 0 upon success and non-zero otherwise.
  */
int (get_mouse_id)(uint8_t* id);

/**
  * @brief Enable mouse data reporting.
  * 
  * @return 0 upon success and non-zero otherwise.
  */
int (mouse_enable_scrolling)();

#endif /* MOUSE_H */
