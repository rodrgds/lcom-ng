// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <machine/int86.h>
#include "vdh.h"
#include "i8254.h"
#include <math.h>

static void * video_mem;
static char * double_buf;        /*double buffer will be a copy of video mem (page flipping)*/
static unsigned char * rd_buf;   /*third buffer - for collision detection*/
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

static vbe_mode_info_t vbe; /* structure to be initialized  with the VBE information */
static mmap_t map; /* mapping of physical memory to virtual memory */

int set_graphic_mode(uint16_t mode){
  struct minix_mem_range mr;
  unsigned int vram_base; 
  unsigned int vram_size;
  map.phys = 0;
  map.size = 1024*1024;

  lm_alloc(1024*1024, &map);
  
  memset(&vbe, 0, sizeof(vbe)); /* zero the structure */
  
  if(vbe_get_info(mode) != OK) return NOT_OK;

  vram_base = vbe.PhysBasePtr; 
  h_res = vbe.XResolution;
  v_res = vbe.YResolution;
  bits_per_pixel = vbe.BitsPerPixel;
  vram_size = v_res * h_res * ((bits_per_pixel+7)/8);
  
  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = vram_base + vram_size;  
  
  int r;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    printf("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NOT_OK;
  }

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED){
    printf("couldn't map video memory");
    return NOT_OK;
  }

  double_buf = (char*) malloc(vram_size);//void *malloc(size_t size) - sizeof video mem = size of doub_buf(is a copy of video_mem)
	rd_buf = (unsigned char*) malloc(vram_size);

  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.ax = CALL_VBE | VBE_SET_MODE; // VBE call, function 02 -- set VBE mode
  reg.bx = 1<<14 | mode; // set bit 14: linear framebuffer
  reg.intno = BIOS_VIDEO;
  if(sys_int86(&reg) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return NOT_OK;
  }

  lm_free(&map);

  return OK;
}

int vbe_get_info(uint16_t mode){
  
  lm_alloc(sizeof(vbe_mode_info_t), &map);

  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.ax = CALL_VBE | GET_MODE_INFO; /* VBE get mode info */
  /* translate the buffer linear address to a far pointer */
  reg.es = PB2BASE(map.phys); /* set a segment base */
  reg.di = PB2OFF(map.phys); /* set the offset accordingly */
  reg.cx = mode;
  reg.intno = BIOS_VIDEO;

  if( sys_int86(&reg) != OK )
    return NOT_OK;

  memcpy(&vbe,map.virt,map.size);
  lm_free(&map);
  
  return OK;
}

int (vg_draw_hline2)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, void * buff){
  for(unsigned int i = 0; i < len ; i++){
    if(x+i >= h_res) return OK;
    if(changePixelColor2(x+i,y,color,buff) != OK) return NOT_OK;
  }
  return OK; 
}

int (vg_draw_rectangle2)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, void * buff){

  for(unsigned int i = 0; i < height; i++){
    if(y+i >= v_res) return OK;
    if(vg_draw_hline2(x,y+i,width,color,buff) != OK) return NOT_OK;
  }
  return OK;
}

int (changePixelColor2)(uint16_t x, uint16_t y, uint32_t color, void * buff){
  
  if(x >= h_res || y >= v_res || x < 0 || y < 0) return OK;

  unsigned bytes_per_pixel = (bits_per_pixel + 7)/8;
  
  uint8_t * ptr = (uint8_t*) buff + (x * bytes_per_pixel) + (y * h_res * bytes_per_pixel);

  uint8_t temp;

  for(unsigned int i = 0; i < bytes_per_pixel; i++){
    temp = color & 0xFF;
    *(ptr + i) = temp;
    color = color >> 8;
  }

  return OK;
}

unsigned getBitsPerPixel(){
  return bits_per_pixel;
}

int getHres(){
  return h_res;
}

int getVres(){
  return v_res;
}

void* get_vid_mem() {
	return video_mem;
}

char* get_double_buffer() {
	return double_buf;
}

unsigned char* get_rd_buffer() {
	return rd_buf;
}

void change_buffers(){
	memcpy(video_mem, double_buf, (h_res * v_res * ceil(bits_per_pixel / 8.0)));
}

