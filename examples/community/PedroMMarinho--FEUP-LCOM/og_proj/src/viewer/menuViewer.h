#pragma once
#include <lcom/lcf.h>
#include <math.h>
#include "../model/button.h"
#include "../model/mouseModel.h"

/**
 * @brief Draws the background image of the menu.
 *
 * This function draws the specified background image at a fixed position on the screen.
 *
 * @param img The xpm_image_t structure representing the background image.
 * @return 0 on success, 1 on failure.
 */
int drawMenuBackground(xpm_image_t img);

/**
 * @brief Draws a menu button.
 *
 * This function draws the specified button, either in its selected or unselected state, at a position offset from the button's position.
 *
 * @param button Pointer to the Button structure representing the button to draw.
 * @return 0 on success, 1 on failure.
 */
int drawMenuButton(Button* button);

/**
 * @brief Draws the mouse cursor in the menu.
 *
 * This function draws the mouse cursor image, either in its default or hover state, at the current mouse position.
 *
 * @param mouse Pointer to the Mouse structure representing the mouse.
 * @param mouseImg The xpm_image_t structure representing the default mouse image.
 * @param imgMouseHover The xpm_image_t structure representing the mouse hover image.
 * @param isHovering Boolean indicating whether the mouse is hovering over an interactable element.
 * @return 0 on success, 1 on failure.
 */

int  drawMenuMouse(Mouse* mouse, xpm_image_t mouseImg, xpm_image_t imgMouseHover, bool isHovering);

/**
 * @brief Draws the menu logo with animation.
 *
 * This function draws the specified logo image at a position that oscillates vertically based on the time parameter.
 *
 * @param img The xpm_image_t structure representing the logo image.
 * @param time The current time, used for animating the logo's vertical position.
 * @return 0 on success, 1 on failure.
 */
int drawMenuLogo(xpm_image_t img,int time);
/**
 * @brief Draws the player's name on the menu.
 *
 * This function draws the player's name using the specified font at a given position with specified spacing between characters.
 *
 * @param playerName Pointer to the string representing the player's name.
 * @param playerNameFont Pointer to the array of xpm_image_t structures representing the font.
 * @param x The x-coordinate where the name should be drawn.
 * @param y The y-coordinate where the name should be drawn.
 * @param spacing The spacing between characters.
 * @return 0 on success, 1 on failure.
 */
int drawPlayerName(char* playerName, xpm_image_t* playerNameFont, uint16_t x, uint16_t y, uint8_t spacing);

/**
 * @brief Draws animated ellipses on the menu.
 *
 * This function draws a sequence of ellipses, with their number and position based on the current time, to create an animation effect.
 *
 * @param imgBall The xpm_image_t structure representing the ellipse image.
 * @param time The current time, used for determining the number of ellipses to draw.
 * @return 0 on success, 1 on failure.
 */

int drawEllipses(xpm_image_t imgBall,int time);
