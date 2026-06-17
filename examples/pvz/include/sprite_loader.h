#ifndef SPRITE_LOADER_H
#define SPRITE_LOADER_H

#include "sprites.h"

int sprite_load_frames(AnimSprite *sprite, xpm_map_t *xpms, int num_frames);
int sprite_load_frames_with_dimensions(AnimSprite *sprite,
                                       xpm_map_t *xpms,
                                       uint16_t *widths,
                                       uint16_t *heights,
                                       int num_frames);
int sprite_load_single(AnimSprite *sprite, xpm_map_t xpm);
void sprite_free_frames(AnimSprite *sprite);
uint16_t sprite_frame_width(const AnimSprite *sprite, const uint16_t *widths, int frame);
uint16_t sprite_frame_height(const AnimSprite *sprite, const uint16_t *heights, int frame);

#endif
