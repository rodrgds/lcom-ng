#include <lcom/lcf.h>
#include "sprites.h"
#include "../game/game_utils.h"

sprite_t *create_sprite(xpm_map_t xpms[],position_t pos,int n,sprite_id_e id) {

  // allocate mem for the sprite_t struct
  sprite_t *spr = (sprite_t *) malloc(sizeof(sprite_t));
  if (spr == NULL)
    return NULL;

  // allocate mem for the frames array field in the sprite_t struct
  spr->frames = (pixmap_t *) malloc(sizeof(pixmap_t) * n);
  if (spr->frames == NULL) {
    free(spr);
    return NULL;
  }

  // assigning values for identifier, position, n_frames and i_frame;
  spr->position=pos;
  spr->identifier=id;
  spr->n_frames=n;
  spr->i_frame=0;

  for(int i=0; i<n;i++){

    xpm_image_t img;
    memset(&img,0,sizeof(img));

    uint8_t *xpm_colors = xpm_load(xpms[i], XPM_8_8_8, &img);
    if (xpm_colors == NULL) return NULL;

    spr->frames[i].color_array = malloc(img.width * img.height * sizeof(uint32_t));
    if(spr->frames[i].color_array==NULL){
      for(int f=0; f<i;f++){
        free(spr->frames[f].color_array);
      }
      free(spr->frames);
      free(spr);
      return NULL;
    }

    for (int y = 0; y < img.height; y++) {
      for (int x = 0; x < img.width; x++) {
          int index = y * img.width + x;
          uint8_t blue = xpm_colors[index * 3];
          uint8_t green = xpm_colors[index * 3 + 1];
          uint8_t red = xpm_colors[index * 3 + 2];
          uint32_t color = 0 | (red << 16) | (green << 8) | blue;

          spr->frames[i].color_array[y * img.width + x] = color;
          //printf("Color at (%d, %d): 0x%08X\n", y, x, color);
      }
    }
    
    spr->frames[i].width=img.width;
    spr->frames[i].height=img.height;
    
  }
 
  return spr;
}

not_pseudo_sprite *(create_not_pseudo_sprite)(xpm_map_t sprite){
  not_pseudo_sprite *nps = (not_pseudo_sprite *) malloc(sizeof(sprite));
  if (nps == NULL) return NULL;

  xpm_image_t img;
  nps->colors = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);
  nps->height = img.height;
  nps->width = img.width;

  if( nps->colors == NULL ) {
    free(nps);
    return NULL;
  }

  return nps;
}

void delete_sprite(sprite_t *spr){
  if(spr->frames != NULL){
    for(int i=0;i<spr->n_frames;i++){
      free(spr->frames[i].color_array);
    }
    free(spr->frames);
  }
  free(spr);
  spr=NULL;
  return;
}

void delete_not_pseudo_sprite(not_pseudo_sprite *sprite){
  if(sprite == NULL) {
      return;
    }
    else if(sprite->colors){
        free(sprite->colors);
    }
    
    free(sprite);
    sprite = NULL;
}

int get_xpm_width_from_char(char letter) {
  xpm_map_t xpm = get_xpm_from_char(letter);
  if (xpm == NULL) {
    return -1;
  }

  xpm_image_t img;
  memset(&img, 0, sizeof(img));
  uint32_t *frame = (uint32_t *)xpm_load(xpm, XPM_8_8_8, &img);
  if (frame == NULL) {
    return -1;
  }

  int width = img.width;
  free(frame);
  return width;
}

xpm_map_t get_xpm_from_char(char letter) {
  switch (letter) {
    case ' ':
      return (xpm_map_t) xpm_space;
    case 'a':
    case 'A':
      return (xpm_map_t) xpm_letter_a;
    case 'b':
    case 'B':
      return (xpm_map_t) xpm_letter_b;
    case 'c':
    case 'C':
      return (xpm_map_t) xpm_letter_c;
    case 'd':
    case 'D':
      return (xpm_map_t) xpm_letter_d;
    case 'e':
    case 'E':
      return (xpm_map_t) xpm_letter_e;
    case 'f':
    case 'F':
      return (xpm_map_t) xpm_letter_f;
    case 'g':
    case 'G':
      return (xpm_map_t) xpm_letter_g;
    case 'h':
    case 'H':
      return (xpm_map_t) xpm_letter_h;
    case 'i':
    case 'I':
      return (xpm_map_t) xpm_letter_i;
    case 'j':
    case 'J':
      return (xpm_map_t) xpm_letter_j;
    case 'k':
    case 'K':
      return (xpm_map_t) xpm_letter_k;
    case 'l':
    case 'L':
      return (xpm_map_t) xpm_letter_l;
    case 'm':
    case 'M':
      return (xpm_map_t) xpm_letter_m;
    case 'n':
    case 'N':
      return (xpm_map_t) xpm_letter_n;
    case 'o':
    case 'O':
      return (xpm_map_t) xpm_letter_o;
    case 'p':
    case 'P':
      return (xpm_map_t) xpm_letter_p;
    case 'q':
    case 'Q':
      return (xpm_map_t) xpm_letter_q;
    case 'r':
    case 'R':
      return (xpm_map_t) xpm_letter_r;
    case 's':
    case 'S':
      return (xpm_map_t) xpm_letter_s;
    case 't':
    case 'T':
      return (xpm_map_t) xpm_letter_t;
    case 'u':
    case 'U':
      return (xpm_map_t) xpm_letter_u;
    case 'v':
    case 'V':
      return (xpm_map_t) xpm_letter_v;
    case 'w':
    case 'W':
      return (xpm_map_t) xpm_letter_w;
    case 'x':
    case 'X':
      return (xpm_map_t) xpm_letter_x;
    case 'y':
    case 'Y':
      return (xpm_map_t) xpm_letter_y;
    case 'z':
    case 'Z':
      return (xpm_map_t) xpm_letter_z;
    case '0':
      return (xpm_map_t) xpm_num_zero;
    case '1':
      return (xpm_map_t) xpm_num_one;
    case '2':
      return (xpm_map_t) xpm_num_two;
    case '3':
      return (xpm_map_t) xpm_num_three;
    case '4':
      return (xpm_map_t) xpm_num_four;
    case '5':
      return (xpm_map_t) xpm_num_five;
    case '6':
      return (xpm_map_t) xpm_num_six;
    case '7':
      return (xpm_map_t) xpm_num_seven;
    case '8':
      return (xpm_map_t) xpm_num_eight;
    case '9':
      return (xpm_map_t) xpm_num_nine;
    default:
      return NULL;
  }
}
