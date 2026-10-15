#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @file button.h
 * @brief Declares the shared UI button data structure.
 */

/**
 * @brief Rectangular text button used by menus.
 */
typedef struct {
  /** @brief Left coordinate of the button. */
  uint16_t x;

  /** @brief Top coordinate of the button. */
  uint16_t y;

  /** @brief Button width in pixels. */
  uint16_t width;

  /** @brief Button height in pixels. */
  uint16_t height;

  /** @brief Fill color used when the button is not hovered. */
  uint32_t color;

  /** @brief Fill color used when the button is hovered. */
  uint32_t hover_color;

  /** @brief Null-terminated text label drawn inside the button. */
  const char *label;

  /** @brief Whether the button is currently hovered. */
  bool hovered;
} button_t;

#endif /* _BUTTON_H_ */
