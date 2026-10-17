#include "../headers/videocard.h"

static uint8_t *video_mem;
static uint8_t *video_buff;
static unsigned x_res;
static unsigned y_res;
static unsigned r_mask_size;
static unsigned r_pos;
static unsigned g_mask_size;
static unsigned g_pos;
static unsigned b_mask_size;
static unsigned b_pos;
static size_t vram_size;
static unsigned bytes_per_pixel;

extern uint32_t timer_counter;
extern int timer_hookid;


int vbe_mode_info(uint16_t mode, vbe_mode_info_t *mode_info) {
  reg86_t r;

  memset(&r, 0, sizeof(r));

  mmap_t map;
  lm_alloc(sizeof(vbe_mode_info_t),&map);
  
  phys_bytes buf = map.phys;

  r.intno = VIDEOCARD;
  r.ah = VBE_FUNC;
  r.al = GET_MODE_INFO;
  r.cx = LINEAR_FB | mode;
  r.es = PB2BASE(buf);
  r.di = PB2OFF(buf);

  if (sys_int86(&r) != OK) {
    printf("sysint86 FAILED!");
    lm_free(&map);
    return 1;
  }

  if (r.ah != 0) {
    lm_free(&map);
    return 1;
  }
  
  *mode_info = *(vbe_mode_info_t *) map.virt;

  lm_free(&map);

  return 0;
}

int set_vbe_mode(uint16_t mode) {
  struct minix_mem_range mr;
  mr.mr_base = 0;
  mr.mr_limit = 1<<20;
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM,&mr) != OK) {
    printf("Error setting premissions!");
    return 1;
  }

  reg86_t r;

  memset(&r, 0, sizeof(r));

  r.intno = VIDEOCARD;
  r.ah = VBE_FUNC;
  r.al = SET_VBE_MODE;
  r.bx = LINEAR_FB | mode;

  if (sys_int86(&r) != OK) {
    printf("sysint86 FAILED!");
    return 1;
  }

  return 0;
}

int bitsToBytes(uint8_t bits) {
  double bytes = (double)bits / 8.0;
  return ceil(bytes);
}

void map_video_mem(uint16_t mode) {
  struct minix_mem_range mr;
  vbe_mode_info_t mode_info;
  vbe_mode_info(mode, &mode_info);

  x_res = mode_info.XResolution;
  y_res = mode_info.YResolution;
  bytes_per_pixel = bitsToBytes(mode_info.BitsPerPixel);
  r_mask_size = mode_info.RedMaskSize;
  r_pos = mode_info.RedFieldPosition;
  g_mask_size = mode_info.GreenMaskSize;
  g_pos = mode_info.GreenFieldPosition;
  b_mask_size = mode_info.BlueMaskSize;
  b_pos = mode_info.BlueFieldPosition;

  vram_size = x_res * y_res * bytes_per_pixel;

  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM,&mr) != OK) {
    panic("Error setting premissions");
  }
  
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  video_buff = malloc(vram_size);

  if (video_mem == MAP_FAILED) {
    panic("Error mapping video memory");
  }
}

void refresh_buffer() {
  memcpy((void*)video_mem,video_buff,vram_size);
}

int set_pixel(uint32_t color, uint16_t x, uint16_t y) {
  if (!in_screen(x, y)) {
    return 1;
  }
  
  void* video_pos = video_buff + (y * x_res + x) * bytes_per_pixel;
  memcpy(video_pos, &color, bytes_per_pixel);
  
  return 0;
}


int draw_hline(uint32_t color, uint16_t x, uint16_t y, uint16_t len) {
  for (uint16_t i = 0; i < len; i++) {
    set_pixel(color, x  + i, y);
  }
  return 0;
}

int draw_rectangle(uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  for (uint16_t i = 0; i < height; i++) {
    draw_hline(color, x, y + i, width);
  }

  return 0;
}


int draw_line(uint32_t color, line_t* line) {
  for (size_t i = 0; i < line->size; i += 2) {
    set_pixel(color, line->points[i], line->points[i+1]);
  }
  return 0;
}

void draw_number(int num, uint16_t x, uint16_t y, bool right, xpm_image_t* digits) {
  uint16_t pos = x;
  
  if (num == 0) {
    pos += pow(-1, right) * digits[0].width;
    draw_xpm(pos, y, digits[0]);
    return;
  }
  
  while (num > 0) {
    int dig = num % 10;
    num /= 10;
    // printf("%d %d\n", num, dig);
    switch (dig)
    {
    case 0:
      pos += pow(-1, right) * digits[0].width;
      draw_xpm(pos, y, digits[0]);
      break;
    case 1:
      pos += pow(-1, right) * digits[1].width;
      draw_xpm(pos, y, digits[1]);
      break;
    case 2:
      pos += pow(-1, right) * digits[2].width;
      draw_xpm(pos, y, digits[2]);
      break;
    case 3:
      pos += pow(-1, right) * digits[3].width;
      draw_xpm(pos, y, digits[3]);
      break;
    case 4:
      pos += pow(-1, right) * digits[4].width;
      draw_xpm(pos, y, digits[4]);
      break;
    case 5:
      pos += pow(-1, right) * digits[5].width;
      draw_xpm(pos, y, digits[5]);
      break;
    case 6:
      pos += pow(-1, right) * digits[6].width;
      draw_xpm(pos, y, digits[6]);
      break;
    case 7:
      pos += pow(-1, right) * digits[7].width;
      draw_xpm(pos, y, digits[7]);
      break;
    case 8:
      pos += pow(-1, right) * digits[8].width;
      draw_xpm(pos, y, digits[8]);
      break;
    case 9:
      pos += pow(-1, right) * digits[9].width;
      draw_xpm(pos, y, digits[9]);
      break;
    default:
      break;
    }
  }
}


int draw_xpm(uint16_t x, uint16_t y, xpm_image_t img) {
  uint32_t *map = (uint32_t*)img.bytes;
  for (int h = 0; h < img.height; h++) {
    for (int w = 0; w < img.width; w++) {
      uint32_t color = map[h * img.width + w];
      if (color != xpm_transparency_color(img.type)) {
        set_pixel(color, w+x, h+y);
      }
    }
  }
  return 0;
}

int clear_xpm(uint16_t x, uint16_t y, xpm_image_t img) {
  uint32_t *map = (uint32_t*)img.bytes;
  for (int h = 0; h < img.height; h++) {
    for (int w = 0; w < img.width; w++) {
      uint32_t color = map[h * img.width + w];
      if (color != xpm_transparency_color(img.type)) {
        set_pixel(0, w+x, h+y);
      }
    }
  }
  return 0;
}

void clear_screen() {
  for (unsigned x = 0; x < x_res; x++) {
    for (unsigned y = 0; y < y_res; y++) {
      set_pixel(0, x, y);
    }
  }
}

void clear_screen_background(xpm_image_t background_img) {
  draw_xpm(0,0,background_img);
}

bool in_screen(uint16_t x, uint16_t y) {
  if (x >= x_res || y >= y_res || x < 0 || y < 0) {
    return false;
  }
  return true;
}

unsigned get_y_res() {
  return y_res;
}

unsigned get_x_res() {
  return x_res;
}

