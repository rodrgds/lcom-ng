#ifndef _LCOM_KBC_MOUSE_KEYBOARD_H_
#define _LCOM_KBC_MOUSE_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#define BIT(n) (1 << (n))


///@brief Read the state of the KBC.
///@note This function reads the state of the Keyboard Controller (KBC) by reading from port 0x64 and stores the state in the state variable
///@param state Pointer to store the state of the KBC.
///@return 0 if the read is successful, otherwise returns 1.

int(reading_KBC_STATE)(uint8_t * state);


///@brief Write a command to the KBC input port.
///@note This function writes a command to the Keyboard Controller (KBC) input port specified by the port parameter and waits for the KBC to be ready for the write operation. It retries the write operation multiple times.
///@param port The KBC input port.
///@param kbc_command The command to be sent to the KBC.
///@return 0 if the write is successful, otherwise returns 1.
int(write_KBC_INPUT)(uint8_t port, uint8_t kbc_command);


///@brief Read the keyboard output from the KBC.
///@note This function reads the keyboard output from the Keyboard Controller (KBC) specified by the port parameter. It waits for the KBC to have an available output and retrieves the output from the KBC. It also checks for any error conditions in the KBC state.
///@param port The KBC port to read from.
///@param kbc_output Pointer to store the keyboard output read from the KBC.
///@return 0 if the read is successful and there are no error conditions, otherwise returns 1.
int(read_KBC_keyboardOUTPUT)(uint8_t port, uint8_t * kbc_output);

///@brief Read the mouse output from the KBC.
///@note This function reads the mouse output from the Keyboard Controller (KBC) specified by the port parameter. It waits for the KBC to have an available output and retrieves the output from the KBC. It also checks for any error conditions in the KBC state.
///@param port The KBC port to read from.
///@param kbc_output Pointer to store the mouse output read from the KBC.
///@return 0 if the read is successful and there are no error conditions, otherwise returns 1.
int(read_KBC_mouseOUTPUT)(uint8_t port, uint8_t * kbc_output);

#endif
