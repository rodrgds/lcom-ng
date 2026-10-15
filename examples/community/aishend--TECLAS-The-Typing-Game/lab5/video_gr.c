#include <lcom/lcf.h>
#include "video_gr.h"
#include "vbe_macros.h"

/* Static global variables */
static char *video_mem;          /* frame-buffer VM address */

/* Other variables that might be used */
static vbe_mode_info_t vmi;      /* VBE mode info */
static unsigned bytes_per_pixel; /* Number of VRAM bytes per pixel */

void * (vg_init)(uint16_t mode) {

  int r;
  struct minix_mem_range mr;
  unsigned int vram_base;          /* VRAM's physical addresss */
  unsigned int vram_size = 0;          /* VRAM's size, but you can use the frame-buffer size */

  if (vbe_get_mode_info(mode, &vmi) != 0) {
    return NULL; 
  }
  // calcular o tamanho da vram
  bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8; // mais sete, arredondar por excesso
  vram_base = vmi.PhysBasePtr;

  vram_size = vmi.XResolution * vmi.YResolution * bytes_per_pixel;
  
  /* Grant memory mapping permissions */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;  

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED) {
    panic("couldn't map video memory");

  }

  // mudar para o modo grafico

  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ax = 0x4F02;
  r86.bx = mode | BIT(14); // ativamos o linear buffer 


  if (sys_int86(&r86) != OK) return NULL; 

  return video_mem; 
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  // nao pintar fora do display
  if (x >= vmi.XResolution || y >= vmi.YResolution) return 0; 

  // 2. Calcular o endereço (video_mem + offset)
  // O offset é: (y * largura + x) * bytes_por_pixel
  char *pixel_ptr = (char *)video_mem + (y * vmi.XResolution + x) * bytes_per_pixel;

  // 3. Copiar a cor (apenas os bytes necessários para este modo)
  if (memcpy(pixel_ptr, &color, bytes_per_pixel) == NULL) return 1;

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  // esquerdda para a direita 
  for(int i = 0; i < len; i++) {
  
    if (vg_draw_pixel(x + i, y, color) != 0) {
        return 1; 
    }
  }
  return 0; 
}

int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img; 
  
  // convewrter para cores
  // A função xpm_load lê o mapa (xpm) e preenche a nossa estrutura img.
  // Usamos XPM_INDEXED porque o modo 0x105 usa cores de 1 byte (indexado euc acho ).
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  
  if (colors == NULL) return 1; 

  //tracking para ir avancando no array
  int count = 0;
  
  // percorremos todas as posicoes
  for (int i = 0; i < img.height; i++) {       
    for (int j = 0; j < img.width; j++) {    
      // desenhamos o pixel e avancamos x maiss i e y + i 
      vg_draw_pixel(x + j, y + i, colors[count]);
      
      count++; // Atualizar o nosso tracking
    }
  }
  
  return 0;
}
