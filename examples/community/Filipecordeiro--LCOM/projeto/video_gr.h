#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

static char *video_mem;

#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES			1024
#define V_RES			768
#define BITS_PER_PIXEL	8

char rgb(short r, short g, short b);

/* Private global variables */

//static char *video_mem;		/* Process address to which VRAM is mapped */

unsigned h_res;			/* Horizontal screen resolution in pixels */
unsigned v_res;		/* Vertical screen resolution in pixels */
unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
unsigned long vram_size;


/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
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
 * @brief Draws a colored square
 *
 * @param address VRAM
 * @param x topleft X point
 * @param y topleft Y point
 * @param size length of the square side
 * @param color pixel colors
 * @return last mapped address
 */
int vg_drawSquare(char *address, unsigned short x, unsigned short y, unsigned short size, unsigned long color);

int vg_drawRectangle(char *address, unsigned short x, unsigned short y, unsigned short h, unsigned short w, unsigned long color);

int vg_clear(char *address);

 /**
 * @brief Draws a line based on 2 points and a color
 *
 * @param address VRAM
 * @param xi initial X point
 * @param yi initial Y point
 * @param xf final X point
 * @param xi final Y point
 * @param color pixel colors
 * @return last mapped address
 */
int vg_drawLine(char *address, unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);

 /**
 * @brief Draws a XPM
 *
 * @param address VRAM
 * @param xi initial X point
 * @param yi initial Y point
 * @param xpm xpm image
 * @return last mapped address
 */
int vg_drawXPM(char *address,unsigned short xi, unsigned short yi, char *xpm[]);

 /** @} end of video_gr */
 

#endif /* __VIDEO_GR_H */
