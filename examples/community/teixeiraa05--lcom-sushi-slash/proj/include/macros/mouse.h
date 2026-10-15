#ifndef _MACROS_MOUSE_H_
#define _MACROS_MOUSE_H_

/**
 * @file mouse.h
 * @brief Defines mouse cursor and slash-trail tuning constants.
 */

/** @brief Number of historical points kept for the slash trail. */
#define MOUSE_TRAIL_LENGTH 20

/** @brief Initial slash trail width in pixels. */
#define MOUSE_TRAIL_WIDTH 10

/** @brief Minimum slash trail width in pixels. */
#define MOUSE_TRAIL_MIN_WIDTH 2

/** @brief Number of frames for which trail segments remain visible. */
#define MOUSE_TRAIL_VISIBLE_FRAMES 8

/** @brief Horizontal offset from cursor sprite origin to hit center. */
#define MOUSE_CURSOR_CENTER_OFFSET_X 6

/** @brief Vertical offset from cursor sprite origin to hit center. */
#define MOUSE_CURSOR_CENTER_OFFSET_Y 6

/** @brief Extra radius applied to slash hit detection. */
#define MOUSE_SLASH_HIT_RADIUS 6

/** @brief Brightest slash trail color. */
#define MOUSE_SLASH_1 0xFFFFFF

/** @brief Second slash trail color. */
#define MOUSE_SLASH_2 0xD9D9D9

/** @brief Third slash trail color. */
#define MOUSE_SLASH_3 0xA8A8A8

/** @brief Darkest slash trail color. */
#define MOUSE_SLASH_4 0x707070

#endif /* _MACROS_MOUSE_H_ */
