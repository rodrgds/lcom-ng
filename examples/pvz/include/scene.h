#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>

#include "wave.h"

/* Draws the correct screen based on the current GameState */
void render_scene(void);

/* Ecrã de seleção de plantas */
void render_plant_selection(void);

void draw_string(uint16_t x, uint16_t y, const char *str, int scale, uint32_t color);
void draw_sun_counter(void);

/* Overlay de banner de wave — chamar por cima do gameplay quando ativo */
void draw_wave_banner(WaveBannerType type, int ticks_remaining);

#endif /* SCENE_H */
