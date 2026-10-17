#include <lcom/lcf.h>
#include "macros/iMouse.h"
#include <stdint.h>
#include <stdio.h>

/**
 * @brief subscribes mouse's interrupts
 * @return Returns 0 upon success and non-zero otherwise
 */
int(mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief unsubscribes mouse's interrupts
 * @return Returns 0 upon success and non-zero otherwise
 */
int(mouse_unsubscribe_int)();
/**
 * @brief reads status byte from mouse controller
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_get_status)();
/**
 * @brief after read the out Buffer and the status register, check if there was some communications errors
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_check_status)();
/**
 * @brief fills the mouse_packet
 */
void(parse)();
/**
 * @brief checks is there was no erros and fills the mouse_packet
 */
void(mouse_ih)();
/**
 * @brief resets the mouse
 * @return return 0 upon success and non-zero otherwise
 */
int (mouse_reset)();
/**
 * @brief enables data report from mouse
 * @return return 0 upon success and non-zero otherwise
 */
int (mouse_en_data_report)();
/**
 * @brief disables data report from mouse
 * @return return 0 upon success and non-zero otherwise
 */
int (mouse_dis_data_report)();
/**
 * @brief checks if first line of movement of the mouse is valid
 * @param tolerance max error tolerance
 * @return return 0 upon success and non-zero otherwise
 */
int (mouse_is_valid_first_line_mov)(int tolerance);
/**
 * @brief checks if second line of movement of the mouse is valid
 * @param tolerance max error tolerance
 * @return return 0 upon success and non-zero otherwise
 */
int (mouse_is_valid_second_line_mov)(int tolerance);

