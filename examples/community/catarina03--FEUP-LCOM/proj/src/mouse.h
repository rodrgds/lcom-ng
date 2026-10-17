
#include <lcom/lcf.h>
#include "i8042.h"

extern int hook_mouse;
extern uint8_t byte_mouse;
extern bool status_error_mouse;
extern uint8_t ack_byte;
extern struct mouse_state machine_state;


/**
 * @brief Subscribes mouse interrupts.
 * 
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts.
 * 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Handles mouse interrupts.
 * 
 */
void (mouse_ih)();

/**
 * @brief Function implemented in lab4.
 *
 */
int kbc_issue_arg(uint8_t cmd);

/**
 * @brief Function implemented in lab4.
 *
 */
int kbc_issue_command(uint8_t cmd);

/**
 * @brief Function implemented in lab4.
 * 
 */
int returnvalue_mouse(uint8_t *ret);

/**
 * @brief Function implemented in lab4.
 * 
 */
int issue_cmd_mouse(uint8_t mouse_cmd, uint8_t *ack_byte);

/**
 * @brief Function implemented in lab4.
 * 
 */
int16_t convert(uint8_t delta, bool sign);

/**
 * @brief Function implemented in lab4.
 * 
 */
struct mouse_ev* (mouse_detect_ev)(struct packet *pp);


