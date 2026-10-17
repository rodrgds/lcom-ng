#include <lcom/lcf.h>
#include "video.h"



int(vg_start)(uint16_t mode) {
  lm_init(false);

	vbe_mode_info_t vbe_mode;
  vbe_get_mode_info(mode, &vbe_mode);

  v_res = vbe_mode.YResolution;
  h_res = vbe_mode.XResolution;
  bits_per_pixel = vbe_mode.BitsPerPixel;
  memory_model = vbe_mode.MemoryModel;


  BlueScreeMask = vbe_mode.BlueMaskSize;
  GreenScreeMask = vbe_mode.GreenMaskSize;
  RedScreeMask = vbe_mode.RedMaskSize;

  RedFieldPosition = vbe_mode.RedFieldPosition;
  BlueFieldPosition = vbe_mode.BlueFieldPosition;
  GreenFieldPosition = vbe_mode.GreenFieldPosition;






  int r_mem;
  struct minix_mem_range mr;
  unsigned int vram_size = h_res * v_res * (bits_per_pixel / 8);
  

  mr.mr_base = (phys_bytes) vbe_mode.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;
  if (OK != (r_mem = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r_mem);
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

  back_buffer = malloc(vram_size);

  struct reg86u reg86;
  memset(&reg86, 0, sizeof(reg86)); /* zero the structure */
  reg86.u.w.ax = 0x4F02;            // VBE call, function 02 -- set VBE mode
  reg86.u.w.bx = (1 << 14) | mode;    // set bit 14: linear framebuffer
  reg86.u.b.intno = 0x10;

  if (sys_int86(&reg86) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

    return 0;
}

int(get_h_res)(){
  return h_res;
}
int(get_v_res)(){
  return v_res;
}
int(get_bits_per_pixel)(){
  return bits_per_pixel;
}
void * (get_video_mem)(){
  return video_mem;
}
void * (get_back_buffer)() {
  return back_buffer;
}
uint8_t (get_memory_model)(){
  return memory_model;
}
uint8_t(get_blue_screen_mask)(){
  return BlueScreeMask;
}
uint8_t(get_green_screen_mask)(){
  return GreenScreeMask;
}
uint8_t(get_red_screen_mask)(){
  return RedScreeMask;
}
uint8_t(get_blue_screen_mask_position)(){
  return BlueFieldPosition;
}
uint8_t(get_green_screen_mask_position)(){
  return GreenFieldPosition;
}
uint8_t(get_red_screen_mask_position)(){
  return RedFieldPosition;
}


void (clearScreen)(){
  for (unsigned i = 0; i < h_res; i++)
    for (unsigned j = 0; j < v_res; j++){
      changePixel(i,j,0x0000);
    }
}

uint16_t (getpixel)(int x, int y){
    uint16_t color;
    int h_res = get_h_res();
  //int bits_per_pixel = get_bits_per_pixel();
    void *video_mem = get_video_mem();
    uint16_t *ptr_VM = (uint16_t*)video_mem;
    ptr_VM += (y * h_res + x);

    color = *ptr_VM;

    return color;
}

void (changePixel)(int x, int y, uint16_t color){
  int h_res = get_h_res();
  void *video_mem = get_video_mem();

  uint16_t *ptr_VM = (uint16_t*)video_mem;
  ptr_VM += (y * h_res + x);

  *ptr_VM = color;

  /*
  void *vm = get_video_mem();
  char *ptr_VM = vm;
  ptr_VM += (i + h_res * j) * (bits_per_pixel / 8);
  *ptr_VM = color;*/
}

