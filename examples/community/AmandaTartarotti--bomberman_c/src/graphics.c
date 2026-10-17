#include "../inc/graphic.h"
#include "../inc/video.h"
#include "../inc/pixmap.h"
#include "../inc/rtc.h"
#include "../inc/draw.h"
#include "../inc/sprite.h"
#include "../inc/setup_game.h"
#include "../inc/data_structures.h"

vbe_mode_info_t vbe;
uint8_t *frame_buffer;
uint8_t *double_buffer;
uint8_t using_db = FALSE;
uint32_t frame_buffer_size;
extern state_game game_state;

int (set_mode)(uint16_t mode){
    struct reg86 r;

  memset(&r, 0, sizeof(r));
  r.intno = INT_VECTOR;
  r.ah = VBE_FUNCTION;
  r.al = SET_MODE;
  r.bx = BIT(14) | mode;
  

  if( sys_int86(&r) != 0 ) {return 1;}

  return 0;
}

int (map_vram)(uint16_t mode){
  struct minix_mem_range mr;
  unsigned int vram_base = 0;  /* VRAM's physical addresss */
  			
  int t;			    

  /* Use VBE function 0x01 to initialize vram_base and vram_size */
  if(vbe_get_mode_info(mode, &vbe) != 0) return 1;
  vram_base = vbe.PhysBasePtr;
  uint8_t bytes_p_pixel = (vbe.BitsPerPixel+7)/8;

  frame_buffer_size = bytes_p_pixel * vbe.XResolution * vbe.YResolution;


  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + frame_buffer_size;  

  if( OK != (t = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
      panic("sys_privctl (ADD_MEM) fail ed: %d\n", t);
      return 1;
  }

  /* Map memory */

  frame_buffer = vm_map_phys(SELF, (void *)mr.mr_base, frame_buffer_size);

  if(frame_buffer == MAP_FAILED){
     panic("couldn't map video memory");
     return 1;
  }
  return 0;
}

int alloc_double_buffer(uint16_t mode){
  vbe_mode_info_t db_info;
  if(vbe_get_mode_info(mode,&db_info)) return 1;

  uint8_t bytes_p_pixel = (db_info.BitsPerPixel+7)/8;
  unsigned int vram_size = bytes_p_pixel * db_info.XResolution * db_info.YResolution;

  double_buffer = (uint8_t*) malloc(vram_size);

  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if(x > vbe.XResolution || y > vbe.YResolution) return 1;
  unsigned BytesPerPixel = (vbe.BitsPerPixel + 7) / 8;
  unsigned int index = (vbe.XResolution * y + x) * BytesPerPixel;
  if(color != TRANSPARENT){
  if(using_db) {
        if (memcpy(&double_buffer[index], &color, BytesPerPixel) == NULL) return 1;
  }
  else{
    if (memcpy(&frame_buffer[index], &color, BytesPerPixel) == NULL) return 1;
  }
  }
  return 0;
}


int drawSpriteXPM(Sprite *sprite, int x, int y){
  if (sprite == NULL) return 1;

  uint16_t width = sprite->width;
  uint16_t height = sprite->height;

  for(int row = 0; row < height; ++row){
    for(int col = 0; col < width; col++){

      uint32_t color = sprite->colors[col + row * width];

      if (vg_draw_pixel(x + col, y + row, color)) return 1;
    }
  }

  return 0;
}

int (graphic_update)(){

if(using_db == TRUE){
    switch (game_state)
    {
    case MENU:
      if(erase_screen()) return 1;
      if(draw_menu()) return 1;
      break;
    
    case GAME:
      if(erase_screen()) return 1;
      if(draw_map()) return 1;
      if(arrow_draw()) return 1;
      if(create_clock()) return 1;
      break;
    case GAMEOVER:
      if(erase_screen()) return 1;
      if(draw_gameover()) return 1;
      break;
    case INSTRUCTIONS:
      if(erase_screen()) return 1;
      if(draw_instructions()) return 1;
      break;
    case WINNER:
      if(erase_screen()) return 1;
      if(draw_gamewin()) return 1;
      break;
    default:
      break;
    }
    using_db = FALSE;
  }

  return 0;
}
