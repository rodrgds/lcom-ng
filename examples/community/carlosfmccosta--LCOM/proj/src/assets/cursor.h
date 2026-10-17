/**
 * @file cursor.h
 * @brief Contains XPM data for cursor sprites used in the game.
 *
 * Provides XPM pixel maps for different cursor states:
 * - The default cursor for menus and gameplay.
 * - Animated cursor sprites for text input (e.g., name entry), creating a blinking effect.
 */

#ifndef __CURSOR_H
#define __CURSOR_H

/**
 * @brief XPM data for the default cursor sprite.
 *
 * Used as the standard cursor in menus and during gameplay.
 */
static const char *cursor_xpm[] = {
  "10 10 2",
  "  0",
  ". 20",
  "    ..    ",
  "    ..    ",
  "    ..    ",
  "    ..    ",
  "....  ....",
  "....  ....",
  "    ..    ",
  "    ..    ",
  "    ..    ",
  "    ..    "};

 /**
 * @brief XPM data for the blinking cursor sprite (state 1).
 *
 * Used when the user is entering text, alternates with cursor_xpm_3 for blinking effect.
 */
static const char *cursor_xpm_2[] = {
  "10 10 2",
  "  0",
  ". 20",
  "..        ",
  "..        ",
  "..        ",
  "..        ",
  "..        ",
  "..        ",
  "..        ",
  "..        ",
  "..        ",
  "..        "};

 /**
 * @brief XPM data for the blinking cursor sprite (state 2).
 *
 * Used when the user is entering text, alternates with cursor_xpm_2 for blinking effect.
 */
static const char *cursor_xpm_3[] = {
  "10 10 2",
  "  0",
  ". 20",
  "          ",
  "          ",
  "          ",
  "          ",
  "          ",
  "          ",
  "          ",
  "          ",
  "          ",
  "          "};


#endif //__CURSOR_H
