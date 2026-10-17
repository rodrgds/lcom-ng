#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>

/**
 * @struct MenuItem
 * @brief Represents a single item in the game menu.
 * 
 * Stores position, size, image and visibility of the menu item.
 */
typedef struct {
    int16_t x, y;
    uint16_t width, height;
    xpm_image_t ItemImage;
    bool isVisible;
} MenuItem;

/**
 * @brief Initializes a menu item with given position and image.
 * 
 * Sets the position, loads the XPM image, and initializes visibility and size.
 * 
 * @param item Pointer to the MenuItem to initialize
 * @param x Horizontal position on screen
 * @param y Vertical position on screen
 * @param image XPM map image to use for the menu item
 */
void (create_menu_item)(MenuItem* item, int16_t x, int16_t y, xpm_map_t image);

#endif
