#include "GPU.h"

static uint8_t *video_mem; /* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;          /* Horizontal resolution in pixels */
static unsigned v_res;          /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned int vram_size;  /* VRAM's size, but you can use
                        the frame-buffer size, instead */

static uint8_t *back_buffer; // off-screen buffer added to implement a playable character

int(VG_init)(uint16_t mode) {
  reg86_t r86;

  /* Specify the appropriate register values */

  memset(&r86, 0, sizeof(r86)); /* zero the structure */

  r86.intno = 0x10; /* BIOS video services */
  r86.ah = 0x4F;    /* Set Video Mode function */
  r86.al = 0x02;    /* 80x25 text mode */

  r86.bx = mode | BIT(14);
  /* Make the BIOS call */
  if (sys_int86(&r86) != OK) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int(vg_map)(uint16_t mode) {

  struct minix_mem_range mr;
  unsigned int vram_base; /* VRAM's physical addresss */
  int r;

  /* Use VBE function 0x01 to initialize vram_base and vram_size */
  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info) != 0)
    return 1;

  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bits_per_pixel = (mode_info.BitsPerPixel + 7) / 8;

  vram_base = mode_info.PhysBasePtr;
  vram_size = h_res * v_res * bits_per_pixel;

  back_buffer = malloc(vram_size);
  if (back_buffer == NULL) {
    printf("Failed to allocate frame buffer\n");
    return 1;
  }

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return 0;
}

void(VG_clear)() {
  memset(back_buffer, 0, vram_size);
}

void swap_buffers() {
  memcpy(video_mem, back_buffer, vram_size);
}

int(vg_draw_sprite_clipped)(sprite_t *sprite) {
  int x0 = sprite->position.x;
  int y0 = sprite->position.y;
  int w = sprite->width;
  int h = sprite->height;

  for (int y = 0; y < h; y++) {
    int screen_y = y0 + y;
    if (screen_y < 0 || screen_y >= (int)v_res) continue;

    for (int x = 0; x < w; x++) {
      int screen_x = x0 + x;
      if (screen_x < 0 || screen_x >= (int)h_res) continue;

      uint8_t *video_map = (uint8_t *)back_buffer + (screen_y * h_res + screen_x) * bits_per_pixel;
      uint32_t sprite_pos = (h - 1 - y) * w * bits_per_pixel + x * bits_per_pixel;

      if (bits_per_pixel == 4) {
        uint8_t alpha = sprite->video_map[sprite_pos + 3];
        if (alpha == 0)
          continue;
      }
      memcpy(video_map, &sprite->video_map[sprite_pos], bits_per_pixel);
    }
  }
  return 0;
}

int(vg_draw_sprite)(sprite_t *sprite) {
  for (int y = sprite->position.y; y < sprite->position.y + sprite->height; y++) {
    for (int x = sprite->position.x; x < sprite->position.x + sprite->width; x++) {


      uint8_t *video_map = (uint8_t *) back_buffer + (y * h_res + x) * bits_per_pixel;

      uint32_t sprite_pos = (sprite->height - 1 - (y - sprite->position.y)) * sprite->width * bits_per_pixel
                        + (x - sprite->position.x) * bits_per_pixel;
      if(bits_per_pixel == 4){
        uint8_t alpha = sprite->video_map[sprite_pos + 3];
        if (alpha == 0)
          continue;
      }
      if (memcpy(video_map, &sprite->video_map[sprite_pos], bits_per_pixel) == NULL)
        return 1;
    }
  }

  return 0;
}

int(vg_draw_sprite_flipped)(sprite_t *sprite) {
  for (int y = sprite->position.y; y < sprite->position.y + sprite->height; y++) {
    for (int x = sprite->position.x; x < sprite->position.x + sprite->width; x++) {

      uint8_t *video_map = (uint8_t *) back_buffer + (y * h_res + x) * bits_per_pixel;

      uint32_t sprite_pos = (sprite->height - 1 - (y - sprite->position.y)) * sprite->width * bits_per_pixel
                        + (sprite->width - 1 - (x - sprite->position.x)) * bits_per_pixel;
      if(bits_per_pixel == 4){
        uint8_t alpha = sprite->video_map[sprite_pos + 3];
        if (alpha == 0)
          continue;
      }
      if (memcpy(video_map, &sprite->video_map[sprite_pos], bits_per_pixel) == NULL)
        return 1;
    }
  }

  return 0;
}
