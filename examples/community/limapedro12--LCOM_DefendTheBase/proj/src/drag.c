#include "drag.h"

bool no_caminho[12][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                           {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
                           {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1},
                           {0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1},
                           {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1},
                           {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1},
                           {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
                           {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
                           {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                           {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}};

void verifyDrag(int * x, int * y, bool * new, bool * new_next, bool * placed, bool * placed_next, int * level, int * coins, uint8_t* background_pixmap, xpm_image_t background_img, tower towers_level_1[], tower towers_level_2[], tower towers_level_3[]){
  int tower_size = 30;
  
  if(placed && is_lb_pressed() && *x>=get_mouse_position().x - tower_size  && *x<=get_mouse_position().x+tower_size &&*y>=get_mouse_position().y-tower_size && *y<=get_mouse_position().y+tower_size){
    *x = get_mouse_position().x - 15;
    *y = get_mouse_position().y - 15;
  }
  if(is_key_pressed('p', false) && 
     *x>=get_mouse_position().x - tower_size && *x<=get_mouse_position().x+tower_size &&
     *y>=get_mouse_position().y-tower_size && *y<=get_mouse_position().y+tower_size &&
     no_caminho[(*y+15)/50][(*x+15)/50] == 0) {
    *x = ((*x + 15) / 50) * 50;
    *y = ((*y + 15) / 50) * 50;
    *placed = true;
    *placed_next = false;
    *new = false;
    *new_next = true;

    if(*level == 1) {*coins = *coins - 50;}
    else if(*level == 2) {*coins = *coins - 100;}
    else {*coins = *coins - 150;}

    highlight_map(*x + 25, *y +25, 120, background_pixmap, background_img);
    no_caminho[(*y+15)/50][(*x+15)/50] = 1;
  }
}

void verifyUpgrade(int * x, int * y, int * level, int * coins){
  int tower_size = 30;
  if(is_key_pressed('u', true) && *x>=get_mouse_position().x - tower_size  && *x<=get_mouse_position().x+tower_size &&*y>=get_mouse_position().y-tower_size && *y<=get_mouse_position().y+tower_size){
    
    if(*level == 1) {*level = 2;}
    else if(*level == 2) {*level = 3;}

    *coins = *coins -50;
  }
}
