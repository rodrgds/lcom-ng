#include <lcom/lcf.h>

#include <math.h>
#include <stdint.h>

#include "constants.h"
#include "dispatcher.h"
#include "elements.h"
#include "i8042.h"
#include "menus.h"
#include "pixmaps.h"
#include "video.h"
#include <lcom/liblm.h>
#include <lcom/utils.h>
#include <machine/int86.h> // /usr/src/include/arch/i386

static char *video_mem;          /* Process (virtual) address to which VRAM is mapped */
static unsigned h_res;           /* Horizontal resolution in pixels */
static unsigned v_res;           /* Vertical resolution in pixels */
static unsigned bits_per_pixel;  /* Number of VRAM bits per pixel */
static unsigned red_mask_size;   /* Size of direct color red mask in bits */
static unsigned green_mask_size; /* Size of direct color green mask in bits */
static unsigned blue_mask_size;  /* Size of direct color blue mask in bits */

int(set_vbe_mode)(uint16_t mode) {
  reg86_t r;

  memset(&r, 0, sizeof(r)); /* zero the structure */

  r.ax = VBE_CALL << 8 | SET_VBE_MODE; // VBE call, function 02 -- set VBE mode
  r.bx = LINEAR_MODEL | mode;          // set bit 14: linear framebuffer
  r.intno = INT_VIDEO;                 /* BIOS video services */

  /* Make the BIOS call */
  if (sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;
}

int(get_vbe_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p) {
  reg86_t r;

  memset(&r, 0, sizeof(r)); /* zero the structure */

  mmap_t mem_map;

  lm_alloc(MODEINFOBLOCK_SIZE, &mem_map);

  r.ax = VBE_CALL << 8 | RETURN_VBE_MODE_INFO; // VBE call, function 01 -- return VBE mode information
  r.cx = mode;                                 // set mode number
  r.es = PB2BASE(mem_map.phys);                // pointer to ModeInfoBlock structure - set a segment base
  r.di = PB2OFF(mem_map.phys);                 // pointer to ModeInfoBlock structure - set the offset accordingly
  r.intno = INT_VIDEO;                         /* BIOS video services */

  /* Make the BIOS call */
  if (sys_int86(&r) != OK) {
    printf("get_vbe_mode_info: sys_int86() failed \n");
    return 1;
  }

  memcpy(vmi_p, mem_map.virt, MODEINFOBLOCK_SIZE);

  lm_free(&mem_map);

  return 0;
}

int(map_vram)(uint16_t mode) {

  struct minix_mem_range mr;
  unsigned int vram_base; /* VRAM's physical addresss */
  unsigned int vram_size; /* VRAM's size, but you can use the frame-buffer size, instead */

  /* Use VBE function 0x01 to initialize vram_base and vram_size */

  vbe_mode_info_t vmi_p;

  get_vbe_mode_info(mode, &vmi_p);

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;

  /* Direct color */
  if (bits_per_pixel != 8) {
    red_mask_size = vmi_p.RedMaskSize;
    green_mask_size = vmi_p.GreenMaskSize;
    blue_mask_size = vmi_p.BlueMaskSize;
  }

  vram_base = vmi_p.PhysBasePtr;
  vram_size = h_res * v_res * (bits_per_pixel / 8);

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  int r;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return 0;
}

int(draw_pixmap)(uint8_t *pixmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

  if (x >= h_res || y >= v_res) {
    printf("Out of range\n");
    return 1;
  }

  uint16_t w = width, h = height;

  if (x + width > h_res) {
    w = h_res - x;
  }

  if (y + height > v_res) {
    h = v_res - y;
  }

  char *start = video_mem + (y * h_res + x) * (bits_per_pixel / 8);

  uint8_t *px_pos = pixmap;

  bool transparent = true;

  for (unsigned int row = 0; row < h; row++) {
    for (unsigned int col = 0; col < w; col++) {
      px_pos = pixmap + (row * width + col) * (bits_per_pixel / 8);
      for (unsigned int n = 0; n < bits_per_pixel / 8; n++) {
        if (*(px_pos + n) != (uint8_t)((TRANSPARENCY_COLOR >> (n * 8)) & 0xff)) {
          transparent = false;
          continue;
        }
      }
      if (!transparent) {
        for (unsigned int n = 0; n < bits_per_pixel / 8; n++) {
          *(start + (row * h_res + col) * (bits_per_pixel / 8) + n) = *(px_pos + n);
        }
      }
      transparent = true;
    }
  }

  return 0;
}

int(erase_pixmap)(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

  if (x >= h_res || y >= v_res) {
    printf("Out of range\n");
    return 1;
  }

  uint16_t w = width, h = height;

  if (x + width > h_res) {
    w = h_res - x;
  }

  if (y + height > v_res) {
    h = v_res - y;
  }

  char *start = video_mem + (y * h_res + x) * (bits_per_pixel / 8);

  uint8_t *background;
  if (get_state() == MAIN_MENU)
    background = get_main_menu_pixmap();
  else if (get_state() == INSTRUCTIONS_MENU)
    background = get_instructions_pixmap();
  else
    background = get_background();

  uint8_t *bg_start = background + (y * h_res + x) * (bits_per_pixel / 8);

  for (unsigned int row = 0; row < h; row++) {
    for (unsigned int col = 0; col < w; col++) {
      for (unsigned int n = 0; n < bits_per_pixel / 8; n++) {
        *(start + (row * h_res + col) * (bits_per_pixel / 8) + n) = *(bg_start + (row * h_res + col) * (bits_per_pixel / 8) + n);
      }
    }
  }

  return 0;
}

unsigned int(get_hres)() {
  return h_res;
}

unsigned int(get_vres)() {
  return v_res;
}
