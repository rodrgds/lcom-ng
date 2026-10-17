#ifndef TARGET_H
#define TARGET_H

#include <lcom/lcf.h>

/**
 * @struct Target
 * @brief Represents a target in the game.
 * 
 * Contains the position, size, image, visibility and movement properties of a target.
 */
typedef struct {
    int16_t x, y;
    uint16_t width, height;
    xpm_image_t TargetImage;
    bool isVisible;
    int16_t fall_speed;
    int16_t move_speed;
    int direction;
} Target;

/**
 * @brief Initializes a target with given position and image.
 * 
 * Sets the position, loads the XPM image, and initializes visibility and size.
 * 
 * @param target Pointer to the Target to initialize
 * @param x Horizontal position on screen
 * @param y Vertical position on screen
 * @param image XPM map image to use for the target
 */
void (create_target)(Target* target, int16_t x, int16_t y, xpm_map_t image);

#endif

