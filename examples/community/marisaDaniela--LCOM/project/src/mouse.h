#pragma once
#include "utils.h"
#include "i8042.h"
#include "graphics.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

/** @defgroup mouse mouse
 * @{
 * Functions for handling the mouse interruptions
 */
typedef struct {
	int x, y;
	int xSign, ySign;
	int xD, yD;
	double speed;

	int counter;
	unsigned long packet[3];

	int leftButtonDown;
	int rightButtonDown;

	int leftButtonReleased;
	int rightButtonReleased;

	int updated;
	int draw;
} Mouse;

/**
 * @brief Reads from output buffer
 * @return 0 if sucessfull, -1 if not
 */
int mouse_read(unsigned long *inf);

/**
 * @brief Writes the command to mouse
 * @param inf command to execute
 * @return 0 if sucessfull, -1 if not
 */
int mouse_write(unsigned long inf);

/**
 * @brief Subscribes a mouse interruption
 * @return irq bit if sucessfull, -1 if not
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribes a mouse interruption
 * @return irq bit if sucessfull, -1 if not
 */
int mouse_unsubscribe_int();

/**
 * @brief Checks if mouse cursor it's inside the limits
 * @return 0 if sucessfull, -1 if not
 */
int mouseInside(Mouse* mouse, int xi,int xf,int yi,int yf); 

/**
 * @brief Creates new mouse
 * @return pointer to new mouse
 */
Mouse* newMouse();

/**
 * @brief Get mouse
 * @return pointer to mouse
 */
Mouse* getMouse();

/**
 * @brief Draws mouse cursor to the mouse buffer
 */
void drawMouse();

/**
 * @brief Handles the mouse interruptions and updates it
 */
void updateMouse();

/**
 * @brief Mouse destructor
 */
void deleteMouse(); 