#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "src/include/controllers/kbc/MouseInput.h"

extern MouseInput *mouseFormattedInput; /*!< Pointer to last fully read mouse input. */

/*!
 * @brief Subscribes to mouse interrupts.
 * 
 * Sets mouse irq policy with IRQ_REENABLE and IRQ_EXCLUSIVE flags with interruption BIT 3.
 * 
 * @param[out] bit_no Pointer to a variable storing the byte position of the hookID.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouseSubscribeInt)(uint8_t *bit_no);

/*!
 * @brief Unsubscribes from mouse interrupts.
 * 
 * Removes the mouse irq policy previously set with \ref{mouseSubscribeInt}.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouseUnsubscribeInt)();

/*!
 * @brief Handles a mouse interrupt.
 * 
 * Tries to read a mouse input from the kbc and validates it. \n
 * If unable to read a mouse input, it prints 'Unable to read mouse byte.\n'. \n
 * If no input sequence is started, it ignores any input until \ref{MOUSE_CTRL_BYTE} is set and then it starts a new sequence. \n
 * If the current input sequence is finished it is stored in the global variable \ref{mouseFormattedInput}.
 * 
 */
void (mouse_ih)();

/*!
 * @brief Sends a command to the mouse.
 * 
 * Sends a command to the mouse and waits for an ACK response. \n
 * If the mouse does not respond with an ACK, it retries up to MAX_TRIES times.
 * 
 * @param cmd Command to be sent to the mouse.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouseWriteCommand)(uint8_t cmd);

#endif
