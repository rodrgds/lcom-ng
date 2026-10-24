/**
 * @file config.h
 * @brief Configuration macros for the game.
 */

#ifndef CONFIG_H
#define CONFIG_H

#define VIDEO_MODE 0x105

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define TARGET_FPS 60

#define COLOR_BLACK   0x00
#define COLOR_BLUE    0x01
#define COLOR_GREEN   0x02
#define COLOR_CYAN    0x03
#define COLOR_RED     0x04
#define COLOR_MAGENTA 0x05
#define COLOR_BROWN   0x06
#define COLOR_YELLOW  0x0E
#define COLOR_WHITE   0x0F
#define COLOR_ORANGE  42

#define TRANSPARENT_COLOR 0xFFFFFE

#define PLAYER_WIDTH  48
#define PLAYER_HEIGHT 48

#define ENEMY_WIDTH   48
#define ENEMY_HEIGHT  48

#define BULLET_WIDTH  8
#define BULLET_HEIGHT 16

#define CURSOR_WIDTH  24
#define CURSOR_HEIGHT 24

#define BUTTON_WIDTH  240
#define BUTTON_HEIGHT 60

#define HUD_MARGIN_X 20
#define HUD_MARGIN_Y 20

#endif
