#pragma once

#include <lcom/lcf.h>
#include "includes/Sprite.h"

typedef struct
{
    unsigned short type;   // specifies the file type
    unsigned int size;     // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset;   // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

typedef struct
{
    unsigned int size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bits;
    unsigned int compression;
    unsigned int image_size;
    int x_res;
    int y_res;
    unsigned int num_colors;
    unsigned int important_colors;
} BitmapInfoHeader;

typedef struct
{
    BitmapInfoHeader bitmapInfoHeader;
    unsigned char *bitmapData;
} Bitmap;

sprite_t *bitmap_load(const char *path, uint16_t mode);
