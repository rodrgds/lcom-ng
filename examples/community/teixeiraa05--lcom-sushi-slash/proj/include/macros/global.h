#ifndef _MACROS_GLOBAL_H_
#define _MACROS_GLOBAL_H_

/**
 * @file global.h
 * @brief Defines global screen geometry helpers.
 */

/** @brief Screen width in pixels. */
#define SCREEN_WIDTH 800

/** @brief Screen height in pixels. */
#define SCREEN_HEIGHT 600

/** @brief Compute the horizontal coordinate that centers an object. */
#define CENTER_X(width) ((SCREEN_WIDTH - (width)) / 2)

/** @brief Compute the vertical coordinate that centers an object. */
#define CENTER_Y(height) ((SCREEN_HEIGHT - (height)) / 2)

#endif /* _MACROS_GLOBAL_H_ */
