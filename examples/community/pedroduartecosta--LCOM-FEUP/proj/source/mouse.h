#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/drivers.h>
#include <stdbool.h>
#include <math.h>
#include "i8254.h"
#include "Bitmap.h"
#include "Button.h"

/** @defgroup Mouse Mouse
 * @{
 * functions regarding the mouse
 */

unsigned long packet[3];
static int hook_mouse=NOTIFICATION_MOUSE;

typedef struct {
	int x,y;

	int lb_pressed;
	int mb_pressed;
	int rb_pressed;
	int lb_released;
	int mb_released;
	int rb_released;

	Bitmap* image;

} Mouse;

/**
 * @brief gets created mouse
 *
 * @return Null pointer to the mouse
 */
Mouse* getMouse();

/**
 * @brief sets new mouse definitions
 *
 * @param mouse
 */
void setMouse(Mouse* m);

/**
 * @brief creates a new mouse, loads bitmap image and sets initial coordinates
 *
 * @return Null pointer to the mouse
 */
Mouse* newMouse();

/**
 * @brief updates mouse definitions
 *
 */
void updateMouse();
/**
 * @brief draws mouse on screan
 *
 */
void drawMouse();
/**
 * @brief subscribes mouse interruption
 *
 *@return mouse irq
 */
int mouse_subscribe();
/**
 * @brief unsubscribes mouse interruption
 *
 *@return mouse irq
 */
int mouse_unsubscribe();
/**
 * @brief writes command to kbc
 *
 * @return 1 if successful
 */
int mouse_write_command(char port, unsigned char cmd);
/**
 * @brief reads mouse definitions
 *
 * @return 1 if successful
 */
int mouse_read();
/**
 * @brief gets mouse packet containing its definitions
 *
 * @return 1 if successful
 */
int get_packet();
/**
 * @brief prints mouse definitions
 *
 */
void mouse_print();
/**
 * @brief checks if mouse is inside certain boundaries
 *
 * @param x coordinate of left top corner
 * @param y coordinate of left top corner
 * @param x coordinate of right bottom corner
 * @param y coordinate of left bottom corner
 *
 * @return 1 if inside these boundaries
 *
 */
int mouseInside(int x1, int y1, int x2, int y2);
/**
 * @brief checks if mouse is inside a box
 *
 * @param box
 *
 * @return 1 if inside box
 *
 */
int mouseInsideBox(Box* box);
/**@}*/
#endif
