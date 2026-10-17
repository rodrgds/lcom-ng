#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define MAX_TRIES 15

/*!
 * @brief Reads the status register of the KBC.
 * 
 * @param[out] st Pointer to a variable where the status will be stored.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kBCStatus)(uint8_t *st);

/*!
 * @brief Reads a keyboard input from the KBC.
 * 
 * Tries to read a keyboard input from the KBC out buffer.
 * 
 * @param[out] out Pointer to a variable where the read value will be stored.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kBCReadKeyboard)(uint8_t *out);

/*!
 * @brief Reads a mouse input from the KBC.
 * 
 * Tries to read a mouse input from the KBC out buffer.
 * 
 * @param[out] out Pointer to a variable where the read value will be stored.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kBCReadMouse)(uint8_t *out);


/*!
 * @brief Sends a command to the KBC.
 * 
 * Tries to writes a command to the KBC input buffer at 0x64.
 * 
 * @param[in] cmd The command to be sent.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kBCCommand)(uint8_t cmd);

/*!
 * @brief Sends a command argument to the KBC.
 * 
 * Tries to writes a command argument to the KBC input buffer at 0x60. 
 * 
 * @param[in] arg The argument to be sent.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (kBCArgument)(uint8_t arg);

#endif
