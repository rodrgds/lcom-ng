#pragma once

#include <lcom/lcf.h>
#include "src/Model/Elements/Sprite.h"

/**
 * @file bitmap.h
 * @brief Declares data structures and functions for handling BMP image loading.
 *
 * Defines the headers used to parse BMP image files and convert them
 * into sprites usable within the graphical system.
 */

/**
 * @struct BitmapFileHeader
 * @brief Represents the header of a BMP file.
 *
 * Contains metadata about the BMP file such as its size and the offset where the actual pixel data starts.
 */

typedef struct
{
    unsigned short type;   // specifies the file type
    unsigned int size;     // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset;   // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

/**
 * @struct BitmapInfoHeader
 * @brief Contains detailed information about the BMP image.
 *
 * This structure holds image dimensions, color depth, compression, and more.
 */

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

/**
 * @struct Bitmap
 * @brief Represents a loaded BMP image.
 *
 * Stores both the header info and the raw pixel data.
 */

typedef struct
{
    BitmapInfoHeader bitmapInfoHeader;
    unsigned char *bitmapData;
} Bitmap;

/**
 * @brief Loads a BMP image and converts it into a sprite.
 *
 * Parses the BMP file located at `path` and allocates a `sprite_t`
 * containing the image data ready for rendering.
 *
 * @param path Path to the BMP file.
 * @param mode Graphics mode to interpret the image (e.g., 0x14C).
 * @return Pointer to the created `sprite_t`, or NULL on failure.
 */

sprite_t *bitmap_load(const char *path, uint16_t mode);
