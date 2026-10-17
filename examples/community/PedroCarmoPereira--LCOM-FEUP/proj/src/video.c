#include <minix/devio.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <liblm.h>
#include <lcom/lcf.h>

#include "video.h"

static char *video_mem;
static char *double_buffer;
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static vbe_mode_info_t vmi_p;

int set_mode(uint16_t mode){
  struct reg86u reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.u.b.intno = 0x10;
  reg86.u.w.ax =  0x4F02;
  reg86.u.w.bx = 1<<14 | mode;
  if( sys_int86(&reg86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int my_vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
    struct reg86u reg86;        /* Represents a register */
                    /* Holds the result of the sys_int86 call */
    mmap_t mmap;            /* Structure with info about the mapped memory region */

    // allocate memory in the lower 1 MB for the ModInfoStruct to be filled by the BIOS.
    // load the vmi_p struct with the info provided by the BIOS
    // free the low memory allocated
    // use the mapped virtual address to the ModInfoStruct to
    lm_init(false);
    if (lm_alloc(sizeof(vbe_mode_info_t), &mmap) == NULL) {
        panic("Couldn't allocate memory in the first MB region");
    }
    memset(&reg86, 0, sizeof(reg86));
    reg86.u.w.ax = 0x4F01; /* VBE get mode info */
    /* translate the buffer linear address to a far pointer */
    reg86.u.w.es = PB2BASE(mmap.phys); /* set a segment base */
    reg86.u.w.di = PB2OFF(mmap.phys); /* set the offset accordingly */
    reg86.u.w.cx = mode;
    reg86.u.b.intno = 0x10;

    if(sys_int86(&reg86) != OK) {
      fprintf(stderr, "vbe_get_mode_info(): sys_int86() failed \n");
      return 1;
    }

    *vmi_p = *(vbe_mode_info_t*) mmap.virt;

    lm_free(&mmap);

  return 0;
}

void* (vg_init)(uint16_t mode){

  struct minix_mem_range mr;
  int r;

  if(my_vbe_get_mode_info(mode, &vmi_p) != 0) return NULL;

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;
  unsigned int vram_base = vmi_p.PhysBasePtr;
  unsigned int vram_size = h_res*v_res * (bits_per_pixel / 8);

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
     panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
     panic("couldn't map video memory");

  if((double_buffer = malloc(h_res*v_res*bits_per_pixel)) == NULL){
    puts("Error");
  }

  return video_mem;
}

int putPixel(uint16_t x, uint16_t y, uint32_t color) {
    if (x < 0 || x >= h_res || y < 0 || y >= v_res) {
        fprintf(stderr, "Invalid position (%d, %d) to put a pixel.", x, y);
        return 1;
    }

    ((uint32_t *) double_buffer)[(y * h_res + x)] = color;

    return 0;
}

void clearScreen(){
  for (unsigned int x = 0; x < h_res; x++)
    for(unsigned int y = 0; y < v_res; y++)
      putPixel(x, y, 0);
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        putPixel(x, y, color);
        x++;
    }

    return 0;
}

void draw_sproot(xpm_image_t imt, int x0, int y0){
  uint32_t * color = (uint32_t *)imt.bytes;
  for(int i = y0; i < imt.height + y0; i++)
    for(int j = x0; j < imt.width + x0; j++){
      uint32_t pixel = *color++;
      if( pixel == xpm_transparency_color(imt.type)) continue;
      putPixel(j, i, pixel);
    }
}

void draw_background(xpm_image_t * im){
  memcpy(double_buffer, im->bytes, v_res*h_res*4);
}

int getVRes(){return v_res;}

int getHRes(){return h_res;}

void swap_buffer(){
  memcpy(video_mem,double_buffer, v_res*h_res*4);
}
