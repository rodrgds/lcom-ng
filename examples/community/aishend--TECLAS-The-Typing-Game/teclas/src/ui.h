/**
 * @file ui.h
 * @brief Reusable UI widget helpers (buttons, stats, option rows).
 */
#ifndef TECLAS_UI_H
#define TECLAS_UI_H

#include "font.h"
#include "video.h"
#include <stdbool.h>
#include <stdint.h>

#define UI_BTN_FACE 0x141C2E
#define UI_BTN_FACE_HOVER 0x1E2D48
#define UI_BTN_RIM_TOP 0x323B56
#define UI_BTN_RIM_TOP_HOVER 0x4A6A9A
#define UI_BTN_RIM_BOT 0x080C18
#define UI_BTN_ACCENT 0x6A9ADB
#define UI_BTN_TEXT 0xE8E8E8
#define UI_BTN_TEXT_HOVER 0xFFFFFF

/**
 * @brief Draw a label/value stat pair and return the y position below it.
 *
 * @param label Label string.
 * @param value Value string.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param label_col Color for the label text.
 * @param value_col Color for the value text.
 * @return Y coordinate immediately below the drawn stat.
 */
uint16_t ui_draw_stat(const char *label, const char *value, uint16_t x, uint16_t y, uint32_t label_col, uint32_t value_col);

/**
 * @brief Draw a button with a hover state.
 *
 * @param x Top-left X coordinate.
 * @param y Top-left Y coordinate.
 * @param w Width in pixels.
 * @param h Height in pixels.
 * @param label Button label text.
 * @param hovered Whether the cursor is over the button.
 */
void ui_draw_button(int x, int y, int w, int h, const char *label, bool hovered);

/**
 * @brief Test whether a mouse position is inside a button's bounds.
 *
 * @param x Button top-left X.
 * @param y Button top-left Y.
 * @param w Button width.
 * @param h Button height.
 * @param mx Mouse X coordinate.
 * @param my Mouse Y coordinate.
 * @return true if the mouse is over the button, false otherwise.
 */
bool ui_button_hit(int x, int y, int w, int h, int mx, int my);

/**
 * @brief Draw the ESC key hint at the bottom of the screen.
 */
void ui_draw_esc_hint();

/**
 * @brief Draw a horizontal row of selectable options.
 *
 * @param opts Array of option label strings.
 * @param n Number of options.
 * @param sel Index of the currently selected option.
 * @param cx Center X position of the row.
 * @param y Y coordinate of the row.
 * @param col_sel_bg Background color for the selected option.
 * @param col_sel_fg Foreground color for the selected option.
 * @param col_unsel_fg Foreground color for unselected options.
 */
void ui_draw_option_row(const char **opts, int n, int sel, uint16_t cx, uint16_t y, uint32_t col_sel_bg, uint32_t col_sel_fg,
    uint32_t col_unsel_fg);

/**
 * @brief Return the index of the option clicked, or -1 if none.
 *
 * @param opts Array of option label strings.
 * @param n Number of options.
 * @param cx Center X position of the row.
 * @param row_y Y coordinate of the row.
 * @param mx Mouse X coordinate.
 * @param my Mouse Y coordinate.
 * @return Index of the clicked option, or -1 if the click missed.
 */
int ui_hit_option_row(const char **opts, int n, int cx, int row_y, int mx, int my);

#endif
