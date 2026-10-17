#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>


int (switch_to_graphic_mode)(uint16_t submode) {

    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86)); 

    reg86.intno = 0x10;                
    reg86.ah = 0x4F;               
    reg86.al = 0x02;                  
    reg86.bx = submode | BIT(14);  

    return(sys_int86(&reg86));
}


int (set_frame_buffer)(uint16_t submode, uint8_t** frame_buffer) {
  memset(&vbe_info, 0, sizeof(vbe_info));
  if(vbe_get_mode_info(submode, &vbe_info)) return 1;
  unsigned int bytes_per_pixel= (vbe_info.BitsPerPixel + 7)/8;
  unsigned int x_resolution = vbe_info.XResolution;
  unsigned int y_resolution = vbe_info.YResolution;
  unsigned int size = (x_resolution * y_resolution * bytes_per_pixel); 
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = vbe_info.PhysBasePtr; 
  physic_addresses.mr_limit = physic_addresses.mr_base + size;  
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) return 1;
  *frame_buffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, size);
  return (frame_buffer == NULL);

}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer) {

  if (color == TRANSPARENT_COLOR || x >= vbe_info.XResolution || y >= vbe_info.YResolution) return 0;

  unsigned int bytes_per_pixel = (vbe_info.BitsPerPixel + 7) / 8;
  unsigned int index = (vbe_info.XResolution * y + x) * bytes_per_pixel;

  return (memcpy(&frame_buffer[index], &color, ((vbe_info.BitsPerPixel + 7) / 8))==NULL);

}
