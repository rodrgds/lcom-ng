#include <graphics.h>


//struct that contains info about the graphics mode
vbe_mode_info_t mode_info;

//pointer to the beginning of the virtual mem
uint8_t *frame_buffer;



//kbd related
extern uint8_t scancode;

int (wait_for_ESC)() {
    int ipc_status, r;
    message msg;
    uint8_t kbd_irq_set;
  
    //we subscribe interrupt 
    if (kbd_subscribe_int(&kbd_irq_set) != 0) return 1;
  
    while (scancode != 0x81) { 
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }
      
      if (is_ipc_notify(ipc_status)) { // we check if the notification is from the KBC
        if (_ENDPOINT_P(msg.m_source) == HARDWARE && msg.m_notify.interrupts & kbd_irq_set) {
          kbc_ih();
          process_scancode(scancode);
        }
      }
    }
  
    if (kbd_unsubscribe_int() != 0) return 1;
    return 0;
  }



/*graphics*/

//3.1 set the graphics mode
//https://pages.up.pt/~up722898/aulas/lcom2425/lab5/lab5_3.html
int (set_vbe_mode)(uint16_t mode) {
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


/*
instead of using the function above we would do the init like this - w/ vg_init
problem: the unflexibily of the LCF tests, every test that acesses the VRAM must call set_frame_buffer() before set_vbe_mode()

void *(vg_init)(uint16_t mode) {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ah = 0x4F;
  reg86.al = 0x02;
  //same as doing reg.ax = 0x4F02; 
  reg86.bx = mode | BIT(14); 
  if(sys_int86(&reg86) != 0) {
    printf("vg_init: sys_int86() failed \n");
    return NULL;
  }

  if (set_frame_buffer(mode) != 0) {
    printf("vg_init: set_frame_buffer() failed \n");
    return NULL;
  }

  return frame_buffer;
}
*/



int (set_frame_buffer)(uint16_t mode) {
  
  //we get the mode info
  memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info(mode, &mode_info) != 0) return 1;


  //we calculate the buffer size, in bytes
  unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

  //we define physical ranges
  struct minix_mem_range phys_addresses;
  phys_addresses.mr_base = mode_info.PhysBasePtr; //phys beggining of the buffer
  phys_addresses.mr_limit = mode_info.PhysBasePtr + frame_size; //phys end of the buffer

  //we allocate mem (physically) for the frame buff
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &phys_addresses) != 0) return 1;
  

  //as seen in SO, a program should only deal if virtual mem addresses
  //so here we allocate mem (virtually) for the frame buff
  frame_buffer = vm_map_phys(SELF, (void *) phys_addresses.mr_base, frame_size);
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



int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  
  //we check if the coordinates are valid
  if (x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;
  
  //we calulate the bytes per pixel of the color chosen
  unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;

  //index (in bytes) of the location of the pixel to draw
  unsigned int index = (x + mode_info.XResolution * y) * bytes_per_pixel;

  //using the frame buffer pointer, we copy n bytes of the color given on the right place (index)
  if (memcpy(&frame_buffer[index], &color, bytes_per_pixel) == NULL) return 1;
  
  return 0;
}


int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0) return 1;
  }
  return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color) != 0) return 1;
  }
  return 0;
}







//see ppt 8xpm-1 page 8/9 about xmp_load
int(print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
  xpm_image_t img; // pixmap and metadata
  uint8_t *map; // pixmap itself

  // get the pixmap from the XPM
  map = xpm_load(xpm, XPM_INDEXED, &img);
  if (map == NULL) {
    printf("Error loading pixmap\n");
    return 1;
  }

  // copy it to graphics memory
  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (vg_draw_pixel(x + j, y + i, *map) != 0) return 1;
      map++;
    }
  }
  
  return 0;
}


