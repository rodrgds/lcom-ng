#ifndef __GRAPHICS_H
#define __GRAPHICS_H

/** @defgroup graphics graphics
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 * @brief paints a pixel
 * @param x coordinate X
 * @param y coordinate Y
 * @param color color to be written on the memory
 */
void set_pixel(unsigned short x, unsigned short y, unsigned short color);

/**
 * @brief paints a square
 * @param x coordinate X
 * @param y coordinate Y
 * @param size size of the side of the square
 * @param color color of the square
 */
int vg_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color);

/**
 * @brief paints a line
 * @param xi start point X
 * @param yi start point Y
 * @param xf end point X
 * @param yf end point Y
 * @param color color to be writen on memory (line color)
 * @return 0 upon success, non-zero otherwise
 */
int vg_line(unsigned short xi, unsigned short yi,  unsigned short xf, unsigned short yf, unsigned long color);

/**
 * @brief makes the buffer black
 */
void clear_screen();


/**
 * @brief returns the value set in vg_init
 * @return size of horizontal in pixels
 */
unsigned get_hres();

/**
 * @brief returns the value set in vg_init
 * @return size of vertical in pixels
 */
unsigned get_vres();

/**
 * @brief returns the value set in vg_init
 * @return number of bits per pixel (16)
 */
int get_bitsPerPixel();

/**
 * @brief gets the double buffer
 * @return pointer to the double buffer
 */
char * getGraphicsBuffer();

/**
 * @brief copies the double_buffer to the mouse_buffer
 */
void flip_Mouse();

/**
 * @brief copies the mouse_buffer to the video_mem to be displayed on screen
 */
void flip_Display();



/** @} end of graphics */
 
#endif /* __GRAPHICS_H */
