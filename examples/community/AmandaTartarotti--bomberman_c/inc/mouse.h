#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
     * @brief Subscribes and enables Mouse interrupts. 
     * 
     * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (mouse_subscribe_int)(uint8_t *bit_no);
/**
     * @brief Unsubscribes Timer interrupts. 
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (mouse_unsubscribe_int)();
/**
     * @brief Handles mouse interrutps
     * 
     * Reads the status register and the output buffer (OB).
     * If there was some error, the byte read from the OB should be discarded.
     * 
**/
void (mouse_ih)();
/**
     * @brief Evaluates de mouse moviments and clicks done by the user.
     * 
     * @param delta_x Mouse x-displacement: rightwards is positive
     * @param delta_y Mouse y-displacement: upwards is positive
     * @param rb Assigns if the right mouse button have been pressed
     * @param mouse_int Checks if it is the first time that the mouse interrupt has been called
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (update_mouse)(int16_t delta_x, int16_t delta_y, bool rb, bool mouse_int);
/**
     * @brief Enables the keyboard after using the mouse, by sending the respective command to the command register.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (mouse_disable_stream)();

#endif /* _LCOM_MOUSE_H */
