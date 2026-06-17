/**
 * @file layout.h
 * @brief Shared UI layout constants for the gameplay sidebar, context panel, and result overlays.
 * @ingroup group_ui
 *
 * Single source of truth for sidebar geometry, attacker enemy rows, and
 * the various button bars (pause, result, role selection).
 */

#ifndef PROJ_LAYOUT_H
#define PROJ_LAYOUT_H

/* Sidebar card geometry (defender mode). */
#define SIDEBAR_CARD_X        (MAP_WIDTH + 12)
#define SIDEBAR_CARD_WIDTH    172
#define SIDEBAR_SHOP_Y        8
#define SIDEBAR_SHOP_HEIGHT   208
#define SIDEBAR_CONTEXT_Y     (SIDEBAR_SHOP_Y + SIDEBAR_SHOP_HEIGHT + 8)
#define SIDEBAR_CONTEXT_HEIGHT 270
#define SIDEBAR_WAVE_Y        (SIDEBAR_CONTEXT_Y + SIDEBAR_CONTEXT_HEIGHT + 4)

/* Attacker mode shop extends the shop card to fit extra enemy rows. */
#define ATTACKER_SHOP_HEIGHT  364
#define ATTACKER_ENEMY_ROW_Y(row) ((uint16_t)(46U + (uint16_t)(row) * 82U))

/* Standard small button used by the sidebar (wave status, fast-forward). */
#define SIDEBAR_BTN_W         (34U * UI_BUTTON_SCALE)
#define SIDEBAR_BTN_H         (14U * UI_BUTTON_SCALE)

/* Context panel upgrade / sell button row geometry. */
#define CONTEXT_BTN_W         SIDEBAR_BTN_W
#define CONTEXT_BTN_H         SIDEBAR_BTN_H
#define CONTEXT_BTN_GAP       8
#define CONTEXT_BTN_BOTTOM_OFFSET 36

/* Pause menu overlay buttons. */
#define PAUSE_BUTTON_Y        298
#define PAUSE_BUTTON_WIDTH    70
#define PAUSE_BUTTON_HEIGHT   28
#define PAUSE_BUTTON_GAP      16
#define PAUSE_BUTTON_X        322

/* Result screen buttons (game over / victory). */
#define RESULT_BUTTON_Y       328
#define RESULT_BUTTON_WIDTH   70
#define RESULT_BUTTON_HEIGHT  28
#define RESULT_BUTTON_GAP     16

/* Multiplayer role selection buttons. */
#define ROLE_BUTTON_WIDTH     130
#define ROLE_BUTTON_HEIGHT    40
#define ROLE_BUTTON_GAP       24

#endif
