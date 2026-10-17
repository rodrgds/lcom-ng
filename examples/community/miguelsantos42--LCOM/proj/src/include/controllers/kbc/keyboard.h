#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "src/include/controllers/kbc/KeyboardInput.h"

extern KeyboardInput *keyboardFormattedInput; /*!< Pointer to last fully read keyboard input. */

/*!
 * @brief Subscribes to keyboard interrupts.
 * 
 * Sets keyboard irq policy with IRQ_REENABLE and IRQ_EXCLUSIVE flags with interruption BIT 1.
 * 
 * @param[out] bit_no Pointer to a variable storing the byte position of the hookID.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (keyboardSubscribeInt)(uint8_t *bit_no);

/*!
 * @brief Unsubscribes from keyboard interrupts.
 * 
 * Removes the keyboard irq policy previously set with \ref{keyboardSubscribeInt}.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (keyboardUnsubscribeInt)();

/*!
 * @brief Reads a keyboard input from the KBC.
 * 
 * Reads a keyboard input from the kbc and validates it. \n
 * The input is read as part of the current sequence. \n
 * If the input is invalid and is part of a sequence, a new sequence is started with the new input as the first input. \n
 * When an input sequence is finished it is stored in the global variable \ref{keyboardFormattedInput} and starts a new sequence.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (readScanCode)();

/*!
 * @brief Handles a keyboard interrupt.
 * 
 * Calls the \ref{readScanCode} function to read a keyboard input from the KBC, prints 'Unable to read scan code.\n' if unsucessfull.
 * 
 */
void (kbc_ih)();

/*!
 * @brief Checks if current scan code sequence is a make code.
 * 
 * returns true if the current scan code sequence is a make code, false otherwise.
 */
bool (isMakeCode)();

/*!
 * @brief Allows keyboard interrupts from KBC.
 * 
 * Tries to read the KBC command byte and sets the INT flag and writes it back to the KBC. \n
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (allowKeyboardInterrupts)();

#endif
