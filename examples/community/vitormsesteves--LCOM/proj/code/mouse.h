#pragma once

/** @defgroup mouse mouse
 * @{
 * Functions for mouse
 */

#include "rectangle.h"
#include "bitmap.h"


typedef struct {
    int x, y; // specifies the x and y position
    int xSign, ySign; // specifies the x and y sign
    int deltaX, deltaY; // specifies the c and y variation

    unsigned int counter; // specifies the counter used on mouse handler
    unsigned int interrupts; // specifies the interrupts used on mouse handler
    unsigned long packet[3]; // specifies the packets read by the mouse

    int leftButtonDown; // specifies if the left button was pressed
    int middleButtonDown; // specifies if the middle button was pressed
    int rightButtonDown; // specifies if the right button was pressed

    int color; // specifies the mouse rectangle color
    Rectangle* rect; // specifies the mouse rectangle

} Mouse;

/**
 * @brief Get the program mouse
 *
 * @return Non NULL pointer to the mouse
 */
Mouse* getMouse();

/**
 * @brief Creates a new mouse
 *
 */
void newMouse();

/**
 * @brief update a mouse
 *
 */
void updateMouse();

/**
 * @brief Draw a mouse
 *
 */
void drawMouse();

/**
 * @brief Delete a mouse
 *
 */
void deleteMouse();

/**
 * @brief Mouse handler
 *
 */
void handlerMouse();

/**
 * @brief Change the mouse color
 *
 * @param color New color
 */
void setMouseColor(int color);

/**
 * @brief Mouse write command
 *
 * @param cmd Command to be write
 * @return validation
 */
int mouse_write(unsigned char cmd);

/**
 * @brief Mouse read value
 *
 * @param val Variable when the read value is saved
 * @return validation
 */
int mouse_read(unsigned long* val);
/**
 * @brief Mouse subscribe interrupts
 *
 * @return validation
 */
int mouse_subscribe();
/**
 * @brief Mouse unsubscribe interrupts
 *
 * @return validation
 */
int mouse_unsubscribe();
