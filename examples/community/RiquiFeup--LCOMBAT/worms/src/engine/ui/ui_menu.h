#ifndef UI_MENU_H
#define UI_MENU_H

#include <stdint.h>
#include <stdbool.h>
#include "../graphics/font.h"

/** Maximum number of buttons a single UiMenu may hold. */
#define UI_MENU_MAX_BUTTONS  8

typedef struct UiMenu UiMenu;

/**
 * @brief Creates a UiMenu with a vertical button stack.
 *
 * Button positions are computed from center_x and y_start. If title is
 * non-NULL a title panel is drawn above the first button at height MENU_TITLE_H.
 *
 * @param center_x   Horizontal centre of the button column.
 * @param y_start    Y of the title (if any) or first button.
 * @param btn_w      Width of every button.
 * @param btn_h      Height of every button.
 * @param btn_gap    Vertical gap between consecutive buttons.
 * @param title      Optional title string (NULL = no title panel).
 * @param labels     Array of btn_count label strings (borrowed — must outlive UiMenu).
 * @param btn_count  Number of buttons (1 – UI_MENU_MAX_BUTTONS).
 * @return Pointer to the new UiMenu, or NULL on failure.
 */
UiMenu *ui_menu_create(int center_x, int y_start,
                       int btn_w, int btn_h, int btn_gap,
                       const char *title,
                       const char *const labels[], int btn_count);

/**
 * @brief Frees the UiMenu. Does not free label strings.
 */
void ui_menu_destroy(UiMenu *m);

/**
 * @brief Draws the title panel (if any) and all buttons.
 */
void ui_menu_draw(const UiMenu *m, const Font *f);

/**
 * @brief Processes input and updates the selection.
 *
 * @param m         Menu to update. NULL returns -1.
 * @param mx, my    Current mouse position.
 * @param lb_click  True if the left button was clicked this frame.
 * @param nav_delta Keyboard navigation: -1 up, 0 none, +1 down.
 * @param confirm   True if Enter or Space was pressed.
 * @return Index of the confirmed button (0-based), or -1 if no action.
 */
int ui_menu_update(UiMenu *m, int16_t mx, int16_t my,
                   bool lb_click, int nav_delta, bool confirm);

#endif /* UI_MENU_H */
