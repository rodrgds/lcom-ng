/**
 * @file keyboard.h
 * 
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

typedef struct{
  uint16_t x;
  uint16_t y;
} Mouse;



/**
 * @brief This method subscribes and enables the keyboard interrupts
 * 
 * @param bit_no Pointer to the variable that holds the keyboard hook id
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int(kbc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief This method unsubscribes the keyboard interrupts
 * 
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int(kbc_unsubscribe_int)();


/**
 * @brief This method reads the keyboard configuration
 * 
 * @param st Address of the memory position that will receive the status byte
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int(kbc_get_status)(uint8_t *st);


/**
 * @brief This method reads the output buffer of the keyboard 
 * 
 * @param data Pointer to the variable where the data read from the output buffer will be stored.
 * @param st Address of the memory position that will receive the status byte
 * @return Returns an int value of zero upon success and one otherwise
 */
int(kbc_read_out_buffer)(uint8_t *data, uint8_t * st);


/**
 * @brief This method sends a command to the keyboard
 * 
 * @param port The port to use for sending the command
 * @param cmd The command to be sent
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int(kbc_send_cmd)(uint8_t port, uint8_t cmd);

/**
 * @brief This method reads the output buffer of the mouse 
 * 
 * @param data Pointer to the variable where the data read from the output buffer will be stored.
 * @param st Address of the memory position that will receive the status byte
 * @return Returns an int value of zero upon success and one otherwise
 */
int(mouse_read_out_buffer)(uint8_t *data, uint8_t *st );

/**
 * @brief This method subscribes and enables the mouse interrupts
 * 
 * @param bit_no Pointer to the variable that holds the mouse hook id.
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief This method unsubscribes the mouse interrupts
 * 
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int(mouse_unsubscribe_int)();

/**
 * @brief This method writes a command to the mouse
 * 
 * @param cmd Command to write to the mouse
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int(mouse_write_cmd)(uint8_t cmd); 

/**
 * @brief This method parses the mouse packet and sets its values
 * 
 * @param pp Pointer to the packet
 * @return Returns an int value of zero upon success
 */
int (mouse_parse_packet)(struct packet *pp);

/**
 * @brief This method retrieves data from the mouse
 * 
 * @param pp Pointer to the struct where that data will be stored
 * @return Returns an int value of zero upon success and one othersise
 */
int (mouse_get_data)(struct packet *pp);

/**
 * @brief This method reads a scancode from the keyboard
 * 
 * @param data Pointer to the array that will store the scancode
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int(kbc_get_scancode)(uint8_t* data);

/**
 * @brief This method checks the keyboard status
 * 
 * @param st Variable that contains the keyboard status
 * @return Returns an int value of zero upon success and non-zero otherwise
 */
int check_status(uint8_t st);

/**
 * @brief This method checks if the left movement key (A) is pressed
 * 
 * @return true if the key is pressed
 * @return false if the key is not pressed
 */
bool leftKeyPressed();

/**
 * @brief This method checks if the right movement key (D) is pressed
 * 
 * @return true if the key is pressed
 * @return false if the key is not pressed
 */
bool rightKeyPressed();

/**
 * @brief This method sets the state for the left movement key (A)
 * 
 * @param is_pressed Boolean value indicating whether the left key is pressed or not
 */
void pressLeftKey(bool is_pressed);

/**
 * @brief This method sets the state for the right movement key (D)
 * 
 * @param is_pressed Boolean value indicating whether the right key is pressed or not
 */
void pressRightKey(bool is_pressed);



#endif

