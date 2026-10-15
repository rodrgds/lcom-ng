/**
 * @file mouse.h
 * @brief Functions implemented to improve the use of the mouse driver
 */
#ifndef _PROJ_MOUSE_
#define _PROJ_MOUSE_

#include <lcom/lcf.h>
#include "../helper/structs.h"

// interrupt related

/**
 * @brief subscribes mouse interrupts
 * 
 * @param irq_mask pointer to a variable storing the bit number
 * 
 * @return 0 on success, 1 on failure
 */
int mouse_int_sub(uint8_t *irq_mask);


/**
 * @brief unsubscribes mouse interrupts
 * 
 * @return 0 on success, 1 on failure
 */
int mouse_int_unsub();


/**
 * @brief enables or disables data reporting
 * 
 * @param value tells whether to enable or not reporting
 * 
 * @return 0 on success, 1 on failure
 */
int mouse_set_reporting(bool value);

/**
 * @brief processes the mouse packet
 * 
 * @param pack pointer to the mouse packet
 * 
 * @return 0 on success
 */
int mouse_get_packet(struct mouse_packet *pack);


/** 
 * @brief checks if the left mouse button was clicked
 * 
 * @param pack pointer to the mouse packet
 * 
 * @return true if left mouse button was clicked, false if it wasn't
*/
bool left_mouse_clicked(struct mouse_packet *pack);

/** 
 * @brief checks if the right mouse button was clicked
 * 
 * @param pack pointer to the mouse packet
 * 
 * @return true if right mouse button was clicked, false if it wasn't
*/
bool right_mouse_clicked(struct mouse_packet *pack);

/** 
 * @brief checks if the middle mouse button was clicked
 * 
 * @param pack pointer to the mouse packet
 * 
 * @return true if middle mouse button was clicked and false if it wasn't
*/
bool mid_mouse_clicked(struct mouse_packet *pack);

/**
 * @brief checks if mouse is hovering a sprite
 * 
 * @param button pointer to a sprite
 * 
 * @return true if the mouse is hovering the sprite and false if it isn't
 */
bool mouse_hover(struct Sprite *button);


/**
 * @brief handles mouse interrupts
 */
void (mouse_ih)();

#endif // _PROJ_MOUSE_
