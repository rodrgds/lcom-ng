#include <lcom/lcf.h>
#include "platforms.h"
#include "devices/video.h"
#include "xpm/xpm_id.h"

void initialize_platforms(){
  Platform platform = {0,0,0,0};
  for(int i = 0; i < N_PLATFORMS; i++){
    platforms[i] = platform;

  }
    platforms_location(850,900,150,40,0);
    platforms_location(300,850,150,40,1);
    platforms_location(400,750,150,40,2);
    platforms_location(600,690,150,40,3);
}

void platforms_location(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint8_t number){

  platforms[number].x = x;
  platforms[number].y = y;
  platforms[number].width = width;
  platforms[number].height = height;

}


void draw_platforms(){

  for(int i = 0 ; i < N_PLATFORMS; i++){ 
    vg_draw_xpm(PLATFORFORM_XPM_ID,platforms[i].x,platforms[i].y);
  }

}
