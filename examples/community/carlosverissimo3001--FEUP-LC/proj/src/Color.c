#include <lcom/lcf.h>

#include "Color.h"


/* Keeps the contents of the current frame -> from pen_pal.c*/
extern xpm_image_t frame;

extern uint32_t background_color;
extern bool dark;

static Color colors[NO_COLORS];

void(load_colors)() {
  if (dark){
    colors[0].sp = create_sprite(color_white_xpm, 25, 640, 0, 0);
    colors[0].color = WHITE;
  }
  else{
    colors[0].sp = create_sprite(color_black_xpm, 25, 640, 0, 0);
    colors[0].color = BLACK;
  }

  colors[1].sp = create_sprite(color_red_xpm, 100, 640, 0, 0);
  colors[1].color = 0xFF7779;
  colors[2].sp = create_sprite(color_blue_xpm, 175, 640, 0, 0);
  colors[2].color = 0x89CFF0;
  colors[3].sp = create_sprite(color_green_xpm, 250, 640, 0, 0);
  colors[3].color = 0xa1FFa1;
  colors[4].sp = create_sprite(color_orange_xpm, 57, 716, 0, 0);
  colors[4].color = 0xFCBE85;
  colors[5].sp = create_sprite(color_violet_xpm, 132, 716, 0, 0);
  colors[5].color = 0xB891E3;
  colors[6].sp = create_sprite(color_grey_xpm, 206, 716, 0, 0);
  colors[6].color = 0x454545;
  /* colors[7].sp = create_sprite(color_grey_xpm, 226, 967, 0, 0);
  colors[7].color = background_color; */
}

void(draw_colors)(){
  for (int i = 0; i < NO_COLORS; i++) {
    draw_sprite(colors[i].sp, true);
 }
}

void(destroy_colors)(){
  for (int i = 0; i < NO_COLORS; i++) {
    destroy_sprite(colors[i].sp);
  }
}

uint32_t(get_color_rgb)(int index){
  if (index < 4)
    return 0;
  return colors[index - 4].color;
}
