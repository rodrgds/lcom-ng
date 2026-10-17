#include "night_mode.h"

extern unsigned char* double_buffer;

uint8_t* highlighted_map;
xpm_image_t map_img;

void start_night_mode(xpm_map_t map_xpm) {
  night_mode_filter = malloc((get_vram_size()/3)*sizeof(bool));
  for(int i = 0; i < get_vram_size()/3; i++)
    night_mode_filter[i] = 1;

  highlighted_map = xpm_load(map_xpm, XPM_8_8_8, &map_img);
}

double get_value_to_multiply(){
  int hours = get_hours();
  int minutes = get_minutes();
  int combinacao = ((hours-3)%24)*60 + minutes;
  if(hours >= 15)
    combinacao = 1440 - combinacao;

  return (double) 0.4 + 0.6*((double) combinacao/(double) 720);
}

void apply_night_mode_on_xpm(uint8_t* pixmap, xpm_image_t img){
  for(int i = 0; i < img.height; i++)
    for(int j = 0; j < 600; j++){
      for(int k = 0; k < 3; k++){
        pixmap[i*img.width*3 + j*3 + k] = highlighted_map[i*img.width*3 + j*3 + k] * get_value_to_multiply();
      }
    }
}

void highlight_map(int x, int y, int range, uint8_t* pixmap, xpm_image_t img){
  for(int i = y - range/2; i < y + range/2; i++)
    for(int j = x - range/2; j < x + range/2; j++){
      if(i >= 0 && i < img.height && j >= 0 && j < img.width &&
         (i-y)*(i-y) + (j-x)*(j-x) <= range*range/4)
        for(int k = 0; k < 3; k++){
            pixmap[i*img.width*3 + j*3 + k] = highlighted_map[i*img.width*3 + j*3 + k];
        }
    }
}

