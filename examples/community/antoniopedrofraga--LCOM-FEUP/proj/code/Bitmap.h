#pragma once
/** @defgroup Bitmap Bitmap
 * @{
 * Functions for manipulating bitmaps
 * This code was developed by Henrique Ferrolho, a MIEIC student, it's a code that we adapted to our needs
 */

unsigned char * db;

typedef enum {
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
} Alignment;

typedef struct {
    unsigned short type; // specifies the file type
    unsigned int size; // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bits; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} BitmapInfoHeader;

/// Represents a Bitmap
typedef struct {
    BitmapInfoHeader bitmapInfoHeader;
    unsigned char* bitmapData;
} Bitmap;

/**
 * @brief Loads a bmp image
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
int drawBitmap_byLine(Bitmap* bitmap, int x, int y, Alignment alignment);
/**
 * @brief Draws an unscaled, unrotated bitmap at the given position, paiting pixel to pixel
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
int drawBitmap_byPixel(Bitmap* bmp, int x, int y, Alignment alignment);
/**
 * @brief Draws an unscaled, unrotated bitmap at the given position, paiting pixel to pixel, on double buffer
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 * @param double_buffer where the bitmap will be drawn
 */
int drawBitmap_onBoard(Bitmap* bmp, int x, int y, Alignment alignment,unsigned char**double_buffer);
/**
 * @brief Erase any number on given coords of a sudoku square
 *
 * @param bitmap Used to give size of a square
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 * @param double_buffer where the square will be erased
 * @param background Initial background given to draw initial pixels (squares without numbers)
 */
int erase_square(Bitmap* bmp, int x, int y, Alignment alignment,unsigned char**double_buffer,unsigned char*background);

/**
 * @brief Destroys the given bitmap, freeing all resources used by it.
 *
 * @param bitmap bitmap to be destroyed
 */
void deleteBitmap(Bitmap* bmp);

/**
 * @brief returns double buffering memory address
 */

unsigned char *double_buffering();

/**@}*/
