#include "pvz_platform.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PVZ_TRANSPARENT_COLOR 0x00FF00FFu

uint32_t xpm_transparency_color(enum xpm_image_type type) {
    (void)type;
    return PVZ_TRANSPARENT_COLOR;
}

static int parse_header(const char *line, int *width, int *height, int *colors, int *cpp) {
    return sscanf(line, "%d %d %d %d", width, height, colors, cpp) == 4;
}

static uint32_t parse_color(const char *s, bool *transparent) {
    *transparent = false;
    while (*s != '\0' && isspace((unsigned char)*s)) s++;
    if (strncmp(s, "None", 4) == 0) {
        *transparent = true;
        return PVZ_TRANSPARENT_COLOR;
    }
    if (*s != '#') return 0;

    unsigned r = 0;
    unsigned g = 0;
    unsigned b = 0;
    if (sscanf(s + 1, "%02x%02x%02x", &r, &g, &b) != 3) return 0;
    return (b & 0xFFu) | ((g & 0xFFu) << 8) | ((r & 0xFFu) << 16);
}

static int key_matches(const char *a, const char *b, int cpp) {
    return strncmp(a, b, (size_t)cpp) == 0;
}

static unsigned direct_key(const char *key, int cpp) {
    if (cpp == 1) return (unsigned char)key[0];
    if (cpp == 2) {
        return ((unsigned)(unsigned char)key[0] << 8) | (unsigned char)key[1];
    }
    return ((unsigned)(unsigned char)key[0] << 16) |
           ((unsigned)(unsigned char)key[1] << 8) |
           (unsigned char)key[2];
}

uint8_t *xpm_load(xpm_map_t xpm, enum xpm_image_type type, xpm_image_t *img) {
    (void)type;
    if (xpm == NULL || img == NULL) return NULL;

    int width = 0;
    int height = 0;
    int colors = 0;
    int cpp = 0;
    if (!parse_header(xpm[0], &width, &height, &colors, &cpp) ||
        width <= 0 || height <= 0 || colors <= 0 || cpp <= 0) {
        return NULL;
    }

    const bool use_direct_table = cpp <= 3;
    char *keys = NULL;
    uint32_t *table = NULL;
    uint32_t *direct_table = NULL;

    if (use_direct_table) {
        size_t direct_entries = (size_t)1u << (8u * (unsigned)cpp);
        direct_table = (uint32_t *)calloc(direct_entries, sizeof(uint32_t));
    } else {
        keys = (char *)calloc((size_t)colors, (size_t)cpp);
        table = (uint32_t *)calloc((size_t)colors, sizeof(uint32_t));
    }

    if ((use_direct_table && direct_table == NULL) ||
        (!use_direct_table && (keys == NULL || table == NULL))) {
        free(keys);
        free(table);
        free(direct_table);
        return NULL;
    }

    for (int i = 0; i < colors; i++) {
        const char *line = xpm[1 + i];
        const char *color = strstr(line + cpp, " c ");
        if (color == NULL) color = strstr(line + cpp, "\tc ");
        bool transparent = false;
        uint32_t value = color == NULL ? 0 : parse_color(color + 3, &transparent);
        if (transparent) value = PVZ_TRANSPARENT_COLOR;

        if (use_direct_table) {
            direct_table[direct_key(line, cpp)] = value;
        } else {
            memcpy(keys + (size_t)i * cpp, line, (size_t)cpp);
            table[i] = value;
        }
    }

    uint8_t *pixels = (uint8_t *)calloc((size_t)width * height * 3u, 1);
    if (pixels == NULL) {
        free(keys);
        free(table);
        return NULL;
    }

    for (int row = 0; row < height; row++) {
        const char *line = xpm[1 + colors + row];
        for (int col = 0; col < width; col++) {
            const char *key = line + col * cpp;
            uint32_t color = 0;
            if (use_direct_table) {
                color = direct_table[direct_key(key, cpp)];
            } else {
                for (int i = 0; i < colors; i++) {
                    if (key_matches(key, keys + (size_t)i * cpp, cpp)) {
                        color = table[i];
                        break;
                    }
                }
            }
            uint8_t *dst = pixels + ((size_t)row * width + col) * 3u;
            dst[0] = (uint8_t)(color & 0xFFu);
            dst[1] = (uint8_t)((color >> 8) & 0xFFu);
            dst[2] = (uint8_t)((color >> 16) & 0xFFu);
        }
    }

    free(keys);
    free(table);
    free(direct_table);
    img->width = (uint16_t)width;
    img->height = (uint16_t)height;
    return pixels;
}
