#ifndef _MENU_H
#define _MENU_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <xpms/playPart.xpm>
#include <xpms/exitPart.xpm>
#include "kbcKeys.h"
#include <xpms/leaderPart.xpm>
#include <xpms/menu.xpm>
#include <xpms/mouseCursor.xpm>
#include "devices/graphics.h"

/**
 * @brief checks the mouse position and chooses the option that it's hovering if clicked on
 * 
 */
void MenuMouseHandler();
/**
 * @brief refreshes the menu screen with the update mouse position
 * 
 */
void MenuTimerHandler();
/**
 * @brief draws with the selected option highlighted
 * 
 */
void drawMenu();
/**
 * @brief draws mouse cursor
 * 
 */
void drawCursor();
/**
 * @brief draws background of menu
 * 
 */
void drawDefMenu();
/**
 * @brief erases the text and cursor from the menu
 * 
 */
void cleanMenuBG();
/**
 * @brief changes gamestate depending on the selected option
 * 
 */
void changeState();
#endif
