#include "gpu.h"
#include "vbe.h"

static void *video_mem;
static void *buffer;
static void *screenshot;
unsigned int h_res;	
unsigned int v_res;	
static unsigned int bits_per_pixel; 
static unsigned int bytes_per_pixel;

static uint8_t red_mask_size;
static uint8_t green_mask_size;
static uint8_t blue_mask_size;
static uint8_t red_pos;
static uint8_t green_pos;
static uint8_t blue_pos;

static bool index_mode;

int (get_vbe_mode_info)(uint16_t mode, vbe_mode_info_t *vbe_info) {
    
    if (mode != MODE1 && mode != MODE2 && mode != MODE3 && mode != MODE4 && mode != MODE5) {
        return 1;
    }

    mmap_t map;

    if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
        return 1;
    }

    phys_bytes mmap_addr = map.phys;

    reg86_t r86;
    memset(&r86, 0, sizeof(r86));


    r86.intno = INT_10; 
    r86.ah = WRT_FUNC;    
    r86.al = VBE_GET_MODE;  

    r86.cx = mode;
    r86.es = PB2BASE(mmap_addr);
    r86.di = PB2OFF(mmap_addr);

    if (sys_int86(&r86)) {
        return 1;
    }

    if (r86.ah != FUNC_SUCCESS) {
        return 1;
    }

    memcpy(vbe_info, map.virt, sizeof(vbe_mode_info_t));

    lm_free(&map);

    return 0;
}

int (set_mode)(uint16_t mode){

  if (mode == MODE1)
    index_mode = true;
  else 
    index_mode = false;

  reg86_t r86;
  
  memset(&r86, 0, sizeof(r86));	

  r86.intno = INT_10; 
  r86.ah = WRT_FUNC;    
  r86.al = VBE_SET_MODE;  
  r86.bx = mode | LINEAR_FRAME_BUFFER;

  if( sys_int86(&r86) != OK ) {
    return 1;
  }

  if (r86.al != FUNC_SUPPORTED){
    return 1;
  }
  else if (r86.ah != 0x00){
    return 1;
  }
  
  return 0;
}

int (map_info)(vbe_mode_info_t* vmi_p){

  int r;				    
  unsigned int vram_base;  
  unsigned int vram_size;      
  struct minix_mem_range mr;
  
  h_res = vmi_p->XResolution;
  v_res = vmi_p->YResolution;
  bits_per_pixel = vmi_p->BitsPerPixel; 
  bytes_per_pixel = (bits_per_pixel + 7) / 8.0;   
  red_mask_size = vmi_p->RedMaskSize;
  green_mask_size = vmi_p->GreenMaskSize;
  blue_mask_size = vmi_p->BlueMaskSize;
  vram_base = vmi_p->PhysBasePtr;
  red_pos = vmi_p->RedFieldPosition;
  green_pos = vmi_p->GreenFieldPosition;
  blue_pos = vmi_p->BlueFieldPosition;

  vram_size = h_res * v_res * bytes_per_pixel;    
  
  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if(OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  
  if(video_mem == MAP_FAILED){
    panic("couldn't map video memory");
    return 1;    
  }
  
  buffer = malloc(vram_size);
  screenshot = malloc(vram_size);
  memset(video_mem, 0, vram_size);
  memset(buffer, 0, vram_size);
  memset(screenshot, 0, vram_size);
  
  return 0;
}

int (clean_screen)(){
  
    memset(buffer, 0, h_res * v_res * bytes_per_pixel);
  
    return 0; 
}

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

  if (x < 0 || x >= h_res || y < 0 || y >= v_res) {

    return 1;
  }

  for (unsigned int cur_y = y; cur_y < y + height && cur_y < v_res; cur_y++){

    for (unsigned int cur_x = x; cur_x < x + width && cur_x < h_res; cur_x++){
       
      uint8_t* pixel_pos = (uint8_t*)buffer + (cur_y * h_res + cur_x) * bytes_per_pixel;

      memcpy(pixel_pos, &color, bytes_per_pixel);
    }
  }

  return 0;
}

int (copy_buffer_to_screenshot)(){
  
  memcpy(screenshot, buffer, h_res * v_res * bytes_per_pixel);

  return 0;
}

int (draw_screenshot_to_buffer)(uint32_t x, uint32_t y, uint32_t width, uint32_t height){

  for (unsigned int cur_y = y; cur_y < y + height && cur_y < v_res; cur_y++){

    for (unsigned int cur_x = x; cur_x < x + width && cur_x < h_res; cur_x++){
       
      uint8_t* pixel_pos = (uint8_t*)buffer + (cur_y * h_res + cur_x) * bytes_per_pixel;
      uint8_t* screenshot_pos = (uint8_t*)screenshot + (cur_y * h_res + cur_x) * bytes_per_pixel;

      memcpy(pixel_pos, screenshot_pos, bytes_per_pixel);
    }
  }

  return 0;
}

int (copy_buffer_to_video_mem)(){
  
  memcpy(video_mem, buffer, h_res * v_res * bytes_per_pixel);

  return 0;
}

uint8_t R(uint32_t first) {
    return (first >> (green_mask_size + blue_mask_size)) & ((1 << red_mask_size) - 1);
}

uint8_t G(uint32_t first) {
    return (first >> blue_mask_size) & ((1 << green_mask_size) - 1);
}

uint8_t B(uint32_t first) {
    return first & ((1 << blue_mask_size) - 1);
}

int (draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step){

  uint16_t width = h_res / no_rectangles;
  uint16_t height = v_res / no_rectangles;
  
  for (uint16_t row = 0; row < no_rectangles; row++){
    for (uint16_t col = 0; col < no_rectangles; col++){

        uint32_t color;

        if (index_mode)
          color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel); 
        else{
          uint32_t red = (R(first) + col * step) % (1 << red_mask_size); 
          uint32_t green = (G(first) + row * step) % (1 << green_mask_size); 
          uint32_t blue  = (B(first) + (col + row) * step) % (1 << blue_mask_size);

          color = (red << (green_mask_size + blue_mask_size)) | (green << blue_mask_size) | blue;
        }

        if (draw_rectangle(col * width, row * height, width, height, color))
          return 1;
    }
  }
  return 0;
}

int get_xpm_image_type(enum xpm_image_type* type, uint16_t mode){

  switch (mode){
    case 0x105:
      *type = XPM_INDEXED;
      break;
    case 0x110:
      *type = XPM_1_5_5_5;
      break;
    case 0x115:
      *type = XPM_8_8_8;
      break;
    case 0x11A:
      *type = XPM_5_6_5;
      break;
    case 0x14C:
      *type = XPM_8_8_8_8;
      break;
    default:
      return 1;
  }

  return 0;
}

int (load_xpm)(xpm_map_t xpm, enum xpm_image_type type, xpm_image_t* sprite,  uint8_t** img_addr){
  
  if ((*img_addr = xpm_load(xpm, type, sprite)) == NULL)
    return 1;

  return 0;
}

int (draw_xpm)(xpm_image_t sprite, uint8_t* img_addr, uint16_t x, uint16_t y){

  for(unsigned int cur_y = y; cur_y < y + sprite.height && cur_y < v_res; cur_y++){

    for(unsigned int cur_x = x; cur_x < x + sprite.width && cur_x < h_res; cur_x++){

      unsigned int img_x = cur_x - x, img_y = cur_y - y;

      uint8_t* color = img_addr + ((img_y * sprite.width) + img_x) * bytes_per_pixel;

      if (*((uint32_t*)color) == 0xFF00FF){
        continue;
      }

      uint8_t* pixel_pos = (uint8_t*)buffer + (cur_y * h_res + cur_x) * bytes_per_pixel;

      memcpy(pixel_pos, color, bytes_per_pixel);
    }
  }

  return 0;
}

int (erase_xpm)(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y){

  xpm_image_t img_info;

  uint8_t* img_addr;
  
  if ((img_addr = xpm_load(xpm, type, &img_info)) == NULL)
    return 1;

  for(unsigned int cur_y = y; cur_y < y + img_info.height && cur_y < v_res; cur_y++){

    for(unsigned int cur_x = x; cur_x < x + img_info.width && cur_x < h_res; cur_x++){

      unsigned int color = 0;

      uint8_t* pixel_pos = (uint8_t*)buffer + (cur_y * h_res + cur_x) * bytes_per_pixel;

      memcpy(pixel_pos, &color, bytes_per_pixel);
    }
  }
  return 0;
}

bool (move_sprite)( uint16_t* xi, uint16_t* yi, uint16_t xf, uint16_t yf, int16_t speed, uint32_t frame_count){

  if (*xi == xf){
    if (speed < 0){
      if (frame_count % (-speed) == 0){
        if (yf > *yi){
          (*yi)++;
          return *yi >= yf;
        }
        else{
          (*yi)--;
          return *yi <= yf;
        }
      }

    } else {
      if (yf > *yi){
        *yi += speed;
        return *yi >= yf;
      } else {
        *yi -= speed;
        return *yi <= yf;
      }
    }
  } else {
    if (speed < 0){
      if (frame_count % (-speed) == 0) {
        if (xf > *xi){
          (*xi)++;
          return *xi >= xf;  
        }
        else{
          (*xi)--;
          return *xi <= xf;  
        }
      }
    } else {
      if (xf > *xi){
        *xi += speed;
        return *xi >= xf;
      } else{
        *xi -= speed;
        return *xi <= xf;
      }
    }
  }
  return false;
}

int (draw_xpm_letter)(xpm_image_t sprite, uint8_t* img_addr, uint16_t x, uint16_t y, char letter){

  for(unsigned int cur_y = y; cur_y < y + sprite.height && cur_y < v_res; cur_y++){

    for(unsigned int cur_x = x; cur_x < x + LETTER_WIDTH && cur_x < h_res; cur_x++){
 
      unsigned int img_x = cur_x - x, img_y = cur_y - y;

      uint32_t offset =  (letter - 'A') * LETTER_WIDTH + 5;

      uint8_t* color = img_addr + (offset + (img_y * sprite.width) + img_x)*bytes_per_pixel;

      if (*((uint32_t*)color) == 0xFF00FF){
        continue;
      }

      uint8_t* pixel_pos = (uint8_t*)buffer + (cur_y * h_res + cur_x) * bytes_per_pixel;

      memcpy(pixel_pos, color, bytes_per_pixel);
    }
  }

  return 0;
}

int (draw_xpm_word)(xpm_image_t sprite, uint8_t* img_addr, uint16_t x, uint16_t y, char* word){

  int back = 0;
  for(size_t i = 0; i < strlen(word); i++){
    
    if (word[i] < 'A' || word[i] > 'Z')
      continue;
      
    if(draw_xpm_letter(sprite, img_addr, x + (i - back) * LETTER_WIDTH, y, word[i])) 
      	return 1;
  }

  return 0;
}

int (draw_number)(xpm_image_t sprite, uint8_t* img_addr, uint16_t x, uint16_t y, char number){

  for(unsigned int cur_y = y; cur_y < y + sprite.height && cur_y < v_res; cur_y++){

    for(unsigned int cur_x = x; cur_x < x + sprite.width / 12 && cur_x < h_res; cur_x++){

      unsigned int img_x = cur_x - x, img_y = cur_y - y;

      unsigned int offset = (number - '/') * sprite.width / 12;

      uint8_t* color = img_addr + offset * bytes_per_pixel + ((img_y * sprite.width) + img_x) * bytes_per_pixel;

      if (*((uint32_t*)color) == 0xFF00FF){
        continue;
      }

      uint8_t* pixel_pos = (uint8_t*)buffer + (cur_y * h_res + cur_x) * bytes_per_pixel;

      memcpy(pixel_pos, color, bytes_per_pixel);
    }
  }

  return 0;

}
