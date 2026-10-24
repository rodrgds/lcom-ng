#include "ui.h"

#include <stddef.h>

#include "graphics.h"
#include "font.h"
#include "../config.h"

#define UI_PANEL_BORDER 4
#define UI_BUTTON_BORDER 4
#define UI_CURSOR_SIZE 10
#define UI_BUTTON_TEXT_SCALE 2

void button_init(Button *btn, int x, int y, int w, int h) {
  if (btn == NULL) {
    return;
  }

  btn->x = x;
  btn->y = y;
  btn->width = w;
  btn->height = h;
  btn->is_hovered = false;
  btn->is_clicked = false;
}

void button_update(Button *btn, const InputState *input) {
  if (btn == NULL || input == NULL) {
    return;
  }

  if (btn->width <= 0 || btn->height <= 0) {
    btn->is_hovered = false;
    btn->is_clicked = false;
    return;
  }

  btn->is_hovered =
    input->mouse_x >= btn->x &&
    input->mouse_x < btn->x + btn->width &&
    input->mouse_y >= btn->y &&
    input->mouse_y < btn->y + btn->height;

  btn->is_clicked = btn->is_hovered && input->mouse_left_clicked;
}

void ui_draw_panel(int x, int y, int width, int height, uint32_t border_color, uint32_t fill_color) {
  if (width <= 2 * UI_PANEL_BORDER || height <= 2 * UI_PANEL_BORDER) {
    return;
  }

  graphics_draw_rectangle_clipped(x, y, width, height, border_color);
  graphics_draw_rectangle_clipped(x + UI_PANEL_BORDER,
                                  y + UI_PANEL_BORDER,
                                  width - 2 * UI_PANEL_BORDER,
                                  height - 2 * UI_PANEL_BORDER,
                                  fill_color);
}

void ui_draw_text_centered(const char *text,
                           int x,
                           int y,
                           int width,
                           int height,
                           uint32_t color,
                           uint8_t scale) {
  if (text == NULL || width <= 0 || height <= 0 || scale == 0) {
    return;
  }

  int text_width = font_text_width(text, scale);
  int text_height = FONT_HEIGHT * scale;

  int text_x = x + (width - text_width) / 2;
  int text_y = y + (height - text_height) / 2;

  font_draw_text(text, text_x, text_y, color, scale);
}

void ui_draw_button(const Button *button,
                    const char *text,
                    uint32_t normal_color,
                    uint32_t hover_color,
                    uint32_t text_color) {
  if (button == NULL) {
    return;
  }

  uint32_t fill_color = button->is_hovered ? hover_color : normal_color;

  if (button->width <= 2 * UI_BUTTON_BORDER || button->height <= 2 * UI_BUTTON_BORDER) {
    return;
  }

  graphics_draw_rectangle_clipped(button->x,
                                  button->y,
                                  button->width,
                                  button->height,
                                  COLOR_WHITE);

  graphics_draw_rectangle_clipped(button->x + UI_BUTTON_BORDER,
                                  button->y + UI_BUTTON_BORDER,
                                  button->width - 2 * UI_BUTTON_BORDER,
                                  button->height - 2 * UI_BUTTON_BORDER,
                                  fill_color);

  if (text != NULL) {
    ui_draw_text_centered(text,
                          button->x,
                          button->y,
                          button->width,
                          button->height,
                          text_color,
                          UI_BUTTON_TEXT_SCALE);
  }
}

void ui_draw_cursor(const InputState *input) {
  if (input == NULL) {
    return;
  }

  graphics_draw_rectangle_clipped(input->mouse_x,
                                  input->mouse_y,
                                  UI_CURSOR_SIZE,
                                  2,
                                  COLOR_WHITE);

  graphics_draw_rectangle_clipped(input->mouse_x,
                                  input->mouse_y,
                                  2,
                                  UI_CURSOR_SIZE,
                                  COLOR_WHITE);

  graphics_draw_rectangle_clipped(input->mouse_x + 3,
                                  input->mouse_y + 3,
                                  4,
                                  4,
                                  COLOR_RED);
}
