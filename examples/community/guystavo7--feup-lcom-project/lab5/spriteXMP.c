#include "spriteXMP.h" 
#include "terminal.h"
#include <stdint.h>

struct SpriteXPM {
    xpm_image_t* img;
	int offsetX;
	int offsetY;

    uint8_t* pixMap;
};

SpriteXPM* newSpriteXPM(xpm_map_t map, enum xpm_image_type type, int offsetX, int offsetY) {
    xpm_image_t* img = malloc(sizeof(xpm_image_t));
    uint8_t* pixMap = xpm_load(map, type, img);

    SpriteXPM* sprite = malloc(sizeof(SpriteXPM));
    sprite->img = img;
    sprite->pixMap = pixMap;
	sprite->offsetX = offsetX;
	sprite->offsetY = offsetY;

    return sprite;
}

void freeSpriteXPM(SpriteXPM* sprite) {
    free(sprite->pixMap);
    free(sprite->img);
    free(sprite);
}

static uint32_t getColorFromPixel(SpriteXPM* sprite, unsigned int bytesPerPixel, int x, int y) {
    uint32_t color = 0;
    long currentX = x * bytesPerPixel;
    long offset = currentX + y * bytesPerPixel * sprite->img->width;

    for (int i = bytesPerPixel - 1; i >= 0; i--) color = (color << 8) | sprite->pixMap[offset + i];
    return color;
}

static void blitSpriteXPM(SpriteXPM* sprite, Terminal* terminal, int x, int y, bool isFlipped, bool to_empty_buffer) {
    unsigned int bpp = getBytesPerPixel(terminal);
    for (uint16_t i = 0; i < sprite->img->height; i++) {
        for (uint16_t j = 0; j < sprite->img->width; j++) {
            uint32_t color = getColorFromPixel(sprite, bpp, j, i);
            if (color == 0 || color == xpm_transparency_color(XPM_8_8_8)) continue;

            int pixelX = isFlipped ? x + sprite->offsetX + sprite->img->width - 1 - j : x + sprite->offsetX + j;
            int pixelY = y + sprite->offsetY + i;
            if (to_empty_buffer)
                drawPixelInEmptyBuffer(terminal, pixelX, pixelY, color);
            else
                drawPixel(terminal, pixelX, pixelY, color);
        }
    }
}

void drawSpriteXPM(SpriteXPM* sprite, Terminal* terminal, int x, int y, bool isFlipped) {
    blitSpriteXPM(sprite, terminal, x, y, isFlipped, false);
}

void drawSpriteXPMToEmptyBuffer(SpriteXPM* sprite, Terminal* terminal, int x, int y, bool isFlipped) {
    blitSpriteXPM(sprite, terminal, x, y, isFlipped, true);
}

void generateEmptyBuffer(SpriteXPM* sprite, Terminal* terminal, int x, int y, bool isFlipped) {
    blitSpriteXPM(sprite, terminal, x, y, isFlipped, true);
    freeSpriteXPM(sprite);
}

enum xpm_image_type getSpriteType(SpriteXPM* sprite) {
    return sprite->img->type;
}

uint16_t getSpriteWidth(SpriteXPM* sprite) {
    return sprite->img->width;
}

uint16_t getSpriteHeight(SpriteXPM* sprite) {
    return sprite->img->height;
}

size_t getSpriteSize(SpriteXPM* sprite) {
    return sprite->img->size;
}
