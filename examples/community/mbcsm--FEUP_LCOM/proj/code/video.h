#pragma once
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include "video.h"

#include <i8042.h>
#include <stdint.h>
#include <stdio.h>

#define GAME_MODE 0x11A

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */



static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned short *back_buffer; /* Secondary buffer to use for double buffering */
static unsigned memory_model;
static unsigned BlueScreeMask;
static unsigned GreenScreeMask;
static unsigned RedScreeMask;
static unsigned BlueMaskSize;
static unsigned GreenMaskSize;
static unsigned RedMaskSize;
static unsigned BlueFieldPosition;
static unsigned GreenFieldPosition;
static unsigned RedFieldPosition;

int vg_start(uint16_t mode);
int get_h_res();
int get_v_res();
int get_bits_per_pixel();
void * get_video_mem();
void * get_back_buffer();
uint8_t get_memory_model();
uint8_t get_blue_screen_mask();
uint8_t get_green_screen_mask();
uint8_t get_red_screen_mask();
uint8_t get_blue_screen_mask_position();
uint8_t get_green_screen_mask_position();
uint8_t get_red_screen_mask_position();


void clearScreen();

uint16_t getpixel(int x, int y);
void changePixel(int x, int y, uint16_t color);


