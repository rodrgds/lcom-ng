#ifndef __MOUSE_H
#define __MOUSE_H

/** @defgroup mouse mouse
 * @{
 * Functions related to the mouse methods
 */

#include "i8042.h"
#include "i8254.h"
#include "kbd.h"
#include "Bitmap.h"

/**
 * Mouse struct
 */
typedef struct
{
	int x, y;
	int LeftPressed;
	char packet[3];
} Mouse;

/**
 * Inicialize the mouse struct
 * @return the new struct
 */
Mouse* newMouse();
/**
 * Give mouse permissions
 *
 */
void mousePermissions();
/**
 * Subscribe the mouse
 * @return irq
 */
int mouse_subscribe_int();
/**
 * Unsubscribe the mouse
 * @return irq
 */
int mouse_unsubscribe_int();
/**
 * Checks if the leftButton was press
 * @param auxMouse mouse struct
 */
void LeftButton(Mouse* auxMouse);
/**
 * Handler the mouse
 * @param packet[] packet
 * @return 1 if the packet is complete, 0 if otherwise
 */
int mouse_int_handler(char packet[]);
/**
 * Prints the mouse packet
 * @param packet[] packet
 */
void mouse_print(unsigned char packet[]);
void print_config(char packet[]);


/** @} */

#endif /* __MOUSE.H */
