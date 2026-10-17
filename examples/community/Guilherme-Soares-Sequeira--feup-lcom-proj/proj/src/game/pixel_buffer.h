#ifndef PIXEL_BUFFER_H
#define PIXEL_BUFFER_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdlib.h>

/** @defgroup pixel_buffer pixel_buffer
 *  @{
 * 
 * Constants, structures and functions used for programming a pixel buffer
 */

/**
 * @brief A structure to represent a buffer of pixels
 * @param h_res horizontal resolution of the buffer
 * @param v_res vertical resolution of the buffer
 * @param bytes_per_pixel number of bytes each pixel occupies
 * @param buffer_start base address of the pixel buffer
 *
 */
typedef struct {
  uint16_t h_res;
  uint16_t v_res;

  uint32_t buffer_size;

  uint8_t bytes_per_pixel;

  void *buffer_start;
} pixel_buffer;

/**
 * @brief Creates a new pixel buffer
 *
 * @param h_res horizontal resolution of the buffer
 * @param v_res vertical resolution of the buffer
 * @param bytes_per_pixel number of bytes per pixel
 * @return pixel_buffer
 */
pixel_buffer(new_pixel_buffer)(uint16_t h_res, uint16_t v_res, uint8_t bytes_per_pixel);

/**@}*/

#endif
