#include "graphics.h"

uint16_t hres;                      // Horizontal resolution in pixels
uint16_t vres;                      // Vertical resolution in pixels
static uint8_t bits_per_pixel;      // Number of bits used for each individual pixel
static void *video_mem;             // Pointer to memory location where the screen pixels information is
char previous[1024 * 768 * 32];     // Exact copy of background without cursor sprites
char buffer[1024 * 768 * 32];       // Exact copy of screen, used for double buffering
static size_t vram_base;            // Physical address for flat memory frame buffer
static size_t vram_size;            // Total number of bits used for all screen pixels
static uint8_t RedScreeMask;        // Size of direct color red mask in bits
static uint8_t GreenScreeMask;      // Size of direct color green mask in bits
static uint8_t BlueScreeMask;       // Size of direct color blue mask in bits
static uint8_t RedLSB;              // Bit position of lsb of red mask
static uint8_t GreenLSB;            // Bit position of lsb of green mask
static uint8_t BlueLSB;             // Bit position of lsb of blue mask

static int loop_counter = 0;        // Counter used for function loop_colorS

int get_mode_info(uint16_t mode, vbe_mode_info_t *mode_info) {
  mmap_t memory_map;
  struct reg86u reg86;

  video_mem = lm_init(true);
  while (video_mem == NULL) {
    video_mem = lm_init(true);
  }

  video_mem = lm_alloc(sizeof(vbe_mode_info_t), &memory_map);
  while (video_mem == NULL) {
    video_mem = lm_alloc(sizeof(vbe_mode_info_t), &memory_map);
  }

  memset(&reg86, 0, sizeof(reg86));

  phys_bytes buf = memory_map.phys;

  reg86.u.b.intno = VBE_MODE;
  reg86.u.w.ax = VBE_MODE_INFO;
  reg86.u.w.es = PB2BASE(buf);
  reg86.u.w.di = PB2OFF(buf);
  reg86.u.w.cx = mode;

  if (sys_int86(&reg86) != OK) {
    return 1;
  }

  if (reg86.u.b.ah != VBE_CALL_SUCCESS)
    ;

  *mode_info = *(vbe_mode_info_t *) memory_map.virt;

  lm_free(&memory_map);

  return 0;
}

void *(vg_init)(uint16_t mode) {

  struct reg86u reg;

  int R;
  struct minix_mem_range mr;

  vbe_mode_info_t mode_info;

  if (get_mode_info(mode, &mode_info) != 0) {
    panic("get_mode_info unsuccessful");
  }

  /* Allow memory mapping */

  hres = mode_info.XResolution;
  vres = mode_info.YResolution;
  bits_per_pixel = mode_info.BitsPerPixel;
  vram_base = mode_info.PhysBasePtr;
  RedScreeMask = mode_info.RedMaskSize;
  GreenScreeMask = mode_info.GreenMaskSize;
  BlueScreeMask = mode_info.BlueMaskSize;
  RedLSB = mode_info.LinRedFieldPosition;
  GreenLSB = mode_info.LinGreenFieldPosition;
  BlueLSB = mode_info.LinBlueFieldPosition;

  vram_size = hres * vres * ((bits_per_pixel + 7) / 8);

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (R = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", R);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  memset(&reg, 0, sizeof(reg));
  reg.u.w.ax = 0x4F02;
  reg.u.w.bx = mode | BIT(14);
  reg.u.b.intno = 0x10;

  if (sys_int86(&reg) != OK) {
    return 0;
  }

  return video_mem;
}

void paint_pixel(int x, int y, u_int32_t color) {
  // Calculates number of bytes used for each piel
  int bytes = (int) ceil((float) (bits_per_pixel) / 8.0);

  if (x < hres && x > 0 && y < vres && y > 0) {
    uint8_t *ptr = (uint8_t *) buffer + (x + y * hres) * bytes;

    uint8_t *copy = (uint8_t *) previous + (x + y * hres) * bytes;

    // Changes the values both in the buffer and in the previous arrays.
    // ATENCION: Should not be used to paint cursor pixels!

    memcpy(ptr, &color, bytes);
    memcpy(copy, &color, bytes);
  }
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t length, u_int32_t color) {

  for (uint16_t i = 0; i < length; i++) {
    paint_pixel(x + i, y, color);
  }

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, u_int32_t color) {

  for (uint16_t j = 0; j < height; j++) {
    vg_draw_hline(x, y + j, width, color);
  }

  return 0;
}

void paint_pixel_cursor(int x, int y, u_int32_t color) {
  int bytes = (int) ceil((float) (bits_per_pixel) / 8.0);

  // Chenges the values only in the buffer.
  // ATENCION: Should only be used to paint cursor pixels

  uint8_t *ptr = (uint8_t *) buffer + (x + y * hres) * bytes;

   memcpy(ptr, &color, bytes);

}

void loop_color(int * color){
  // If it reaches end of color pallete returns to initial position
  if (loop_counter > 279)
    loop_counter = 0;

  // Retrieves the color in the screen without the cursor
  *color = get_color(980, 195 + loop_counter);

  // Skipping one position gives a quicker rainbow effect
  loop_counter += 2;
}

int get_color(uint16_t x, uint16_t y){

  int color = 0;

  // Calculates bytes used for each pixel
  int bytes = (int) ceil((float) (bits_per_pixel) / 8.0);

  // Reads the color in the buffer, starts i at 3 because colors are writen "backwards" in the array
  for (int i = 3; i >= 0; i--)
    color = (color << 8) | (uint8_t) (previous[(x + y * hres) * bytes + i]);
  
  return color;
}

void double_buffer() {
  // Copies all values in the array buffer to video memory (screen)
  memcpy(video_mem, buffer, vram_size);
}
