#include "name_viewer.h"
#include "viewer.h"
#include "../model/name/name_input.h"

int (name_refresh)() {  

  int x = nameInput->x;
  int y = nameInput->y;

  char* word = nameInput->name;

  erase_cursor();

  draw_screenshot_to_buffer(x, y, nameInput->limit * 76, 126);

  draw_word(alphabet, x, y, word);

  draw_cursor();

  if (render_screen()) return 1;

  return 0;
}

int (name_screenshot)(){

  if (clean_screen()) 
    return 1;

 if(draw_sprite(chessBackground, 0, 0))  return 1;

  if(draw_word(alphabet, 550, 100, "PLAYER")) return 1;
  if(draw_word(alphabet, 630, 200, "NAME")) return 1;

  if (copy_buffer_to_screenshot())
    return 1;
  
  return 0;
}
