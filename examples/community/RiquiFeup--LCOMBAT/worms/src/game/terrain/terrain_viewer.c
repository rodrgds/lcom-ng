#include "terrain_viewer.h"

#include <stddef.h>

#include "terrain.h"
#include "../../config.h"
#include "../../engine/graphics/renderer.h"
#include "../../assets/assets.h"

typedef struct {
    const uint32_t *grass_colors;
    int grass_w;
    int grass_h;
    const uint32_t *dirt_colors;
    int dirt_w;
    int dirt_h;
} TerrainTextureContext;

// Finds where the source grass texture begins in a texture column.
static int find_texture_surface_y(const uint32_t *colors, int texture_w, int texture_h, int texture_x) {
    for (int row = 0; row < texture_h; row++) {
        if (colors[texture_x + row * texture_w] != TRANSPARENT_COLOR) return row;
    }

    return -1;
}

// Samples one texture pixel and falls back when the sampled pixel is transparent.
static uint32_t sample_texture(const uint32_t *colors, int texture_w, int texture_h, int texture_x, int texture_y, uint32_t fallback) {
    if (texture_x >= 0 && texture_x < texture_w && texture_y >= 0 && texture_y < texture_h) {
        uint32_t color = colors[texture_x + texture_y * texture_w];
        if (color != TRANSPARENT_COLOR) return color;
    }

    return fallback;
}

// Uses depth for the grass cap and world coordinates for one continuous dirt texture.
static uint32_t terrain_texture_color(int col, int row, int run_depth, void *context) {
    TerrainTextureContext *texture = context;
    if (texture == NULL) return COLOR_TERRAIN_DIRT;

    if (run_depth < TERRAIN_GRASS_DEPTH) {
        int texture_x = TERRAIN_GRASS_TEXTURE_X + (col % TERRAIN_GRASS_TEXTURE_W);
        int surface_y = find_texture_surface_y(texture->grass_colors,
                                               texture->grass_w,
                                               texture->grass_h,
                                               texture_x);
        if (surface_y >= 0) {
            return sample_texture(texture->grass_colors,
                                  texture->grass_w,
                                  texture->grass_h,
                                  texture_x,
                                  surface_y + run_depth,
                                  COLOR_TERRAIN_GRASS);
        }
        return COLOR_TERRAIN_GRASS;
    }

    return sample_texture(texture->dirt_colors,
                          texture->dirt_w,
                          texture->dirt_h,
                          col % texture->dirt_w,
                          row % texture->dirt_h,
                          COLOR_TERRAIN_DIRT);
}

int terrain_viewer_draw(void) {
    const Sprite *grass = assets_get_terrain();
    const Sprite *dirt = assets_get_terrain_dirt();
    const uint8_t *mask = terrain_get_map();
    TerrainTextureContext texture;

    if (grass == NULL || dirt == NULL || mask == NULL) return 1;

    texture.grass_colors = sprite_get_colors(grass);
    texture.grass_w = sprite_get_width(grass);
    texture.grass_h = sprite_get_height(grass);
    texture.dirt_colors = sprite_get_colors(dirt);
    texture.dirt_w = sprite_get_width(dirt);
    texture.dirt_h = sprite_get_height(dirt);

    if (texture.grass_colors == NULL || texture.dirt_colors == NULL ||
        texture.grass_w <= 0 || texture.grass_h <= 0 ||
        texture.dirt_w <= 0 || texture.dirt_h <= 0) {
        return 1;
    }

    return renderer_draw_masked_pixels(0, 0, mask, TERRAIN_WIDTH, TERRAIN_HEIGHT,
                                       terrain_texture_color, &texture);
}
