#include "graphics.h"

vbe_mode_info_t mode_info;
uint8_t *frame_buffer;
uint8_t *img_buffer;
unsigned bytes_per_pixel;
uint16_t x_resolution;
uint16_t y_resolution;

unsigned(calculate_bytes_per_pixel)(uint8_t bits_per_pixel) {
  unsigned bytes_per_pixel = (bits_per_pixel + 7) / 8;
  return bytes_per_pixel;
}

int(initialize_graphics_mode)(uint16_t graphic_mode) {
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));

  reg.ah = SET_AH_GRAPHIC;
  reg.al = SET_AL_GRAPHIC;
  reg.bx = graphic_mode | SET_LINEAR_MODE;
  reg.intno = ITNO;

  if (sys_int86(&reg) != 0) {
    return 1;
  }

  return 0;
}

int(map_address_space)(uint16_t mode) {
  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    return 1;
  }

  // size of the frame buffer in bytes
  bytes_per_pixel = calculate_bytes_per_pixel(mode_info.BitsPerPixel);
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

  // filling physical addresses
  struct minix_mem_range physic_address_space;
  physic_address_space.mr_base = mode_info.PhysBasePtr;
  physic_address_space.mr_limit = physic_address_space.mr_base + frame_size;

  // allocatiing the physical memory necessary for the frame buffer
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_address_space) != 0) {
    return 1;
  }

  // allocating the virtual memory for the frame buffer
  frame_buffer = vm_map_phys(SELF, (void *) physic_address_space.mr_base, frame_size);
  if (frame_buffer == NULL) {
    return 1;
  }

  x_resolution = mode_info.XResolution;
  y_resolution = mode_info.YResolution;

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x + len > mode_info.XResolution || y >= mode_info.YResolution) {
    return 1;
  }

  unsigned int start_index = (mode_info.XResolution * y + x) * bytes_per_pixel;
  
  // For single pixel width, just copy directly
  if (bytes_per_pixel == 1) {
    memset(&img_buffer[start_index], color, len);
  } else {
    // Create a temporary buffer with repeated color pattern
    uint8_t *temp_line = malloc(len * bytes_per_pixel);
    if (!temp_line) return 1;
    
    // Fill the temporary buffer with the repeated color
    for (unsigned i = 0; i < len; i++) {
      memcpy(&temp_line[i * bytes_per_pixel], &color, bytes_per_pixel);
    }
    
    // Copy the entire line at once
    memcpy(&img_buffer[start_index], temp_line, len * bytes_per_pixel);
    free(temp_line);
  }

  return 0;
}

int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x >= mode_info.XResolution || y + len > mode_info.YResolution) {
    return 1;
  }

  //unsigned bytes_per_pixel = calculate_bytes_per_pixel(mode_info.BitsPerPixel);
  unsigned int start_index = (mode_info.XResolution * y + x) * bytes_per_pixel;

  for (unsigned i = 0; i < len; i++) {
    unsigned int index = start_index + i * mode_info.XResolution * bytes_per_pixel;
    if (memcpy(&img_buffer[index], &color, bytes_per_pixel) == NULL) {
      return 1;
    }
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  // For single color rectangles, we can optimize further
  if (bytes_per_pixel == 1) {
    // For 1 byte per pixel, use memset for each row
    for (unsigned i = 0; i < height; i++) {
      unsigned int row_start = ((y + i) * mode_info.XResolution + x) * bytes_per_pixel;
      memset(&img_buffer[row_start], color, width);
    }
  } else {
    // Create one line and copy it multiple times
    uint8_t *temp_line = malloc(width * bytes_per_pixel);
    if (!temp_line) return 1;
    
    // Fill the first line
    for (unsigned j = 0; j < width; j++) {
      memcpy(&temp_line[j * bytes_per_pixel], &color, bytes_per_pixel);
    }
    
    // Copy this line for each row
    for (unsigned i = 0; i < height; i++) {
      unsigned int row_start = ((y + i) * mode_info.XResolution + x) * bytes_per_pixel;
      memcpy(&img_buffer[row_start], temp_line, width * bytes_per_pixel);
    }
    
    free(temp_line);
  }

  return 0;
}

void allocate_secondary_buffer() {
  img_buffer = (uint8_t *)malloc(x_resolution * y_resolution * bytes_per_pixel);
}

void copy_buffer_to_video() {
  memcpy(frame_buffer, img_buffer, x_resolution * y_resolution * bytes_per_pixel);
}

void clear_secondary_buffer() {
  memset(img_buffer, 0, x_resolution * y_resolution * bytes_per_pixel);
}

void free_secondary_buffer() {
  if (img_buffer) {
    free(img_buffer);
    img_buffer = NULL;
  }
}

int draw_cell_outline(uint16_t x, uint16_t y, uint16_t cell_width, uint16_t cell_height, uint32_t color) {
  return vg_draw_hline(x, y, cell_width, color) ||
         vg_draw_hline(x, y + cell_height - 1, cell_width, color) ||
         vg_draw_vline(x, y, cell_height, color) ||
         vg_draw_vline(x + cell_width - 1, y, cell_height, color);
}

int draw_grid_outline(uint16_t grid_rows, uint16_t grid_cols, uint16_t cell_width, uint16_t cell_height, uint16_t start_x, uint16_t start_y, uint32_t color) {
  for (uint16_t row = 0; row < grid_rows; row++) {
    for (uint16_t col = 0; col < grid_cols; col++) {
      uint16_t x = start_x + col * cell_width;
      uint16_t y = start_y + row * cell_height;

      if (draw_cell_outline(x, y, cell_width, cell_height, color) != 0) {
        return 1;
      }
    }
  }

  return 0;
}

void set_white_background() {
  for (unsigned i = 0; i < x_resolution * y_resolution; i++) {
    img_buffer[i * 3] = 0xFF;     
    img_buffer[i * 3 + 1] = 0xFF;  
    img_buffer[i * 3 + 2] = 0xFF; 
  }
}
