#ifndef VBE_H
#define VBE_H

#include <stdint.h>

// vbe operation
#define VBE 0x4F
#define VBE_SETMODE 0x02

/**
 * @brief Set the VBE video mode
 * @param mode vbe mode to switch to
 * @return zero on success, non-0 otherwise
*/
int vbe_set_mode(uint16_t mode);

/**
 * @brief Flip VBE video buffer, 
 * assuming a double buffer architecture
 * @param vres virtual screen resolution
 * @return zero on success, non-0 otherwise
*/
int vbe_flip_buffer(uint32_t vres);

#endif
