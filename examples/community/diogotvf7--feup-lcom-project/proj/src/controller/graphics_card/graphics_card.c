#include "graphics_card.h"

uint8_t *video_mem = NULL;
uint16_t bytes_per_pixel;
uint16_t h_res;
uint16_t v_res;
uint8_t red_field_position;
uint8_t red_mask_size;
uint8_t green_field_position;
uint8_t green_mask_size;
uint8_t blue_field_position;
uint8_t blue_mask_size;
uint8_t mem_model;
uint8_t page_index = 0;
vbe_mode_info_t vmi_p;

extern uint8_t mem_model;
static uint16_t mode;

uint16_t _mode() {
  return mode;
}

void *(vg_init)(uint16_t mode) {
  void *video_mem = map_graphics_memory(mode);
  reg86_t reg86;
  bzero(&reg86, sizeof reg86);
  
  reg86.intno = GRAPHICS_INT_NO;
  reg86.ah = VESA_FUNC;
  reg86.al = SET_GRAPHICS_MODE;
  reg86.bx = mode | LINEAR_MODE;

  if (sys_int86(&reg86) != OK)
    return NULL;
  return video_mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++, x++)
    vg_draw_pixel(x, y, color);
  return EXIT_SUCCESS;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++, y++)
    vg_draw_hline(x, y, width, color);
  return EXIT_SUCCESS;
}

void *(map_graphics_memory)(uint16_t mode) {
  bzero(&vmi_p, sizeof vmi_p);
  vbe_get_mode_info(mode, &vmi_p);

  int r;
  struct minix_mem_range mr;
  unsigned int vram_base = vmi_p.PhysBasePtr;
  unsigned int vram_size = (unsigned int) vmi_p.BytesPerScanLine * vmi_p.YResolution * 2; // Double of the size because of page flipping

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK)
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
    panic("couldn’t map video memory\n");
  
  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;
  red_field_position = vmi_p.RedFieldPosition;
  green_field_position = vmi_p.GreenFieldPosition;
  blue_field_position = vmi_p.BlueFieldPosition;
  red_mask_size = vmi_p.RedMaskSize;
  green_mask_size = vmi_p.GreenMaskSize;
  blue_mask_size = vmi_p.BlueMaskSize;
  mem_model = vmi_p.MemoryModel;
  
  return video_mem;
}

int (vg_flip_frame) () {
  if (video_mem == NULL) {
    fprintf(stderr, "vg_flip_frame(): video_mem is NULL\n");
    return EXIT_FAILURE;
  }
  reg86_t reg86;
  bzero(&reg86, sizeof reg86);
  reg86.intno = GRAPHICS_INT_NO;
  reg86.ah = VESA_FUNC;
  reg86.al = SET_DISPLAY_START;
  reg86.bh = 0;
  reg86.bl = 0;
  reg86.cx = 0;
  reg86.dx = v_res * page_index;

  if (sys_int86(&reg86) != OK){
    fprintf(stderr, "vg_flip_frame(): sys_int86() failed\n");
    return EXIT_FAILURE;
  }
  if (reg86.ax != VBE_RETURN_SUCCESS) {
    fprintf(stderr, "vg_flip_frame(): sys_int86() failed with: %x\n", reg86.ax);
    return EXIT_FAILURE;
  }

  page_index = page_index == 0;

  return EXIT_SUCCESS;
}

int (copy_base_frame)(frame_buffer_t frame_buffer) {
  if (video_mem == NULL) 
    return EXIT_FAILURE;
  if (memcpy(video_mem + page_index * v_res * h_res * bytes_per_pixel, frame_buffer.base_addr, frame_buffer.size) == NULL)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int (vg_draw_pixel) (uint16_t x, uint16_t y, uint32_t color) {
  if (video_mem == NULL) 
      return EXIT_FAILURE;
  if (x >= h_res || x < 0 || y >= v_res || y < 0)
      return EXIT_FAILURE;

  int pixel_pos = h_res * y + x;
  int byte_offset = pixel_pos * bytes_per_pixel;

  if (memcpy(&video_mem[byte_offset + page_index * (v_res * h_res * bytes_per_pixel)], &color, (unsigned) bytes_per_pixel) == NULL)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

// int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
//   uint8_t width = get_xpm_width(xpm);
//   uint8_t height = get_xpm_height(xpm);
//   uint8_t colors_no = get_xpm_colors_no(xpm); 
//   color_symbol *colors_symbols = get_xpm_colors(xpm, colors_no);

//   for (int i = colors_no; i < height + colors_no; i++) {
//     for (int j = 0; j < width; j++) {
//       for (int k = 0; k < colors_no; k++) {
//         if (xpm[i][j] == colors_symbols[k].symbol) {
//           if (vg_draw_pixel(x + j, y + i - colors_no, colors_symbols[k].color_index) != OK)
//             return EXIT_FAILURE;
//         }
//       }
//     }
//   }
//   return EXIT_SUCCESS;
// }

uint32_t (red)(unsigned y, uint32_t first, uint8_t step) {
  return (get_red_bits(first) + y * step) % (1 << red_mask_size);
}

uint32_t (green)(unsigned x, uint32_t first, uint8_t step) {
  return (get_green_bits(first) + x * step) % (1 << green_mask_size);
}

uint32_t (blue)(unsigned x, unsigned y, uint32_t first, uint8_t step) {
  return (get_blue_bits(first) + (x + y) * step) % (1 << blue_mask_size);
}

uint32_t (get_red_bits)(uint32_t color) {

  uint32_t mask = 0;
  for (int i = 0; i < red_mask_size; i++)
    mask |= (1 << (red_field_position + i));
  return (mask & color) >> red_field_position;
}

uint32_t (get_green_bits)(uint32_t color) {
  uint32_t mask = 0;
  for (int i = 0; i < green_mask_size; i++)
      mask |= (1 << (green_field_position + i));
  return (mask & color) >> green_field_position;
}

uint32_t (get_blue_bits)(uint32_t color) {
  uint32_t mask = 0;
  for (int i = 0; i < blue_mask_size; i++)
      mask |= (1 << (blue_field_position + i));
  return (mask & color) >> blue_field_position;
}

uint32_t (index_color)(uint32_t first, uint8_t step, uint8_t no_rectangles, uint8_t row, uint8_t col) {
    return (first + (row * no_rectangles + col) * step) % (1 << bytes_per_pixel * 8);
}

uint32_t (direct_color)(uint32_t red, uint32_t green, uint32_t blue){
  return ((red << red_field_position) | (blue << blue_field_position) | (green << green_field_position));
}

uint8_t (get_xpm_width)(xpm_map_t xpm) {
  int buffer = 0, i = 0;
  while (xpm[0][i] != (char) ' ') {
      buffer = buffer * 10 + (xpm[0][i] - '0');
      i++;
  }
  return buffer;
}

uint8_t (get_xpm_height)(xpm_map_t xpm) {
  int buffer = 0, i = 0;
  while (xpm[0][i] != ' ') i++; i++;
  while (xpm[0][i] != ' ') {
      buffer = buffer * 10 + (xpm[0][i] - '0');
      i++;
  }
  return buffer;
}

uint8_t (get_xpm_colors_no)(xpm_map_t xpm) {
  int buffer = 0, i = 0;
  while (xpm[0][i] != ' ') i++; i++;
  while (xpm[0][i] != ' ') i++; i++;
  while (xpm[0][i] != '\0') {
      buffer = buffer * 10 + (xpm[0][i] - '0');
      i++;
  }
  return buffer;
}
