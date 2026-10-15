#include "terrain.h"

#include <stdlib.h>
#include <string.h>

#include "../../config.h"
#include "../../assets/assets.h"
#include "../../engine/graphics/sprite.h"

static uint8_t s_terrain_map[TERRAIN_WIDTH][TERRAIN_HEIGHT];

// Checks whether a map coordinate is inside the terrain mask.
static inline bool is_in_bounds(int x, int y) {
    return (x >= 0 && x < TERRAIN_WIDTH && y >= 0 && y < TERRAIN_HEIGHT);
}

// Keeps generated surface heights inside playable terrain bounds.
static int clamp_height(int height) {
    if (height < TERRAIN_MIN_HEIGHT) return TERRAIN_MIN_HEIGHT;
    if (height > TERRAIN_MAX_HEIGHT) return TERRAIN_MAX_HEIGHT;
    return height;
}

// Returns a random integer in the inclusive [min, max] range.
static int random_between(int min, int max) {
    if (max <= min) return min;
    return min + (rand() % (max - min + 1));
}

int terrain_init(void) {
    const Sprite *terrain = assets_get_terrain();
    const uint32_t *colors;
    uint16_t width;
    uint16_t height;

    if (terrain == NULL) return 1;

    colors = sprite_get_colors(terrain);
    width = sprite_get_width(terrain);
    height = sprite_get_height(terrain);
    if (colors == NULL) return 1;

    memset(s_terrain_map, 0, sizeof(s_terrain_map));

    for (uint16_t x = 0; x < width && x < TERRAIN_WIDTH; x++) {
        for (uint16_t y = 0; y < height && y < TERRAIN_HEIGHT; y++) {
            s_terrain_map[x][y] = (colors[x + y * width] == TRANSPARENT_COLOR) ? 0 : 1;
        }
    }

    return 0;
}

// Builds rolling terrain by interpolating between random target heights.
static void generate_heightmap(int *heights) {
    if (heights == NULL) return;

    int x = 0;
    int current = clamp_height(TERRAIN_BASE_HEIGHT +
                               random_between(-(TERRAIN_VARIATION / 2),
                                              TERRAIN_VARIATION / 2));

    while (x < TERRAIN_WIDTH) {
        int segment_width = random_between(TERRAIN_SEGMENT_MIN_WIDTH,
                                           TERRAIN_SEGMENT_MAX_WIDTH);
        int target = clamp_height(TERRAIN_BASE_HEIGHT +
                                  random_between(-(TERRAIN_VARIATION / 2),
                                                 TERRAIN_VARIATION / 2));

        if (x + segment_width > TERRAIN_WIDTH) {
            segment_width = TERRAIN_WIDTH - x;
        }

        for (int i = 0; i < segment_width; i++) {
            int t = (segment_width <= 1) ? 0 : i;
            int interpolated = current + ((target - current) * t) / segment_width;
            int noise = random_between(-TERRAIN_NOISE_AMPLITUDE,
                                       TERRAIN_NOISE_AMPLITUDE);
            heights[x + i] = clamp_height(interpolated + noise);
        }

        current = target;
        x += segment_width;
    }
}

// Applies local averaging to remove noisy one-column spikes.
static void smooth_heightmap(int *heights, int passes) {
    if (heights == NULL || passes <= 0) return;

    int smoothed[TERRAIN_WIDTH];

    for (int pass = 0; pass < passes; pass++) {
        smoothed[0] = heights[0];
        smoothed[TERRAIN_WIDTH - 1] = heights[TERRAIN_WIDTH - 1];

        for (int x = 1; x < TERRAIN_WIDTH - 1; x++) {
            smoothed[x] = (heights[x - 1] + heights[x] + heights[x + 1]) / 3;
        }

        for (int x = 0; x < TERRAIN_WIDTH; x++) {
            heights[x] = clamp_height(smoothed[x]);
        }
    }
}

// Adds a small number of flat playable areas at random positions.
static void apply_platforms(int *heights) {
    if (heights == NULL) return;

    for (int platform = 0; platform < TERRAIN_PLATFORM_COUNT; platform++) {
        int start = random_between(0, TERRAIN_WIDTH - TERRAIN_PLATFORM_MIN_WIDTH);
        int width = random_between(TERRAIN_PLATFORM_MIN_WIDTH,
                                   TERRAIN_PLATFORM_MAX_WIDTH);
        int end = start + width;
        if (end > TERRAIN_WIDTH) end = TERRAIN_WIDTH;

        int base = heights[start];
        for (int px = start; px < end; px++) {
            heights[px] = base;
        }
    }
}

// Adds abrupt height offsets to create occasional vertical terrain edges.
static void apply_cliffs(int *heights) {
    if (heights == NULL) return;

    for (int cliff = 0; cliff < TERRAIN_CLIFF_COUNT; cliff++) {
        int x = random_between(TERRAIN_SEGMENT_MIN_WIDTH,
                               TERRAIN_WIDTH - TERRAIN_SEGMENT_MIN_WIDTH);
        int direction = (rand() % 2 == 0) ? -1 : 1;
        int delta = direction * random_between(TERRAIN_CLIFF_MIN_HEIGHT,
                                               TERRAIN_CLIFF_MAX_HEIGHT);

        for (int px = x; px < TERRAIN_WIDTH; px++) {
            heights[px] = clamp_height(heights[px] + delta);
        }
    }
}

// Carves random pre-spawn craters into the generated terrain mask.
static void apply_craters(const int *heights) {
    if (heights == NULL) return;

    int crater_count = random_between(TERRAIN_CRATER_MIN_COUNT,
                                      TERRAIN_CRATER_MAX_COUNT);

    for (int crater = 0; crater < crater_count; crater++) {
        int radius = random_between(TERRAIN_CRATER_MIN_RADIUS,
                                    TERRAIN_CRATER_MAX_RADIUS);
        int x = random_between(radius, TERRAIN_WIDTH - radius - 1);
        int y = heights[x] + TERRAIN_CRATER_DEPTH;

        if (y >= WATER_LEVEL_Y - radius) {
            y = WATER_LEVEL_Y - radius - 1;
        }

        terrain_explode(x, y, radius);
    }
}

int terrain_generate_procedural(uint32_t seed) {
    if (seed == 0) return 1;

    int heights[TERRAIN_WIDTH];

    srand(seed);
    memset(s_terrain_map, 0, sizeof(s_terrain_map));

    generate_heightmap(heights);
    smooth_heightmap(heights, TERRAIN_SMOOTH_PASSES);
    apply_cliffs(heights);
    apply_platforms(heights);

    for (int x = 0; x < TERRAIN_WIDTH; x++) {
        int ground_y = heights[x];
        if (ground_y < 0) ground_y = 0;
        if (ground_y >= TERRAIN_HEIGHT) ground_y = TERRAIN_HEIGHT - 1;

        for (int y = ground_y; y < TERRAIN_HEIGHT; y++) {
            s_terrain_map[x][y] = 1;
        }
    }

    apply_craters(heights);

    return 0;
}

void terrain_cleanup(void) {
    /* Terrain data is stored statically and has no dynamic resources to free. */
}

bool terrain_collides(int x, int y) {
    if (!is_in_bounds(x, y)) return false;
    return s_terrain_map[x][y] == 1;
}

bool terrain_collides_hitbox(Hitbox hitbox) {
    if (hitbox.width == 0 || hitbox.height == 0) return false;

    uint32_t start_x = hitbox.x;
    uint32_t start_y = hitbox.y;
    uint32_t end_x = start_x + hitbox.width;
    uint32_t end_y = start_y + hitbox.height;

    if (start_x >= TERRAIN_WIDTH || start_y >= TERRAIN_HEIGHT) return false;
    if (end_x > TERRAIN_WIDTH) end_x = TERRAIN_WIDTH;
    if (end_y > TERRAIN_HEIGHT) end_y = TERRAIN_HEIGHT;

    for (uint32_t x = start_x; x < end_x; x++) {
        for (uint32_t y = start_y; y < end_y; y++) {
            if (s_terrain_map[x][y] == 1) return true;
        }
    }

    return false;
}

void terrain_explode(int cx, int cy, int radius) {
    for (int x = cx - radius; x <= cx + radius; x++) {
        for (int y = cy - radius; y <= cy + radius; y++) {
            if (!is_in_bounds(x, y)) continue;

            if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= (radius * radius)) {
                s_terrain_map[x][y] = 0;
            }
        }
    }
}

const uint8_t *terrain_get_map(void) {
    return (const uint8_t *) s_terrain_map;
}
