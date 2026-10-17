#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>
#include "helper.h"

/**
 @brief Reads the KBC status register.
This function reads the KBC status register and stores the status in the provided pointer.
Time Complexity: O(1) - Constant time complexity as the operations performed are independent of input size.
 @param status uint8_t* - Pointer to a uint8_t variable where the KBC status will be stored.
 @return int - Returns 0 upon success and non-zero otherwise.
*/
int (fetch_kbc_status)(uint8_t* status);


/**
 @brief Writes a command to the KBC.
This function attempts to write a command to the specified KBC port. It first checks
the KBC status and ensures that the input buffer is not full. It then tries to write
the command to the port. If unsuccessful, it retries up to NUMBER_TRY times, waiting
WAIT_KBC microseconds between each attempt.
Time Complexity: O(NUMBER_TRY) - Linear time complexity with respect to the number of maximum attempts.
 @param port uint8_t - The port to which the command will be written.
 @param comand uint8_t - The command to be written to the port.
 @return int - Returns 0 upon success and 1 otherwise.
*/
int (execute_KBC_command)(uint8_t port, uint8_t commandByte);


/**
 @brief Reads the output from the specified KBC port.
This function attempts to read the output from the specified KBC port. It first checks
the KBC status and ensures that the interrupt enable bit is set. It then reads the output
and checks for timeout and parity errors. If unsuccessful, it retries up to NUMBER_TRY times,
waiting WAIT_KBC microseconds between each attempt. If the mouse flag is set, it only reads
mouse data; otherwise, it only reads keyboard data.
Time Complexity: O(NUMBER_TRY) - Linear time complexity with respect to the number of maximum attempts.
 @param port uint8_t - The port from which the output will be read.
 @param output uint8_t* - Pointer to store the output read from the port.
 @param mouse uint8_t - Flag to indicate whether to read mouse data (1) or keyboard data (0).
 @return int - Returns 0 upon success and 1 otherwise.
*/
int (get_KBC_out)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 @brief Fetches the output from a specified port.
This function fetches the output from the specified port and stores it in the provided pointer.
 @param port The port from which to fetch the output.
 @param output Pointer to store the fetched output.
 @return 0 indicating successful execution, 1 in case of an error.
*/
int fetch_output(uint8_t port, uint8_t *output);

/**
 @brief Verifies the keyboard controller (KBC) output status.
This function verifies the KBC output status based on the provided KBC status byte and the flag indicating whether it is for a mouse or keyboard.
 @param kbc_status The KBC status byte.
 @param is_mouse Flag indicating whether the output is for a mouse (1) or keyboard (0).
 @return 0 indicating successful verification, 1 in case of an error.
*/
int verify_kbc_output(uint8_t kbc_status, uint8_t is_mouse); 

/**
 @brief Checks for status errors in the keyboard controller (KBC) status byte.
This function checks for timeout error and parity error in the provided KBC status byte and prints corresponding error messages if detected.
 @param kbc_status The KBC status byte to check.
 @return 0 indicating no errors detected, 1 if a timeout error or parity error is detected.
*/
int check_for_status_errors(uint8_t kbc_status);


#endif /* _LCOM_KBC_H_ */
