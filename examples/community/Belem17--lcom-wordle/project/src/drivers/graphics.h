
#ifndef Graphics_H
#define Graphics_H
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
// Addresses
#define AH 0x4F
#define set_Videomode 0x00

// Write this values to the Al register to execute the functions
#define return_VBE_Mode_Information 0x01
#define return_VBE_Controller_Information 0x00
#define set_VBE_Mode 0x02

// return codes in the Ah function calls
#define sucess 0x00
#define callFailed 0x01
#define notSupportedHWconfig 0x02
#define invalidIncurrentvideoMode 0x03

// VBE modes use Set mode function and pass in the Bx register (Bit 14 should be set )
#define mode_1 0x105
#define mode_2 0x110
#define mode_3 0x115
#define mode_6 0x118
#define mode_4 0x11A
#define mode_5 0x14c
//functions 


#define y_axis 786
#define x_axis 1024


int (vg_enter)(uint16_t mode);
int (vg_int) (uint16_t mode);
int(vg_draw_xpm)(uint16_t xi, uint16_t yi, uint8_t *map,uint16_t width ,uint16_t height);
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
uint32_t(corrected_Colour)(uint32_t color);
int (vg_f7_withsubfunction80)();
void clean_screen();
#endif
