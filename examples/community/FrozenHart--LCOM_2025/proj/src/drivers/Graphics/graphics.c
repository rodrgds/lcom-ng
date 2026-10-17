#include "graphics.h"
#include "VBE.h"
#include "lcom/pixmap.h"

vbe_mode_info_t modeinf;
static char *video_mem; /* frame-buffer VM address */
static uint16_t hres;   /* XResolution */
static uint16_t vres;   /* YResolution */

/*
 * VBE/BIOS Video Functions Reference:
 *
 * Function                        | AH    | AL       | VBE Ref | Parameters & Notes
 * ------------------------------------------------------------------------------------
 * Set VBE mode                   | 0x4F  | 0x02     | p.25    | - Mode number in BX (bit 14 must be set for LFB)
 * Return VBE Mode Info           | 0x4F  | 0x01     | p.16    | - Mode number in CX
 * Return VBE Controller Info     | 0x4F  | 0x00     | p.12    | - Output buffer address in ES:DI
 * BIOS Set Video Mode            | 0x00  | <mode>   | N/A     | - Non-VBE function
 *                                                             | - Use AL=0x03 to restore Minix text mode
 *
 * Legend:
 * LFB = Linear Frame Buffer
 * <mode> = Video mode number (e.g., 0x03 for text, 0x105 for 1024x768x8)
 */

int(set_graphic_mode)(uint16_t mode) {
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ah = 0x4F;
  reg.al = 0x02;
  reg.bx = mode | BIT(14);
  if (sys_int86(&reg) != 0) {
    printf("set_graphic_mode(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int(set_text_mode)() { // vg_exit()
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ah = 0x00;
  reg.al = 0x03;
  if (sys_int86(&reg) != 0) {
    printf("set_text_mode(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int(map_VRAM)(unsigned short mode) {
  memset(&modeinf, 0, sizeof(modeinf));
  if (vbe_get_mode_info(mode, &modeinf) != 0) { // retrieves information about the input VBE mode, including the physical address of the frame buffer
    printf("vbe_get_mode_info failed\n");
    return 1;
  }

  vres = modeinf.YResolution;
  hres = modeinf.XResolution;
  int r;
  struct minix_mem_range mr;                                                                               /* physical memory range */
  unsigned int vram_base = modeinf.PhysBasePtr;                                                            /* VRAM’s physical addresss */
  unsigned int vram_size = modeinf.XResolution * modeinf.YResolution * ((modeinf.BitsPerPixel + 7) / 8.0); /* VRAM’s size, but you can use the frame-buffer size, instead */

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

  return video_mem == MAP_FAILED;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if (x >= modeinf.XResolution || y >= modeinf.YResolution) {
    printf("[vg_draw_pixel] Out of bounds: x=%u y=%u (max: %u x %u)\n", x, y, modeinf.XResolution, modeinf.YResolution);
    return 1;
  }
  uint8_t BytesPerPixel = ((modeinf.BitsPerPixel + 7) / 8.0);
  uint64_t pixel_pos = (y * modeinf.XResolution + x) * BytesPerPixel;

  if (video_mem == NULL) {
    printf("[vg_draw_pixel] video_mem is NULL!\n");
    return 1;
  }

  int ret = memcpy(video_mem + pixel_pos, &color, BytesPerPixel) == 0;
  if (ret != 0) {
    printf("[vg_draw_pixel] memcpy failed or returned 0\n");
  }
  return ret;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color))
      return 1;
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color))
      return 1;
  }
  return 0;
}

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint16_t h_sise = hres / no_rectangles;
  uint16_t v_sise = vres / no_rectangles;

  for (uint8_t row = 0; row < no_rectangles; row++) {
    for (uint8_t col = 0; col < no_rectangles; col++) {
      uint32_t color;

      if (get_color(first, &color, row, col, no_rectangles, step) != 0) {
        printf("Failed to normalize color \n");
        return 1;
      }

      if (vg_draw_rectangle(col * h_sise, row * v_sise, h_sise, v_sise, color)) {
        printf("vg_draw_rectangle failed\n");
        return 1;
      }
    }
  }
  return 0;
}

/*
enum xpm_image_type {
  XPM_INDEXED,
  XPM_1_5_5_5,
  XPM_5_6_5,
  XPM_8_8_8,
  XPM_8_8_8_8,
  XPM_GRAY_1_5_5_5,
  XPM_GRAY_5_6_5,
  XPM_GRAY_8_8_8,
  XPM_GRAY_8_8_8_8,
  INVALID_XPM
};
*/
int(vg_draw_xpm)(xpm_image_t img, uint8_t *colors, uint16_t x, uint16_t y) {
  for (int row = 0; row < img.height; row++) {
    for (int col = 0; col < img.width; col++) {
      if (vg_draw_pixel(x + col, y + row, *colors) != 0) {
        printf("Failed to draw pixel \n");
        return 1;
      }
      colors++;
    }
  }
  return 0;
}

uint8_t *(vg_load_xpm) (xpm_map_t xpm, xpm_image_t *img) {
  // Reads an XPM pixmap xmap, and returns the pixmap as a two-dimensional uint8_t array. The color encoding of the (output) pixmap is the one specified in type.
  return xpm_load(xpm, XPM_INDEXED, img);
}

int(vg_erase_xpm)(xpm_image_t img, uint16_t x, uint16_t y) {
  for (int row = 0; row < img.height; row++) {
    for (int col = 0; col < img.width; col++) {
      if (vg_draw_pixel(x + col, y + row, xpm_transparency_color(img.type)) != 0) {
        printf("Failed to draw pixel \n");
        return 1;
      }
    }
  }
  return 0;
}
/*
 * Graphics Mode Configuration Summary:
 *
 * Mode    | Resolution    | Color Model     | Bits per Pixel (R:G:B)
 * ----------------------------------------------------------------
 * 0x105   | 1024x768      | Indexed         | 8 (Single index)
 * 0x110   | 640x480       | Direct Color    | 15 (1:5:5:5)*
 * 0x115   | 800x600       | Direct Color    | 24 (8:8:8)
 * 0x11A   | 1280x1024     | Direct Color    | 16 (5:6:5)
 * 0x14C   | 1152x864      | Direct Color    | 32 (8:8:8:8)**
 *
 * Notes:
 * * 15-bit mode includes 1 unused bit (ARGB1555 format)
 * ** 32-bit mode includes an 8-bit alpha channel (RGBA8888 format)
 *
 */
int(get_color)(uint32_t old_color, uint32_t *new_color, uint16_t row, uint16_t col, uint16_t no_rectangles, uint16_t step) {
  switch (modeinf.MemoryModel) {
    case 0x06: { // Direct Color Mode
      // Calculate components according to test pattern rules
      uint32_t r = ((old_color >> 16) & 0xFF) + col * step;
      uint32_t g = ((old_color >> 8) & 0xFF) + row * step;
      uint32_t b = (old_color & 0xFF) + (col + row) * step;

      // Apply mask constraints
      r %= (1 << modeinf.RedMaskSize);
      g %= (1 << modeinf.GreenMaskSize);
      b %= (1 << modeinf.BlueMaskSize);

      // Pack components according to mode
      *new_color = (r << modeinf.RedFieldPosition) |
                   (g << modeinf.GreenFieldPosition) |
                   (b << modeinf.BlueFieldPosition);
      break;
    }
    case 0x04: { // Indexed Color Mode
      // Calculate index using test pattern formula
      uint32_t index = (old_color + (row * no_rectangles + col) * step);
      index %= (1 << modeinf.BitsPerPixel);
      *new_color = index;
      break;
    }
    default:
      printf("Unsupported memory model: 0x%02X\n", modeinf.MemoryModel);
      return 1;
  }
  return 0;
}

uint16_t(get_h_resolution)() {
  return hres;
}

uint16_t(get_v_resolution)() {
  return vres;
}

int vg_read_pixel(uint16_t x, uint16_t y, uint32_t *color) {
    // Check bounds
    if (x >= modeinf.XResolution || y >= modeinf.YResolution) {
        printf("[vg_read_pixel] Out of bounds: x=%u y=%u (max: %u x %u)\n", 
               x, y, modeinf.XResolution, modeinf.YResolution);
        return 1;
    }
    
    // Calculate bytes per pixel
    uint8_t bytes_per_pixel = (modeinf.BitsPerPixel + 7) / 8;
    uint64_t pixel_offset = (y * modeinf.XResolution + x) * bytes_per_pixel;
    
    if (video_mem == NULL) {
        printf("[vg_read_pixel] video_mem is NULL!\n");
        return 1;
    }
    
    // Read pixel data based on color depth
    switch (bytes_per_pixel) {
        case 1:  // 8-bit indexed color
            *color = *(video_mem + pixel_offset);
            break;
            
        case 2:  // 15/16-bit color
            *color = *((uint16_t *)(video_mem + pixel_offset));
            break;
            
        case 3:  // 24-bit color
            *color = *((uint32_t *)(video_mem + pixel_offset)) & 0xFFFFFF;
            break;
            
        case 4:  // 32-bit color
            *color = *((uint32_t *)(video_mem + pixel_offset));
            break;
            
        default:
            printf("[vg_read_pixel] Unsupported bytes per pixel: %u\n", bytes_per_pixel);
            return 1;
    }
    
    return 0;
}
