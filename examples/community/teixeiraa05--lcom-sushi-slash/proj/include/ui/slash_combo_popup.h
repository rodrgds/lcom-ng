#ifndef __SLASH_COMBO_POPUP_H
#define __SLASH_COMBO_POPUP_H

#include "../game/slash_combo.h"
#include "../init.h"

/**
 * @file slash_combo_popup.h
 * @brief Declares the quick-slash combo popup drawing API.
 */

/** @brief Blue color used for the combo popup text. */
#define SLASH_COMBO_POPUP_COLOR 0x3399FF

/**
 * @brief Draw the active combo popup, if visible.
 *
 * @param combo Combo state containing the popup data.
 * @param assets Shared assets containing the game font.
 */
void ui_combo_popup_draw(const slash_combo_t *combo, const Assets *assets);

#endif /* __SLASH_COMBO_POPUP_H */
