/**
 * @file mainMenuView.h
 * @brief Main menu view header file
 * 
 * This header file defines the main menu view components, including
 * sprites for menu options and functions for rendering the main menu screen.
 */

#ifndef _MAINMENUVIEW_H_
#define _MAINMENUVIEW_H_

#include "../../Model/Sprite/sprite.h"
#include "../../Model/XPMs/mainMenu.xpm"
#include "../../Model/Background/background.h"
#include "../../Model/SelectorMine/selectorMine.h"

// Position constants for menu options
#define PLAY_X 343          // X position for PLAY button
#define PLAY_Y 260          // Y position for PLAY button
#define INST_X 224          // X position for INSTRUCTIONS button
#define INST_Y 360          // Y position for INSTRUCTIONS button
#define EXIT_X 340          // X position for EXIT button
#define EXIT_Y 460          // Y position for EXIT button

// Main menu sprite declarations
Sprite* mm_background;      // Main menu background sprite
Sprite* play;              // Play button sprite
Sprite* instructions;      // Instructions button sprite
Sprite* exit_;             // Exit button sprite

/**
 * @brief Loads all main menu sprites from XPM files
 */
void loadMainMenuSprites();

/**
 * @brief Sets the main menu background for rendering
 */
void setMainMenuBackgorund();

/**
 * @brief Draws the selector mine on the screen
 * 
 * @param mine The selector mine object to draw
 */
void drawSMine(SMine mine);

#endif
