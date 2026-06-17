/**
 * @file sidebar.h
 * @brief Sidebar rendering and hit-test helpers for the gameplay screen.
 * @ingroup group_ui
 */

#ifndef PROJ_SIDEBAR_H
#define PROJ_SIDEBAR_H

#include <stdbool.h>
#include <stdint.h>

struct scene_t;

int scene_draw_sidebar(struct scene_t *scene);

bool scene_is_wave_status_hit(int16_t px, int16_t py);

bool scene_is_fast_forward_hit(int16_t px, int16_t py);

int scene_target_mode_arrow_hit(int16_t px, int16_t py);

typedef enum { BTN_NONE = -1, BTN_UPGRADE, BTN_SELL } context_btn_t;

context_btn_t scene_context_button_hit(int16_t px, int16_t py);

#endif
