#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
/** @defgroup mouse mouse
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**
 * @brief Enumerated type for specifying the States in State Machine
 */
enum mouse_state {
  INIT, /*!< Initial State - First State*/
  DRAW1, /*!< First Line - Second State */
  VERTEX, /*!< Vertex */
  DRAW2 /*!< Final Line - Final State */
  };

extern enum mouse_state state;

/**
 * @brief Mouse handler
 */
void (mouse_ih)(void);

/**
 * @brief Parse the 3 mouse byte, and put it into a struct
 *
 * @param p address of memory that points to a packet
 * @return Return 0 upon success and non-zero otherwise
 */
int (read_packet)(struct packet *p);

/**
 * @brief Subscribes the interruptions of mouse
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the interruptions of mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Write a command in Keyboard Command Register 0x64
 *
 * @param command Command to be written in Keyboard Status Register
 * @return Return 0 upon success and non-zero otherwise
 */
int (write_cmd_command)(uint8_t command);

/**
 * @brief Write a command in Keyboard Output Buffer 0x60
 *
 * @param argument Command to be written in Keyboard Output Buffer
 * @return Return 0 upon success and non-zero otherwise
 */
int (write_cmd_argument)(uint8_t argument);

/**
 * @brief Enable the data report of Mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_enable_data_report)();

/**
 * @brief Disable the data report of Mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_disable_data_report)();

/**
 * @brief Write a command passed as arguments of command 0xD4
 *
 * @param mouse_cmd command passed as arguments of command 0xD4
 * @return Return 0 upon success and non-zero otherwise
 */
int (write_mouse)(uint8_t mouse_cmd);

/**
 * @brief Reset the KBC command byte to Minix's default value
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (minix_default_cmd_byte)();

/**
 * @brief Get the actual mouse event
 *
 * @return Return a pointer to the struct mouse_ev, which contains the type of event that occured in mouse and their displacement
 */
struct mouse_ev * (mouse_event_handler)(struct packet *p);

/**
 * @brief State Machine to read a drawings with mouse
 */
void (state_machine_draw_mouse)(struct mouse_ev *ev, uint8_t x_len, uint8_t tolerance);




#endif /* __MOUSE_H */
