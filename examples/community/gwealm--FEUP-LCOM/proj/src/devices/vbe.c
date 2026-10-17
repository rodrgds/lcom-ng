#include "devices/vbe.h"

static vbe_video_mem info;

int (vbe_init)() {
  int err_no;
  if ((err_no = vbe_set_mode(VBE_MODE)) != OKAY) {
    printf("vbe_init: could not set mode\n");
    return err_no;
  }

  if ((err_no = vbe_mem_map(VBE_MODE)) != OKAY) {
    printf("vbe_init: could not map memory\n");
    return err_no;
  }

  return OKAY;
}

int vbe_cleanup() {
  free(info.buffer);

  int err_no;
  if ((err_no = vbe_set_text_mode()) != OKAY) {
    printf("vbe_cleanup: could not return to text mode\n");
    return err_no;
  }

  return OKAY;
}

int (vbe_set_text_mode)() {
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));

  reg.intno = VBE_INTNO;
  reg.ax = VBE_SET_TEXT_MODE;

  if (sys_int86(&reg)) {
    perror("Couldn't execute system call");
    return VBE_EMODE;
  }

  int err_no;
  if ((err_no = vbe_handle_errors(reg.ah)) != OKAY) {
    return err_no;
  }

  return OKAY;
}

int (vbe_set_mode)(uint16_t mode) {
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));

  reg.intno = 0x10;
  reg.ax = VBE_SET_MODE;
  reg.bx = mode | BIT(14);

  if (sys_int86(&reg)) {
    perror("Couldn't execute system call");
    return VBE_EMODE;
  }

  int err_no;
  if ((err_no = vbe_handle_errors(reg.ah)) != OKAY) {
    return err_no;
  }

  return OKAY;
}

int (vbe_mem_map)(uint16_t mode) {
  int err_no;

  vbe_mode_info_t mode_info;
  if ((err_no = vbe_get_mode_info(mode, &mode_info)) != OKAY) {
    printf("vbe_mem_map: could not get mode info\n");
    return err_no;
  }
  
  info.hres = mode_info.XResolution;
  info.vres = mode_info.YResolution;
  info.bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;

  info.red_pos = mode_info.RedFieldPosition;
  info.green_pos = mode_info.GreenFieldPosition;
  info.blue_pos = mode_info.BlueFieldPosition;

  info.red_size = mode_info.RedMaskSize;
  info.green_size = mode_info.GreenMaskSize;
  info.blue_size = mode_info.BlueMaskSize;
  
  unsigned int vram_base = mode_info.PhysBasePtr;
  unsigned int vram_size = info.hres * info.vres * info.bytes_per_pixel;  // in bytes

  info.buffer = malloc(vram_size);
  if (info.buffer == NULL) {
    printf("vbe_mem_map: could not allocate secondary buffer\n");
    return VBE_EALLOC;
  }

  struct minix_mem_range mr;
  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if( OK != (err_no = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    return VBE_EMAP;
  }

  info.vmem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(info.vmem == MAP_FAILED) {
    return VBE_EMAP;
  }
  
  return OKAY;
}

int (vbe_handle_errors)(uint8_t ah) {
  switch (ah) {
    case 0x00:
      break;
    case 0x01:
      printf("vbe_handle_errrors: function call failed");
      return VBE_EFUNC;
    case 0x02:
      printf("vbe_handle_errors: function not supported in current HW mode");
      return VBE_EFUNC;
    case 0x03:
      printf("vbe_handle_errrors: function not supported in current video mode");
      return VBE_EFUNC;
    default:
      printf("vbe_handle_errrors: unknown error");
      return VBE_EFUNC;
  }

  return OKAY;
}

int (vbe_flush)(){
  memcpy(info.vmem, info.buffer, info.hres * info.vres * info.bytes_per_pixel);
  return OKAY;
}

int (vbe_fill)(uint16_t color) {
  size_t size = info.hres * info.vres; 
  for (size_t i = 0; i < size; i++) {
    info.buffer[i] = color;
  }

  return OKAY;
}

int (vbe_draw_pixel)(uint16_t x, uint16_t y, uint16_t color) {
  if (x >= info.hres || y >= info.vres) {
    return VBE_EBOUNDS;
  }

  size_t start = y * info.hres + x;
  info.buffer[start] = color;

  return OKAY;
}

int (vbe_draw_line) (uint16_t x, uint16_t y, uint16_t len, uint16_t color) {
  if (x >= info.hres || y >= info.vres) {
    return VBE_EBOUNDS;
  }

  int success = OKAY;
  uint16_t max_x = x + len;

  if (max_x >= info.hres) {
    max_x = info.hres;
    success = VBE_EBOUNDS;
  }

  size_t pos = y * info.hres + x;
  for (uint16_t i = x; i < max_x; i++, pos++) {
    info.buffer[pos] = color; 
  }

  return success;
}

int (vbe_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
  if (x >= info.hres || y >= info.vres) {
    return VBE_EBOUNDS;
  }
  int success = OKAY;
  uint16_t max_x = x + width;
  uint16_t max_y = y + height;

  if (max_x >= info.hres) {
    max_x = info.hres;
    success = VBE_EBOUNDS;
  }

  if (max_y >= info.vres) {
    max_y = info.vres;
    success = VBE_EBOUNDS;
  }

  size_t start = y * info.hres + x;
  
  for (uint16_t j = y; j < max_y; j++) {
    size_t pos = start;

    for (uint16_t i = x; i < max_x; i++, pos++) {
      info.buffer[pos] = color;
    }

    start += info.hres;
  }

  return success;
}

int (vbe_draw_xpm)(uint16_t x, uint16_t y, xpm_image_t* xpm) {
  uint16_t transparency = xpm_transparency_color(xpm->type);

  size_t index = 0;
  uint16_t* pixels = (uint16_t*) xpm->bytes;

  for (int row = 0; row < xpm->height; row++) {
    for (int col = 0; col < xpm->width; col++) {
      uint32_t color = pixels[index++];
      if (color == transparency) {
        continue;
      }

      vbe_draw_pixel(x + col, y + row, color);
    }
  }

  return OKAY;
}

uint16_t (vbe_get_width)() {
  return info.hres;
}

uint16_t (vbe_get_height)() {
  return info.vres;
}
