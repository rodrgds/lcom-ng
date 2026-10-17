#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "vbe_graphics.h"
#include "keyboard.h"
#include "i8042.h"

vbe_mode_info_t vmi_p;

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
uint16_t mode_aux = 0; 


static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
  
unsigned int vram_base;
unsigned int vram_size;

char* buffer_aux;


void *(vg_init)(uint16_t mode){
  reg86_t reg86;
  struct minix_mem_range mr; /* physical memory range */

  /*  Mudança para o respetivo modo gráfico  */

  memset(&reg86, 0, sizeof(reg86)); /* zero the structure */

  reg86.intno = 0x10;               /* BIOS video services  */     
  reg86.ah = 0x4F;                  /* Set Video Mode function */
  reg86.al = 0x02;                  /* 80x25 graphics mode */
  
  reg86.bx = mode | BIT(14);

  if(vbe_get_mode_info(mode, &vmi_p) != 0){
    printf("Error in get mode info\n");
    return NULL;
  }

  vram_base = vmi_p.PhysBasePtr;
  vram_size = vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel + 7)/8);

  buffer_aux = (char *) malloc(vram_size);

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = (vmi_p.BitsPerPixel + 7) / 8;

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vmi_p.PhysBasePtr;	
  mr.mr_limit = mr.mr_base + vram_size; 

  if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))
    panic("sys_privctl (ADD_MEM) failed");

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if (sys_int86(&reg86) != 0) {     
    printf("Set graphic mode failed\n");
    return NULL;
  }

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return video_mem;
}

//Draw a rectangle
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (int i = 0; i <height; i++)
    vg_draw_hline(x, y+i, width, color);
  
  return 0;
}

int (vg_draw_vline)(uint16_t x,uint16_t y,uint16_t len, uint32_t color) 	
{
  uint16_t yfinal = y + len;

  //in case number is not even 
  while(y < yfinal)
  { 
    vg_draw_pixel(x,y,color);
    y++;
  }
    
  return 0;
}


// Draw horizontal line
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0 ; i < len ; i++)   
    vg_draw_pixel(x+i, y, color);
  return 0;
}


int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    uint8_t bytesPerPixel = (vmi_p.BitsPerPixel + 7 ) / 8;

    char* dest = (char*) buffer_aux + (y * vmi_p.XResolution * bytesPerPixel) + x * bytesPerPixel;
    if (mode_aux == 0x110) {
        color = color & 0x7FFF;
    }

    memcpy(dest, &color, bytesPerPixel);

    
    return 0;
}

int (draw_xpm)(xpm_image_t* image, uint16_t x, uint16_t y) {
    uint8_t* colour = (uint8_t*) image->bytes;
    uint32_t transp_colour = xpm_transparency_color(XPM_8_8_8);

    for (int yf = 0; yf < image->height; yf++){
        for ( int xf = 0; xf < image->width; xf++){
            if (transp_colour != (*( (uint32_t*) colour) & 0xFFFFFF)){
                vg_draw_pixel(x + xf, y + yf, *( (uint32_t*) colour));
            }
            
            colour = colour + ( (vmi_p.BitsPerPixel + 7) / 8);
        }
    }

    return 0;
}


void (erase_xpm)(uint16_t x, uint16_t y, xpm_image_t *img) {
  for(size_t i=0; i<img->height; i++){
      //memset(buffer + (x+(y+i)*h_res)*bytes_per_pixel,*(map+(i*img.width)*bytes_per_pixel),img.width*bytes_per_pixel);
      for(size_t j=0; j<img->width; j++){
        vg_draw_pixel(x+j, y+i,0x000);
      }
  }
}

char * get_mem()
{
  return video_mem;
}

char * get_buffer()
{
  return buffer_aux;
}

unsigned get_vram_size() 
{
  return vram_size;
}

void (clean_screen)() {
  memset(video_mem,0,vram_size);
}

void (clean_buffer)() {
  memset(buffer_aux,0,vram_size);
}

void video_flip(){
  memcpy(video_mem,buffer_aux,vram_size);
}
