/**
 * @file constants.h
 * @brief Game and graphics configuration constants.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#pragma once

// ======================
// Screen Configuration
// ======================

/** @brief Horizontal resolution */
#define SCREEN_WIDTH        1024
/** @brief Vertical resolution */
#define SCREEN_HEIGHT       768
/** @brief Bits per pixel (indexed color) */
#define BITS_PER_PIXEL      8
/** @brief VBE mode for 1024x768@16bit */
#define VIDEO_MODE          0x105

// ======================
// Gameplay Constants
// ======================
#define PLAYER_SPEED       5       /**< Speed multiplier */
#define BULLET_SPEED       15      /**< Speed multiplier */
#define ENEMY_SPEED        2       /**< Base speed (varies by enemy) */
#define PLAYER_HP          100     /**< Initial health points */

// ======================
// Entity Limits
// ======================
#define MAX_ENEMIES        30      /**< Max active enemies */
#define MAX_BULLETS        50      /**< Max active bullets */
#define MAX_STARS          200     /**< Parallax starfield particles */
#define MAX_EXPLOSIONS     10      /**< Simultaneous explosion animations */

// ======================
// Timing Constants
// ======================
#define ENEMY_SPAWN_RATE   60      /**< Frames between enemy spawns */
#define STARFIELD_SPEED    1       /**< Base star movement speed */
#define FPS                60      /**< Target frames per second */

// ======================
// Color Definitions
// ======================
#define COLOR_BLACK        0x0000  /**< RGB(0, 0, 0) */
#define COLOR_WHITE        0xFFFF  /**< RGB(31, 63, 31) */
#define COLOR_PLAYER       0x07FF  /**< Cyan (RGB(0, 63, 31)) */
#define COLOR_ENEMY        0xF800  /**< Red (RGB(31, 0, 0)) */
#define COLOR_BULLET       0xFFE0  /**< Yellow (RGB(31, 63, 0)) */
#define COLOR_STAR         0x8410  /**< Gray (RGB(16, 32, 16)) */

// ======================
// Sprite Dimensions
// ======================
#define PLAYER_WIDTH       32
#define PLAYER_HEIGHT      32
#define ENEMY_WIDTH        24
#define ENEMY_HEIGHT       24
#define BULLET_WIDTH       8
#define BULLET_HEIGHT      16

/** @brief Frame interval for 60 FPS */
#define FRAME_INTERVAL ((int)(sys_hz() / 60))


#define FONT_SCALE 3                 // 8x8 scaled to 32x32
#define FONT_SIZE (8 * FONT_SCALE)
#define FONT_CHAR_WIDTH  (8 * FONT_SCALE)
#define FONT_CHAR_HEIGHT (8 * FONT_SCALE)

#define HUD_MARGIN_LEFT 20
#define HUD_MARGIN_BOTTOM 20
#define HUD_LINE_SPACING (FONT_CHAR_HEIGHT + 4)


#endif // CONSTANTS_H
