#ifndef FRAMES_H
#define FRAMES_H

#include <stdint.h>
#include "../drivers/graphics.h"

/**
 * @brief Struct representing an image frame.
 *
 * A Frame holds the width, height, and raw pixel map of an XPM image,
 * which can be drawn to the screen.
 */
typedef struct {
    uint16_t width;   /**< Width of the frame in pixels */
    uint16_t height;  /**< Height of the frame in pixels */
    char *map;        /**< Pointer to the pixel data (RGBA format) */
} Frame;

/**
 * @brief Creates a Frame from an XPM image.
 *
 * Allocates and initializes a Frame with the contents of the given XPM image.
 *
 * @param xpm Array of strings representing an XPM image.
 * @return Pointer to the created Frame, or NULL on failure.
 */
Frame *createFrame(const char *xpm[]);

/**
 * @brief Destroys a Frame and frees associated memory.
 *
 * Deallocates memory used by the given Frame.
 *
 * @param frame Pointer to the Frame to destroy.
 */
void destoryFrame(Frame *frame);

/**
 * @brief Draws a Frame on the screen at the given position.
 *
 * Renders the Frame on screen using the loaded pixel data.
 *
 * @param xi X coordinate of the top-left corner.
 * @param yi Y coordinate of the top-left corner.
 * @param frame Pointer to the Frame to draw.
 */
void drawframe(uint16_t xi, uint16_t yi, Frame *frame);

#endif // FRAMES_H
