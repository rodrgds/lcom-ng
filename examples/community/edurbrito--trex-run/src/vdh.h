#pragma once

#include <stdlib.h>

/** 
 * @defgroup Vdh Vdh
 * @{
 *
 * Functions for using the Vdh
 */

#define BIOS_VIDEO              0x10        //BIOS video services 

#define CALL_VBE                0x4F00      //Call VBE

#define DIRECT_COLOR_MODEL1     6
#define DIRECT_COLOR_MODEL2     7

/*MACROS USED TO GET CONTROLLER INFO*/
#define GET_CONTR_INFO          0x00        //Get controller information

/*MACROS USED TO GET MODE INFO*/
#define GET_MODE_INFO           0x01        //Get mode info

/*MACROS USED TO SET GRAPHIC MODE*/
#define VBE_SET_MODE            0x02        //Sets graphic mode

int vbe_get_info(uint16_t mode);
int set_graphic_mode(uint16_t mode);

int (vg_draw_hline2)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, void * buff);
int (vg_draw_rectangle2)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, void * buff);
int (changePixelColor2)(uint16_t x, uint16_t y, uint32_t color, void * buff);
int (vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);

unsigned getBitsPerPixel();
int getHres();
int getVres();

/**
 * @brief Returns video memory
 */
 void* get_vid_mem();
/**
 * @brief Returns double buffer
 */
 char* get_double_buffer();
/**
 * @brief Returns a third buffer that is used to detect collisions
 */
 unsigned char *get_rd_buffer();
/**
 * @brief Copies memory from double buffer to video_mem
 */
 void change_buffers();
