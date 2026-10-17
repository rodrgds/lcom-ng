#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdbool.h>
#include <stdint.h>
#include "../drivers/graphics.h"

/**
 * @brief Struct representing a UI button.
 *
 * This struct defines a clickable button in the user interface, including
 * its position, size, and optional image data.
 */
struct button {
    uint16_t x;       /**< X coordinate of the top-left corner of the button */
    uint16_t y;       /**< Y coordinate of the top-left corner of the button */
    uint16_t width;   /**< Width of the button in pixels */
    uint16_t height;  /**< Height of the button in pixels */
    char *image;      /**< Pointer to the button's image data (RGBA format or XPM-loaded) */
};

#endif // OBJECTS_H
