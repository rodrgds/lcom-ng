#include "viewer.h" 
#include "../model/xpm/xpm.h"
#include "../drivers/video_card/gpu.h"
#include "../model/cursor/cursor.h"
#include "../model/game/game.h"

int (draw_sprite)(Sprite* sprite, uint16_t x, uint16_t y) {
  
  if (draw_xpm(sprite->img, sprite->addr, x, y))
    return 1;

  return 0;
}

int (render_screen)(){
  if(copy_buffer_to_video_mem())
    return 1;

  return 0;
}

int (draw_cursor)(){

  if (draw_sprite(mouse, cursor->x, cursor->y))
    return 1;

  return 0;
}

int (erase_cursor)(){

  if (draw_screenshot_to_buffer(cursor->old_x, cursor->old_y, mouse->img.width, mouse->img.height))
    return 1;

  cursor->old_x = cursor->x;
  cursor->old_y = cursor->y;
  
  return 0;
}



int (draw_word)(Sprite* sprite, uint16_t x, uint16_t y, char* word){

  if (draw_xpm_word(sprite->img, sprite->addr, x, y, word))
    return 1;

  return 0;
}


int (drawDate)(rtc_timestamp_t* timestamp) {
  char date[17];
  date[0] = timestamp->hours / 10 + '0';
  date[1] = timestamp->hours % 10 + '0';
  date[2] = ':';
  date[3] = timestamp->minutes / 10 + '0';
  date[4] = timestamp->minutes % 10 + '0';
  date[5] = ' ';
  date[6] = timestamp->day / 10 + '0';
  date[7] = timestamp->day % 10 + '0';
  date[8] = '/';
  date[9] = timestamp->month / 10 + '0';
  date[10] = timestamp->month % 10 + '0';
  date[11] = '/';
  date[12] = '2';
  date[13] = '0';
  date[14] = timestamp->year / 10 + '0';
  date[15] = timestamp->year % 10 + '0';
  date[16] = '\0';

  int x = 600;
  int y = 800;
  for (int i = 0; i < 16; i++){

    if(date[i] == ' '){
      x += numbers_small->img.width / 12;
      continue;
    }

    /*if(date[i] == '/'){
      draw_xpm(slash->img, slash->addr, x+15, y+10);
      x += numbers->img.width / 11;
      continue;
    }*/

    if (draw_number(numbers_small->img, numbers_small->addr, x, y, date[i])) return 1;
    
    x += numbers_small->img.width / 12;
    }

  return 0;
}


int erase_date(){
  if (draw_screenshot_to_buffer(600, 800, 16 * (numbers->img.width / 11), numbers->img.height))
    return 1;

  return 0;
}
