#include "background.h"
#include "devices/video.h"
#include "xpms.h"

int (drawBackground)(uint32_t *background){
  xpm_image_t img;
  background = (uint32_t *) xpm_load((xpm_map_t) Court_rec_xpm, XPM_8_8_8_8, &img);
  uint32_t *ptr = background;
  for(int i = 0; i < img.height; i++){
    for(int j = 0; j < img.width; j++){
      if(vg_draw_color(j, i, *ptr) != 0){
        return EXIT_FAILURE;
      };
      ptr++;
    }
  }
  return EXIT_SUCCESS;
}

int (drawDateBackground)( uint32_t *background, int day, int month, int year){
    Sprite *sprite = (Sprite*) malloc(sizeof(Sprite));
    sprite->map=(uint32_t *) malloc(sizeof(char*));
    int x=690;
    int y=800;
    while(year){
        choose_number_sprite(year%10,sprite);
        if(drawElementsInMenuBackground(background, sprite->map, x ,y, sprite->width, sprite->height)!=0){
            return EXIT_FAILURE;
        }
        year /=10;
        x -= 50;
    }

    x -= sprite->width;
    
    while(month){
        choose_number_sprite(month%10,sprite);
        if(drawElementsInMenuBackground(background, sprite->map, x ,y, sprite->width, sprite->height)!=0){
            return EXIT_FAILURE;
        }
        month /=10;
        x -= 50;
    }

    x -= sprite->width;
    
    while(day){
        choose_number_sprite(day%10,sprite);
        if(drawElementsInMenuBackground(background, sprite->map, x ,y, sprite->width, sprite->height)!=0){
            return EXIT_FAILURE;
        }
        day /=10;
        x -= 50;
    }

    free(sprite);

    return EXIT_SUCCESS;
}

int (drawElementsInMenuBackground)(uint32_t *background, uint32_t *element, int x, int y, int width, int height){

    if(element == NULL){
      return EXIT_FAILURE;
    }

    
    uint32_t *ptr2 = element;
    for(int i = y; i < y + height; i++){
      for(int j = x; j < x + width; j++){

          unsigned int index = (j + i * get_hres());
          

          if(index >= (get_vres() * get_hres())){
            printf("here");
          }
        if(*ptr2 != TRANSPARENCY_COLOR && i < MAX_X && j < MAX_Y){

          if(memcpy(&background[index], ptr2 , get_bytes_per_pixel()) == NULL){ 
            return EXIT_FAILURE;
          };
        }
        ptr2++;
      }

    }
  return EXIT_SUCCESS;
}

int (initializeMenuBackground)(uint32_t *background, bool isStart, Menu *menu, int day, int month, int year){

  uint32_t color = 0x000000;

  uint32_t *ptr = background;
  for(int i = 0; i < (int) get_vres(); i++){
    for(int j = 0; j < (int) get_hres(); j++){
      if(memcpy(ptr, &color, get_bytes_per_pixel()) == NULL){
        return EXIT_FAILURE;
      };
      ptr++;
    }
  }

  if(isStart){
    if(drawElementsInMenuBackground(background, menu->title.map, 260, 10, menu->title.width, menu->title.height) != 0){
      return EXIT_FAILURE;
    }
    if(drawDateBackground(background, day, month, year) != 0){
      return EXIT_FAILURE;
    }
    
  }
  else{
    if(drawElementsInMenuBackground(background, menu->pause_menu.map, 0, 0, menu->pause_menu.width, menu->pause_menu.height) != 0){
      return EXIT_FAILURE;
    }
    
  }

  return EXIT_SUCCESS;
}

int (refreshBackground)(uint32_t *background){
  if(memcpy(get_second_buffer(), background, get_hres() * get_vres() * get_bytes_per_pixel()) == NULL){
    return EXIT_FAILURE;
  };
  return EXIT_SUCCESS;
}
