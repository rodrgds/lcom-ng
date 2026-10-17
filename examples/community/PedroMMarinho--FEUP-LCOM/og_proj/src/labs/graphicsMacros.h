#pragma once
#include <lcom/lcf.h>

/**
 * @brief BIOS interrupt number for video services.
 *
 * This macro defines the interrupt number for invoking BIOS video services.
 */
#define INT 0x10;
/**
 * @brief Function to set or get the display start address in VBE.
 *
 * This macro defines the function number for setting or getting the display start address in the VESA BIOS Extensions (VBE).
 */
#define SET_GET_DISPLAY_START 0x4F07;


