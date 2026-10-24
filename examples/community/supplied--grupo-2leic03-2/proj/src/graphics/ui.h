#ifndef _UI_H_
#define _UI_H_

#include <stdbool.h>
#include <stdint.h>

#include "../input/input.h"

/**
 * @brief Simple rectangular UI button state.
 *
 * The button stores its hitbox and the hover/click state computed from the
 * current InputState. Drawing is handled separately by ui_draw_button().
 */
typedef struct {
  int x;
  int y;
  int width;
  int height;
  bool is_hovered;
  bool is_clicked;
} Button;

/**
 * @brief Initializes a button hitbox and clears interaction state.
 *
 * @param btn Button to initialize.
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param w Button width.
 * @param h Button height.
 */
void button_init(Button *btn, int x, int y, int w, int h);

/**
 * @brief Updates button hover and click state from mouse input.
 *
 * A click is registered when the mouse is inside the hitbox and the current
 * frame reports mouse_left_clicked.
 *
 * @param btn Button to update.
 * @param input Current input state.
 */
void button_update(Button *btn, const InputState *input);

/**
 * @brief Draws a filled UI panel with a solid border.
 *
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param width Panel width.
 * @param height Panel height.
 * @param border_color Border color.
 * @param fill_color Interior color.
 */
void ui_draw_panel(int x, int y, int width, int height, uint32_t border_color, uint32_t fill_color);

/**
 * @brief Draws text centered inside a rectangular area.
 *
 * The text width is computed from the 5x7 bitmap font metrics and the given
 * scale.
 *
 * @param text Null-terminated text to draw.
 * @param x Rectangle top-left x coordinate.
 * @param y Rectangle top-left y coordinate.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param color Text color.
 * @param scale Integer font scale.
 */
void ui_draw_text_centered(const char *text,
                           int x,
                           int y,
                           int width,
                           int height,
                           uint32_t color,
                           uint8_t scale);

/**
 * @brief Draws a button with border, hover fill and centered text.
 *
 * @param button Button state and hitbox.
 * @param text Button label. May be NULL.
 * @param normal_color Fill color when not hovered.
 * @param hover_color Fill color when hovered.
 * @param text_color Label color.
 */
void ui_draw_button(const Button *button,
                    const char *text,
                    uint32_t normal_color,
                    uint32_t hover_color,
                    uint32_t text_color);

/**
 * @brief Draws the fallback software cursor.
 *
 * Used when the cursor XPM sprite is not available.
 *
 * @param input Current input state with mouse coordinates.
 */
void ui_draw_cursor(const InputState *input);

#endif
