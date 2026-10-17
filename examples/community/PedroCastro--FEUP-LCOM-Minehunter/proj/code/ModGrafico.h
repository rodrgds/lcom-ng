#ifndef __MODGRAFICO_H
#define __MODGRAFICO_H

#include <minix/syslib.h>
#include <minix/drivers.h>
//#include <sys/types.h>

#define HEIGHT_MINE_EXPERT 32
#define WIDTH_MINE_EXPERT 32
#define HEIGHT_NUMBER_EXPERT 32
#define WIDTH_NUMBER_EXPERT 32
#define HEIGHT_FLAG_EXPERT 32
#define WIDTH_FLAG_EXPERT 32
#define HEIGHT_WATER_EXPERT 32
#define WIDTH_WATER_EXPERT 32
#define HRES             1024
#define VRES		  768
#define VRAM_PHYS_ADDR	0xF0000000
#define videoMemSize HRES*VRES
static uint16_t* video_mem;
uint16_t *buffer;
uint16_t *bufferRato;
#define PATH "home/lcom/proj/code/res/images/"

#define BITS_PER_PIXEL	  16


static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

static char *phys_mem;

///@brief updates the screen as the timer increments and the arrow pointing to the player playing atm
///@param cor colour of the mouse arrow
///@param turn 1 if player1 value if player2
///@param relog if 1 draws the bitmaps related to the timer
void update_screen(int cor,int turn,int relog);

///@brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
///@return 0 upon success, non-zero upon failure
int vg_exit(void);

///@brief Initializes the video module in graphics mode
///@param mode 16-bit VBE mode to set
///@return Virtual address VRAM was mapped to. NULL, upon failure
void *vg_init(unsigned short mode);

///@brief sets a pixel with color in the video mem
///@param x width coordenate
///@param y height coordenate
///@param color color of the pixel
void vg_set_pixel(unsigned int x,unsigned int y, unsigned long color);


///@brief exchanges from the buffer to the mouse buffer
void trocarRato_buffer();

///@brief exchanges from the mouse buffer to video mem
void trocarVideo_Mem_Rato();

///@brief sets a pixel with color in the mouse buffer
///@param x
///@param y
///@param color
void vg_set_pixel_bufferRato(unsigned int x,unsigned int y, unsigned long color);

///@brief sets a pixel with color in the buffer
///@param x
///@param y
///@param color
void vg_set_pixel_buffer(unsigned int x,unsigned int y, unsigned long color);


#endif
