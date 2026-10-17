/**
 * @file mainMenuView.c
 * @brief Main menu view implementation
 * 
 * This file contains the implementation of main menu view functions
 * for loading sprites, setting up the background, and rendering
 * the main menu interface.
 */

#include "mainMenuView.h"

/**
 * @brief Loads all sprites needed for the main menu
 * 
 * This function generates sprites from XPM data for the main menu
 * background and all menu option buttons.
 */
void loadMainMenuSprites() {
    mm_background = generate_Sprite((xpm_map_t) MainMenuBackGround);
    play = generate_Sprite((xpm_map_t) MainMenuPlay);
    instructions = generate_Sprite((xpm_map_t) MainMenuInstructions);
    exit_ = generate_Sprite((xpm_map_t) MainMenuExit);
}

/**
 * @brief Sets up the main menu background with all UI elements
 * 
 * This function loads the background and draws all menu buttons
 * at their designated positions to create the complete main menu screen.
 */
void setMainMenuBackgorund() {
    loadBackground(mm_background);                          // Load background image
    drawSpriteToBackground(play, PLAY_X, PLAY_Y);          // Draw PLAY button
    drawSpriteToBackground(instructions, INST_X, INST_Y);  // Draw INSTRUCTIONS button
    drawSpriteToBackground(exit_, EXIT_X, EXIT_Y);         // Draw EXIT button
}

/**
 * @brief Draws the selector mine sprite on screen
 * 
 * This function renders the mine selector sprite that indicates
 * the currently selected menu option.
 * 
 * @param mine The selector mine object containing position and sprite data
 */
void drawSMine(SMine mine) {
    draw_Sprite(mine.sprite, mine.pos.x, mine.pos.y);
}
