#ifndef CUT_PROFILE_H
#define CUT_PROFILE_H

#include "sprite.h"

#define CUT_DIRECTION_COUNT 8
#define CUT_OFFSET_COUNT 3
#define CUT_PROFILE_COUNT (CUT_DIRECTION_COUNT * CUT_OFFSET_COUNT)

/* A generated line in sprite-local coordinates, measured from its center. */
typedef struct {
    double nx, ny, offset;
} cut_profile_t;

typedef struct {
    uint16_t width, height;
    cut_profile_t profiles[CUT_PROFILE_COUNT];
} cut_profile_set_t;

/* Cropped fragment and its top-left offset within the original image. */
typedef struct {
    sprite_t *sprite;
    int x, y;
} cut_piece_t;

/* Returns NULL for invalid types, dimensions, or a stationary slash. */
const cut_profile_t *cut_profile_select(uint8_t type, const sprite_t *sprite,
                                       point_t start, point_t end);

/* Outputs own their sprites on success; source and outputs survive failure. */
int cut_profile_split(const sprite_t *source, const cut_profile_t *profile,
                      cut_piece_t pieces[2]);

#endif
