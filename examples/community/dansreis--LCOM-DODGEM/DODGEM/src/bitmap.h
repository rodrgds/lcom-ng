#pragma once
/** @defgroup Bitmap Bitmap
 * @{
 * Functions for manipulating bitmaps
 */

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
 * @brief Loads a bmp image (Made by Henrique Ferrolho)
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position (Made by Henrique Ferrolho)
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
void drawBitmap(Bitmap* bitmap, int x, int y, Alignment alignment);

/**
 * @brief Draws a number
 *
 * @param bmp bitmap of the image
 * @param x the x of the coordinates in the buffer
 * @param y the y of the coordinates in the buffer
 * @param number the number to draw
 * @param alignment image alignment
 */
void drawBitmapNumber(Bitmap* bmp, int x, int y,int number, Alignment alignment);

/**
 * @brief Draws a Score (white letters)
 *
 * @param x_inicial destiny x coord
 * @param y_inicial destiny y coord
 * @param segundos seconds of the score
 * @param centesimas of the score
 */
void drawWhiteScore(int x_inicial,int y_inicial,int segundos,int centesimas);

/**
 * @brief Draws a Score (Black letters)
 *
 * @param x_inicial destiny x coord
 * @param y_inicial destiny y coord
 * @param segundos seconds of the score
 * @param centesimas of the score
 */
void drawBlackScore(int x_inicial,int y_inicial,int segundos,int centesimas);

/**
 * @brief draws a part of an image
 *
 * @param bmp bitmap of the image
 * @param x destiny x coord
 * @param y destiny y coord
 * @param x_inicial x start of the image pretended
 * @param y_inicial y start of the image pretended
 * @param x_final x end of the image pretended
 * @param y_final y end of the image pretended
 * @param alignment image alignment
 */
void drawPart(Bitmap* bmp, int x, int y, int x_inicial, int y_inicial, int x_final, int y_final, Alignment alignment); //draw part of an image

/**
 * @brief Returns the width of a number to draw
 *
 * @param number pretended
 * @return the width of the number
 */
int number_draw_width(int number);//return the number width to draw

/**
 * @brief Destroys the given bitmap, freeing all resources used by it. (Made by Henrique Ferrolho)
 *
 * @param bitmap bitmap to be destroyed
 */
void deleteBitmap(Bitmap* bmp);

/**
 * @brief Draw a name
 *
 * @param name pretended
 * @param x destiny x coord
 * @param y destiny y coord
 * @return the x it ended
 */
int drawPlayerName(char* name,int x,int y); // desenha o nome do player no ecra e retorna onde está o x(ou seja, o comprimento do nome)

/**
 * @brief Draws a text after losing a game
 *
 * @param segundos of the record
 * @param centesimas of the record
 */
void drawLosingText(int segundos, int centesimas);

/**
 * @brief draw a letter
 *
 * @param bmp the bitmap of the alphabet
 * @param x destiny x coord
 * @param y destiny y coord
 * @param letter to draw
 * @param alignment image alignment
 */
void drawBitmapLetter(Bitmap* bmp, int x, int y,char letter, Alignment alignment);

/**
 * @brief draw highscores
 *
 * @param name of the player
 * @param segundos record seconds
 * @param centesimas record
 * @param x destiny x coord
 * @param y destiny y coord
 */
void drawHighscores(char*name,int segundos,int centesimas,int x,int y); // desenho o score no ecra

/**
 * @brief draw a Date
 *
 * @param day of the month
 * @param month
 * @param year
 * @param hours
 * @param min minutes
 * @param sec seconds
 * @param x destiny x coord
 * @param y destiny y coord
 */
void drawData(int day, int month, int year, int hours, int min, int sec, int x, int y);

/**@}*/
