#include "graphics.h"

static gph_t gph;

gph_t vg_get_info() {
  return gph;
}

int vg_init_mode() {
  if (vg_enter_graphic_mode(VG_MODE))
    return 1;

  if (vg_map_memory(VG_MODE))
    return 1;

  return 0;
}

int vg_map_memory(uint16_t mode) {
  struct minix_mem_range mr;
  vbe_mode_info_t mode_info;

  memset(&mode_info, 0, sizeof(vbe_mode_info_t));
  memset(&gph, 0, sizeof(gph_t));

  if (vbe_get_mode_info(mode, &mode_info))
    return 1;

  gph.x_res = mode_info.XResolution;
  gph.y_res = mode_info.YResolution;
  gph.bits_per_pixel = mode_info.BitsPerPixel;
  gph.bytes_per_pixel = (gph.bits_per_pixel + 7) / 8;

  gph.direct_color = mode_info.MemoryModel == DIRECT_COLOR;

  gph.red_mask_size = mode_info.RedMaskSize;
  gph.red_field_pos = mode_info.RedFieldPosition;
  gph.green_mask_size = mode_info.GreenMaskSize;
  gph.green_field_pos = mode_info.GreenFieldPosition;
  gph.blue_mask_size = mode_info.BlueMaskSize;
  gph.blue_field_pos = mode_info.BlueFieldPosition;

  unsigned int screen_size = gph.x_res * gph.y_res * gph.bytes_per_pixel;

  mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + 3 * screen_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))
    return 1;

  for (uint32_t i = 0; i < 3; i++) {
    gph.frame_buffer[i] = vm_map_phys(SELF, (void *) (mr.mr_base + i * screen_size), screen_size);
    if (gph.frame_buffer[i] == NULL)
      return 1;

    memset(gph.frame_buffer[i], 0, screen_size);
  }

  gph.selectedNum = 1;
  gph.needs_redraw = true;

  return 0;
}

int (vg_flip)() {
  reg86_t r;
  memset(&r, 0, sizeof(reg86_t));

  r.intno = 0x10;
  r.ah = AH_PORT;
  r.al = AL_DISPLAY_START_CONTROL;
  r.bl = BL_SET_DISPLAY_START;
  r.cx = 0;
  r.dx = gph.selectedNum * gph.y_res;

  gph.needs_redraw = false;
  gph.selectedNum = gph.selectedNum == 2 ? 0 : gph.selectedNum + 1;

  return sys_int86(&r);
}

int vg_enter_graphic_mode(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(reg86_t));
  r.intno = 0x10;
  r.ah = AH_PORT;
  r.al = AL_VBEMODE_SET;
  r.bx = BX_LINEAR_FRAME_BUFF | mode;

  return sys_int86(&r);
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= gph.x_res || y >= gph.y_res)
    return 1;

  uint64_t pos = (gph.x_res * y + x) * gph.bytes_per_pixel;

  return memcpy(gph.frame_buffer[gph.selectedNum] + pos, &color, gph.bytes_per_pixel) == 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color))
      return 1;
  }
  return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (vg_draw_pixel(x, y + i, color))
      return 1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width,
                       uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color))
      return 1;
  }

  return 0;
}

int vg_draw_border(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                   uint32_t color, uint8_t thickness) {
  for (int i = 0; i < thickness; i++) {
    if (vg_draw_hline(x, y + i, width, color))
      return 1;

    if (vg_draw_hline(x, y + height - 1 - i, width, color))
      return 1;

    if (vg_draw_vline(x + i, y, height, color))
      return 1;

    if (vg_draw_vline(x + width - 1 - i, y, height, color))
      return 1;
  }

  return 0;
}

unsigned vg_get_width() {
  return gph.x_res;
}

unsigned vg_get_height() {
  return gph.y_res;
}

void vg_set_redraw() {
  gph.needs_redraw = true;
}

int vg_has_redraw() {
  return gph.needs_redraw;
}
