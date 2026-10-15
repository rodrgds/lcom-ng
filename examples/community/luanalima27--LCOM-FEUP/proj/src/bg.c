#include <lcom/lcf.h>
#include "bg.h"
#include "rtc.h"
#include "video.h"


extern uint8_t hours;

int draw_mmbg(){
  extern uint8_t *mmbg_sprite;
  extern xpm_image_t img_mmbg;
  if (draw_xpm(mmbg_sprite,img_mmbg,0,0) != 0){
     return 1;  
  }
  return 0;
}

int delete_mmbg(){
  extern uint8_t *erasebg_sprite;
  extern xpm_image_t img_erasebg;
  if (draw_xpm(erasebg_sprite, img_erasebg,0,0) != 0){
    return 1;
  }
  return 0;
}
