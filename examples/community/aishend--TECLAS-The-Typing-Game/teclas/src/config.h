/**
 * @file config.h
 * @brief Game configuration types and color theme definitions.
 */
#ifndef _TECLAS_CONFIG_H_
#define _TECLAS_CONFIG_H_

#include <stdint.h>

/**
 * @brief Color palette for a UI theme.
 */
typedef struct {
    uint32_t bg;
    uint32_t untyped;
    uint32_t correct;
    uint32_t wrong_bg;
    uint32_t wrong_fg;
    uint32_t cur_bg;
    uint32_t cur_fg;
    uint32_t cur_bar;
    uint32_t timer;
    const char *name;
} ColorTheme;

/** @brief Number of available color themes. */
#define NUM_THEMES 10

/** @brief Number of selectable time limit options. */
#define NUM_TIME_OPTIONS 3

/** @brief Available color themes. */
extern const ColorTheme color_themes[NUM_THEMES];

/** @brief Available time limit values in seconds. */
extern const int time_options[NUM_TIME_OPTIONS];

/**
 * @brief User-selected game configuration.
 */
typedef struct {
    int time_idx;
    int theme_idx;
    char player_name[16];
} GameConfig;

#endif
