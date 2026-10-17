#ifndef __MOUSE_H
#define __MOUSE_H

#include "bitmap.h"
#include "rectangle.h"

/** @defgroup mouse mouse
 * @{
 *
 * Functions for controlling the mouse
 */

/*! Represents the mouse */
typedef struct {
    int x, y;

    int leftButtonPressed, leftButtonReleased;
    int readingInfo;
    unsigned char packet[3];

    int draw, update;
    Bitmap* bmp;

} Mouse;

/**
 * @brief creates the mouse; allocates it
 * @return a pointer to the mouse
 */
Mouse* newMouse();

/**
 * @brief gets the mouse
 * @return a pointer to the mouse
 */
Mouse* getMouse();

/**
 * @brief enables mouse stream
 */
void enableMouse();

/**
 * @brief disables mouse stream
 */
void disableMouse();

/**
 * @brief draws the mouse on the screen
 */
void drawMouse();

/**
 * @brief frees mouse form memory
 */
void deleteMouse();

/**
 * @brief updates mouse position and buttons upon interrupt
 */
void updateMouse();

/**
 * @brief checks if a mouse is inside a rectangle form
 * @param rect a rectangle
 * @return non-zero if mouse is inside, 0 if it isn't
 */
int mouseInsideRect(Rectangle* rect);

/**
 * @brief subscribes mouse interrupts
 * @return non-zero if mouse is subscribed, 0 if it isn't
 */
int mouse_subscribe_int();

/**
 * @brief subscribes mouse interrupts
 * @return non-zero if mouse is unsubscribed, 0 if it isn't
 */
int mouse_unsubscribe_int();

/**
 * @brief reads data from output buffer
 * @param data pointer to where to store the data
 * @return non-zero if mouse is successful, 0 if it isn't
 */
int mouse_read(unsigned long *data);

/**
 * @brief writes a command to the mouse
 * @param where_to address to write to
 * @param command what command is to be given to the kbc
 * @return non-zero if mouse is successful, 0 if it isn't
 */
int mouse_write(char where_to, unsigned long command);

/**
 * @brief mouse interrupt handler
 * @param count 0 or 1 or 2, the number of the packet to read
 * @param packets array of 3 packets sent by the mouse
 */
void mouse_int_handler(unsigned int count, char packets[]);

/**
 * @brief sends command to the mouse until successful (ACK received)
 * @param command what command is to be given to the mouse
 */
void mouse_interface(unsigned long command);

/**
 * @brief sends command to the mouse until successful (ACK received)
 */
void clear_mouse_buffer();

/** @} */

#endif /* __MOUSE.H */
