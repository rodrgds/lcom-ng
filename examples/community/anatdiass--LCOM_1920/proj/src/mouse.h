#ifndef __MOUSE_H
#define __MOUSE_H

#include "i8042.h"

/**
 * @brief Subscribes mouse interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe_int (uint8_t *bit_no);
/**
 * @brief Unsubscribes mouse interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int();
//int util_sys_inb (int port, uint8_t *value);
/**
 * @brief Receives an incomplete packet and parses it
 * 
 * @param pp 
 */
void mouse_process_packets(struct packet *pp);
/**
 * @brief Writes byte to mouse
 * 
 * @param command 
 * @return Return 0 upon success and non-zero otherwise
 */
int write_command(uint8_t command);
/**
 * @brief Enables stream mode data reporting
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int enableDataReporting();
/**
 * @brief Disables stream mode data reporting
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int disableDataReporting();
/**
 * @brief the function detects if the user draws 
 * an inverted V, from left to right and how it is drawn
 * 
 * @param evt 
 * @param x_len 
 * @param tolerance 
 */
void mouse_gesture_handler(struct mouse_ev *evt, uint8_t x_len, uint8_t tolerance);
/**
 * @brief function detects the events that occur in the state machine
 * 
 * @param pp 
 * @return struct mouse_ev* 
 */
struct mouse_ev* detectMouseEvent(struct packet *pp);

//State machine
enum state_t{
    INIT,
    DRAW,   //Start drawing first line
    FIRSTLINE, //First line done
    DRAWSECOND,  //Start drawing second line
    //COMP,        //Inverted V complete
};

#endif /* __MOUSE_H */
