#include "graphics.h"
#include "vbe.h"

int (set_graphics_mode)(uint16_t mode) {
  reg86_t reg86;

  // Set the values of reg86 to 0
  memset(&reg86, 0, sizeof(reg86));

  // Set the values of the reg86 structure
  reg86.intno = BIOS_VIDEO_SERVICES;
  reg86.ah = VBE_FUNCTION_CALL;
  reg86.al = VBE_SET_MODE;
  reg86.bx = mode | BIT(14);

  // Call the BIOS
  return sys_int86(&reg86);
}

int (map_linear_frame_buffer)(uint16_t mode) {
  // clear mode_info
  memset(&mode_info, 0, sizeof(mode_info));

  // get mode info
  if (vbe_get_mode_info(mode, &mode_info)) {
    return 1;
  }

  struct minix_mem_range phys_mem;

  /* Load static vars related to mode */

  // bits per pixel / 8 to get bytes per pixel (15 / 8 = 1) but we want to store 2 bytes
  // so add 7 to get (15 + 7) / 8 = 2 (2 bytes per pixel)
  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;
  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;

  rgb_direct.red.mask_size = mode_info.RedMaskSize;
  rgb_direct.red.field_position = mode_info.RedFieldPosition;
  rgb_direct.green.mask_size = mode_info.GreenMaskSize;
  rgb_direct.green.field_position = mode_info.GreenFieldPosition;
  rgb_direct.blue.mask_size = mode_info.BlueMaskSize;
  rgb_direct.blue.field_position = mode_info.BlueFieldPosition;

  frame_size = h_res * v_res * bytes_per_pixel;

  phys_mem.mr_base = (phys_bytes) mode_info.PhysBasePtr;
  phys_mem.mr_limit = phys_mem.mr_base + frame_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &phys_mem)) {
    return 1;
  }

  video_mem = vm_map_phys(SELF, (void *)phys_mem.mr_base, frame_size);

  if (video_mem == MAP_FAILED) {
    printf("Error: couldn't map video memory\n");
    return 1;
  }

  double_buffer = malloc(frame_size);

  return 0;
}

int (exit_graphics_mode)() {
  free(double_buffer);
  return vg_exit();
}

int (move_buffer_to_video)() {
  if (memcpy(video_mem, double_buffer, frame_size) == NULL) {
    return 1;
  }
  return 0;
}

int (set_pixel_color)(uint16_t x, uint16_t y, uint32_t color) {
  if (x > h_res || y > v_res) {
    return 1;
  }

  uint32_t pixel_index = (y * h_res + x) * bytes_per_pixel;
  uint8_t *pixel = double_buffer + pixel_index;

  if (memcpy(pixel, &color, bytes_per_pixel) == NULL) {
    return 1;
  }

  return 0;
}

/* LCF Function */
//! lcf functions may not work properly in proj (if so add #include <lcf/lab5.h> or rewrite them)

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (set_pixel_color(x + i, y, color)) {
      vg_exit();
      return 1;
    }
  }

  return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (set_pixel_color(x, y + i, color)) {
      vg_exit();
      return 1;
    }
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color)) {
      vg_exit();
      return 1;
    }
  }

  return 0;
}

/* End Of LCF Function */

int (draw_xpm)(xpm_image_t img, uint8_t* sprite, uint16_t x, uint16_t y) {
  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (set_pixel_color(x + j, y + i, *(sprite))) {
        vg_exit();
        return 1;
      }
      sprite++;
    }
  }

  return 0;
}

uint32_t (R)(uint32_t color) {
  return (color >> rgb_direct.red.field_position) & ((1 << rgb_direct.red.mask_size) - 1);
}

uint32_t (G)(uint32_t color) {
  return (color >> rgb_direct.green.field_position) & ((1 << rgb_direct.green.mask_size) - 1);
}

uint32_t (B)(uint32_t color) {
  return (color >> rgb_direct.blue.field_position) & ((1 << rgb_direct.blue.mask_size) - 1);
}

uint16_t (get_h_res)() {
  return h_res;
}

uint16_t (get_v_res)() {
  return v_res;
}

uint8_t (get_bits_per_pixel)() {
  return bits_per_pixel;
}

rgb_direct_t (get_rgb_direct)() {
  return rgb_direct;
}

bool (is_direct_mode)() {
  return mode_info.MemoryModel == VBE_DIRECT_COLOR_MODEL;
}
