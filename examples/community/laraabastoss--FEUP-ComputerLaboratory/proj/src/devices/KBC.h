#ifndef _LCOM_KBC_MOUSE_H_
#define _LCOM_KBC_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/// @brief writes a command to the KBC
/// @param port egister to write information to
/// @param command command sent
/// @return  0 in case of success and 1 otherwise
int (write_command_KBC)(uint8_t port, uint8_t command);

/// @brief reads the output that is currently in the KBC
/// @param port register to read information from
/// @param output variable tha will be set with the rea output
/// @param mouse boolean variable that indicates if we want to read information from the KBC or the mouse
/// @return 0 in case of success and 1 otherwise
int (get_output_KBC)(uint8_t port, uint8_t *output, uint8_t mouse);

#endif
