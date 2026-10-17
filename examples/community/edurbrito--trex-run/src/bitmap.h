#pragma once

/** @defgroup Bitmap bitmap
 * @{
 *
 * Functions for using the bitmap assets
 */

//Pink = 11111(R) 000000(G) 111111(B) in mode 5.6.5
 #define pink_first_byte  0x1f
 #define pink_second_byte 0xf8

typedef enum { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT } Alignment;

typedef enum { REAL_BUFFER, SECOND_BUFFER, THIRD_BUFFER} Buffer;

typedef struct {
    unsigned short  type; // specifies the file type
    unsigned int    size; // specifies the size in bytes of the bitmap file
    unsigned int    reserved; // reserved; must be 0
    unsigned int    offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

typedef struct {
    unsigned int    size; // specifies the number of bytes required by the struct
    int             width; // specifies width in pixels
    int             height; // specifies height in pixels
    unsigned short  planes; // specifies the number of color planes, must be 1
    unsigned short  bits; // specifies the number of bit per pixel
    unsigned int    compression; // specifies the type of compression
    unsigned int    imageSize; // size of image in bytes
    int             xResolution; // number of pixels per meter in x axis
    int             yResolution; // number of pixels per meter in y axis
    unsigned int    nColors; // number of colors used by the bitmap
    unsigned int    importantColors; // number of colors that are important
} BitmapInfoHeader;

typedef struct {
    BitmapInfoHeader  bitmapInfoHeader;
    unsigned char*    bitmapData;
} Bitmap; //Bitmap Representation

/**
 * @brief Loads a bmp image
 *
 * @param filename Path of the image to be loaded
 * @return pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws a bitmap at the given position x,y
 *
 * @param bitmap bitmap to be drawn
 * @param x x-axis coordenate
 * @param y y-axis coordenate
 * @param alignment image alignment
 * @param buffer select Buffer to draw Bitmap
 */
void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment, Buffer buffer);

/**
 * @brief Draws a bitmap at the given position x,y to a 3rd buffer (for collisions purposes)
 * and checks for a collision in each pixel in the 3rd buffer
 *
 * @param bitmap bitmap to be drawn
 * @param x x-axis coordenate
 * @param y y-axis coordenate
 * @param alignment image alignment
 */
int drawBitmap_collision(Bitmap* bmp, int x, int y, Alignment alignment);

/**
 * @brief Destroys the given bitmap and frees the allocated memory
 *
 * @param bmp bitmap to be freed
 */
void deleteBitmap(Bitmap* bmp);
