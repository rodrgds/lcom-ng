#ifndef __MENU_H_
#define __MENU_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "states/main.h"
#include "states/menuOption.h"
#include "keyboard_mouse/keyboard.h"
#include "keyboard_mouse/mouse.h"
#include "video/video.h"

/**
 * @defgroup menu Menu
 * @{
 *
 * Functions and structures for handling menu navigation
 */

/**
 * @brief Handles keyboard input during menu navigation
 *
 * This function handles keyboard input during menu navigation.
 */
void handle_kbd_menu();

/**
 * @brief Handles mouse input during menu navigation
 *
 * This function handles mouse input during menu navigation.
 */
void handle_mouse_menu();

/**
 * @brief Handles timer interrupts during menu navigation
 *
 * This function handles timer interrupts during menu navigation.
 */
void handle_timer_menu();

/** @} */

#endif /* __MENU_H_ */
