#ifndef _VIDEO_H
#define _VIDEO_H

#define FRAME_RATE    30

/*
@brief sets video mode

@return 0 if everything ok
*/
int set_mode(uint16_t mode);

/*
@brief draws a line

@param x x initial position

@param y y initial position

@param width width of the line

@param color color of the line
*/
void draw_line(uint16_t x, uint16_t y, uint16_t width, uint32_t color);

/*
@brief clears screen
*/

void clearScreen();

/*
@brief puts pixel in position x, y with color color

@param x position x

@param y position y

@param color color of the pixel

@return 0 if everything ok
*/

int putPixel(uint16_t x, uint16_t y, uint32_t color);

/*
@brief draws an image

@param imt image to draw

@param x0 position x to start

@param y0 position y to start
*/
void draw_sproot(xpm_image_t imt, int x0, int y0);

/*
@brief draws background

@param im image of the background
*/

void draw_background( xpm_image_t * im);

/*
@brief gets vertical resolution

@return vertical resolution
*/

int getVRes();

/*
@brief gets horizontal resolution

@return horizontal resolution
*/
int getHRes();


/*
@brief swaps the buffer in double buffer
*/

void swap_buffer();

#endif /*_VIDEO_H*/
