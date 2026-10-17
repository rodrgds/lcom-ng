#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "KBC.h"
#include "helper.h"

#define mask_mouse BIT(2)

typedef struct {
    int16_t x;
    int16_t y;
} MouseLoc;


/**
 @brief Subscribes and enables mouse interrupts.
This function subscribes mouse interrupts and enables them, returning the
bit number for the interrupt notification in the 'bit_no' parameter.
Time Complexity: O(1) - Constant time complexity.
 @param bit_no Pointer to a uint8_t where the bit number for the interrupt notification will be stored.
 @return 0 on success, 1 on failure.
*/
int (mouse_subscribe_int)(uint8_t *bit_no);


/**
 @brief Unsubscribes mouse interrupts.
This function unsubscribes mouse interrupts, disabling them.
Time Complexity: O(1) - Constant time complexity.
 @return 0 on success, a non-zero value otherwise.
*/
int (mouse_unsubscribe_int)();


/**
 @brief Mouse interrupt handler.
This function handles mouse interrupts, reading the mouse byte from
the output buffer.
Time Complexity: O(1) - Constant time complexity.
*/
void (mouse_ih)();


/**
 @brief Synchronizes mouse bytes.
This function synchronizes mouse bytes by checking if the current byte is the
first byte, and then incrementing the byte index accordingly.
Time Complexity: O(1) - Constant time complexity.
*/
void (mouse_sync_bytes)();


/**
 @brief Converts mouse bytes to a mouse packet structure.
This function populates a mouse_data structure with the appropriate
information extracted from the packet array. It sets the button states,
overflow flags, and delta values for the X and Y axis.
Time Complexity: O(1) - Constant time complexity.
*/
void (packet_load)();


/**
 @brief Writes a command to the mouse.
This function writes a command to the mouse, by first sending the WRITE_BYTE_MOUSE command to the KBC,
followed by the actual command to be sent to the mouse. It then waits for the KBC to respond with an
ACK message before returning 0. If the maximum number of attempts is reached without receiving an ACK
message, it returns 1.
 @param command the command to be sent to the mouse.
 @return 0 upon success, 1 upon failure.
 @note This function uses the following constants: NUMBER_TRY, KBC_IN_CMD, KBC_OUT_CMD, WRITE_BYTE_MOUSE,
ACK, and WAIT_KBC.
 @warning This function assumes that the mouse is already enabled, as it does not enable the mouse.
 @details The complexity of this function is O(NUMBER_TRY), as it may need to wait up to NUMBER_TRY times
for an ACK message to be received from the KBC.
*/
int (send_mouse_command)(uint8_t command);

/**
 @brief Changes the mouse location based on the received mouse packet.
This function updates the current mouse location by adding the delta x and delta y values
from the received mouse packet. It also checks for overflow conditions and makes sure that
the new location is within the screen bounds.
 @return void
*/
void (mouse_change_location)();

/**
 @brief Retrieves the response from the mouse.
This function retrieves the response from the mouse by reading the data from the KBC output port.
 @param mouse_response Pointer to store the mouse response.
 @return 0 indicating successful retrieval, non-zero value in case of an error.
*/
int get_response_from_mouse(uint8_t* mouse_response);

#endif
