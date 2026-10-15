#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/// @brief Reads the status of the KBC
/// @param status Current KBC status
/// @return Returns 0 on success and 1 otherwise
int (read_KBC_status)(uint8_t* status);

/// @brief Reads the output from the KBC
/// @param port Port to read the command
/// @param output Output from KBC
/// @param mouse Flag that determines if the output received came from mouse or keyboard
/// @return 
int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse);

/// @brief Writes a command to the KBC
/// @param port Port to write the command
/// @param commandByte Command to write
/// @return Returns 0 on success and 1 otherwise 
int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif /* _LCOM_KBC_H_ */
