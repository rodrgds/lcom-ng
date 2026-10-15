#include "graphics.h"


//struct that contains info about the graphics mode
vbe_mode_info_t mode_info;

//here we store the main frame
uint8_t *frame_buffer;

//and here we store the current one
uint8_t *current_buffer;


//frame size
unsigned int frame_size = 0;




int (set_graphics_mode)(uint16_t mode) {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ah = 0x4F;
  reg86.al = 0x02;
  //same as doing reg.ax = 0x4F02; 
  reg86.bx = mode | BIT(14); 
  if(sys_int86(&reg86) != 0) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}


int (set_text_mode)() {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ah = 0x00;
  reg86.al = 0x03; 
  reg86.bx = 0x0000;
  if(sys_int86(&reg86) != 0) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}





int (set_frame_buffer)(uint16_t mode) {
  
  //we get the mode info
  memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info(mode, &mode_info) != 0) return 1;


  //we calculate the buffer size, in bytes
  unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

  //we define physical ranges
  struct minix_mem_range phys_addresses;
  phys_addresses.mr_base = mode_info.PhysBasePtr; //phys beggining of the buffer
  phys_addresses.mr_limit = mode_info.PhysBasePtr + frame_size; //phys end of the buffer

  //we allocate mem (physically) for the frame buff
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &phys_addresses) != 0) return 1;
  

  //as seen in SO, a program should only deal if virtual mem addresses
  //so here we allocate mem (virtually) for the frame buff
  frame_buffer = vm_map_phys(SELF, (void *) phys_addresses.mr_base, frame_size);
  current_buffer = (uint8_t *) malloc(frame_size);

  if (frame_buffer == NULL) return 1;
  return 0;
}




int (normalize_color)(uint32_t color, uint32_t *new_color) {
  //if each pixel has 32 bits, we can use the color as it is
  if (mode_info.BitsPerPixel == 32) {*new_color = color;}
  
  //otherwise, we set the unnecessary bits to 0
  else {*new_color = color & (BIT(mode_info.BitsPerPixel) - 1);}

  return 0;
}



int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *buffer) {
  
  //we check if the coordinates are valid
  if (x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;
  
  //we calulate the bytes per pixel of the color chosen
  unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;

  //index (in bytes) of the location of the pixel to draw
  unsigned int index = (x + mode_info.XResolution * y) * bytes_per_pixel;

  //using the buffer pointer, we copy n bytes of the color given on the right place (index)
  if (memcpy(&buffer[index], &color, bytes_per_pixel) == NULL) return 1;
  
  return 0;
}


int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *buffer) {
  for (uint16_t i = 0; i < len; i++) {
    if (draw_pixel(x + i, y, color, buffer) != 0) return 1;
  }
  return 0;
}


int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *buffer) {
  for (uint16_t i = 0; i < height; i++) {
    if (draw_hline(x, y + i, width, color, buffer) != 0) return 1;
  }
  return 0;
}




int(fill_screen)(uint32_t color, uint8_t *buffer) {
  return draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, color, buffer);
}



int (swap_buffers)() {
  if (memcpy(frame_buffer, current_buffer, mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8)) == NULL) return 1;
  return 0;
}


int (free_buffer)(uint8_t *buffer) {
  if (buffer != NULL) {
    free(buffer);
    buffer = NULL;
  }
  return 0;
}


