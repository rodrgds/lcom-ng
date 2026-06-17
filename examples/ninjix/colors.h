/**
 * @file colors.h
 * @brief Colour palette constants used throughout the game's rendering.
 * @ingroup group_scene
 *
 * Defines the hexadecimal colour values for map tiles, path, UI panels,
 * cards, text, and cursors. Colours are referenced by name rather than
 * hardcoded values in scene and drawing code.
 */

#ifndef PROJ_COLORS_H
#define PROJ_COLORS_H

#define COLOR_MAP_BORDER 0x334A2C
#define COLOR_SIDEBAR_BG 0x2F2017

#define COLOR_CARD_BORDER 0x6F5336
#define COLOR_CARD_SHADOW 0x3C271A
#define COLOR_CARD_FILL 0xF6D7B4
#define COLOR_CARD_INSET 0xE7BF93

#define COLOR_GRASS_BACKDROP 0x3A8E4F
#define COLOR_GRASS_DARK 0x1E6F50
#define COLOR_GRASS_LIGHT 0x5AC54F

#define COLOR_PATH_BORDER 0xE7BF93
#define COLOR_PATH_FILL 0xF7D7AE
#define COLOR_PATH_PEBBLE 0xD9AC77

#define COLOR_PANEL_BORDER_DARK 0x131313
#define COLOR_PANEL_BORDER_MID 0x8A4836
#define COLOR_PANEL_BORDER_INNER 0x5D2C28
#define COLOR_PANEL_BORDER_ACCENT 0x391F21
#define COLOR_PANEL_FILL 0xF6CA9F

#define COLOR_CHIP_FILL 0x4D6951
#define COLOR_MENU_FILL 0xB7BCC7
#define COLOR_ROCK_FILL 0x7B7B7B
#define COLOR_ROCK_SHADE 0x545454
#define COLOR_FLOWER_PINK 0xF68187
#define COLOR_FLOWER_YELLOW 0xFFEB57

#define COLOR_TEXT_DARK 0x131313
#define COLOR_TEXT_DIM 0x7A5C3B
#define COLOR_TEXT_SOFT 0xF7ECD9

#define COLOR_BG 0x3A8E4F
#define COLOR_CURSOR 0xffffff

#define COLOR_OPTION_NORMAL COLOR_TEXT_DARK
#define COLOR_OPTION_HOVER COLOR_GRASS_LIGHT
#define COLOR_OPTION_SELECTED COLOR_CARD_FILL

#endif
