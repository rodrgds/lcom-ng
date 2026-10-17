#include <lcom/lcf.h>

#include "pixel_buffer.h"

pixel_buffer new_pixel_buffer(uint16_t h_res, uint16_t v_res, uint8_t bytes_per_pixel) {
  uint32_t buffer_size = h_res * v_res * bytes_per_pixel;

  void *buffer_start = malloc(buffer_size);

  return (pixel_buffer){h_res, v_res, buffer_size, bytes_per_pixel, buffer_start};
}
