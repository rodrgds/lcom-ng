#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "sprite.h"

/** @defgroup sprite sprite
 * @{
 *
 * Functions for the sprites
 */

extern bool door1_open, door2_open;
extern Sprite *door1, *door2, *button1, *button1_down, *button2, *button2_down, *player2;

/**
 * @brief Loads XPM image data to a sprite.
 * @param xpm The XPM.
 * @param sp Pointer to the Sprite that will be initialized.
 * @return 0 on success and non-zero on error.
 */
int (loading_xpm)(xpm_map_t xpm, Sprite *sp) {
    xpm_image_t image;
    sp->map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);
    sp->height = image.height;
    sp->width = image.width; 
  
    return 0; 
}

/**
 * @brief Creates a sprite and initializes its position and speed.
 * @param xpm The XPM.
 * @param x The initial coordinate x.
 * @param y The initial coordinate y.
 * @param speed The speed.
 * @return Pointer to the created Sprite and NULL if it failed.
 */
Sprite* (create_sprite)(xpm_map_t xpm, int x, int y, int speed) {
  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
  if (sp == NULL )
    return NULL;

  if( ((char *)xpm) == NULL ) {
    printf("XPM is NULL \n");
    free(sp);
    return NULL;
  }

  loading_xpm(xpm, sp);
  sp->x = x;
  sp->y = y;
  sp->speed = speed;
  return sp;
}


/**
 * @brief Frees the memory allocated for a sprite and its map.
 * @param sp Pointer to the Sprite that will be destroyed.
 */
void destroy_sprite(Sprite *sp) {
  if ( sp == NULL )
    return;

  free(sp->map);
  free(sp);
}


/**
 * @brief Draws a sprite to a buffer skipping transparent pixels.
 * @param sp Pointer to the Sprite that will be drawn.
 * @param buffer The buffer to draw the sprite.
 * @return 0 if success and 1 if error.
 */
int (drawing_to_buffer)(Sprite *sp, uint8_t *buffer) {
  uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

  for (int y = 0 ; y < sp->height ; y++) {
      for (int x = 0 ; x < sp->width  ; x++) {
          uint32_t current_color = sp->map [y * sp->width + x];
          
          if (current_color != transparent_color) {
              if (draw_pixel(sp->x + x, sp->y + y, current_color, buffer)) {
                  printf("Drawing pixel failed \n");
                  return 1;
              }
          }
      }
  }
  
  return 0;
}


/**
 * @brief Draws a sprite using the global draw buffer.
 * @param sp Pointer to the Sprite that will be drawn.
 * @return 0 if success and 1 if error.
 */
int (drawing_sprite)(Sprite *sp) {
  if (drawing_to_buffer(sp, draw_buffer) != 0) {
    printf("Drawing sprite failed \n");
    return 1;
  }

  return 0;
}


/**
 * @brief Draws a lantern around a sprite to a buffer.
 * @param bg Pointer to the background Sprite.
 * @param sp Pointer to the lantern Sprite.
 * @param buffer The buffer to draw to.
 * @param lant_radius The radius of the lantern.
 * @return 0 if success and 1 if error.
 */
int (drawing_to_buffer_lantern)(Sprite *bg, Sprite *sp, uint8_t *buffer, int lant_radius) {
  uint32_t transparent_color = xpm_transparency_color(XPM_8_8_8_8); 

    int center_x = sp->x + sp->width / 2;
    int center_y = sp->y + sp->height / 2;


    for (int y = -lant_radius; y <= lant_radius; y++) {
        for (int x = -lant_radius; x <= lant_radius; x++) {
            int pixel_x = center_x + x;
            int pixel_y = center_y + y;

            if (x * x + y * y <= lant_radius * lant_radius) {
                if (pixel_x >= 0 && pixel_x < bg->width && pixel_y >= 0 && pixel_y < bg->height) {
                    uint32_t current_color = bg->map[pixel_y * bg->width + pixel_x];
                    
                    if (current_color != transparent_color) {
                        if (draw_pixel(pixel_x, pixel_y, current_color, buffer)) {
                            printf("Drawing pixel failed \n");
                            return 1;
                        }
                    }
                }
            }
        }
    }
    if(!door1_open){
      if (is_sprite_inside_radius(sp, door1, lant_radius)) {
          drawing_sprite(door1);
      }
    }
    if(!door2_open){
      if (is_sprite_inside_radius(sp, door2, lant_radius)) {
          drawing_sprite(door2);
      }
    }
    if (is_sprite_inside_radius(sp, button1, lant_radius)) {
      if (!door1_open) drawing_sprite(button1);
      else drawing_sprite(button1_down);
    }
    if (is_sprite_inside_radius(sp, button2, lant_radius)) {
      if (!door2_open) drawing_sprite(button2);
      else drawing_sprite(button2_down);
    }    
  
    return 0;
}


/**
 * @brief Sees if a sprite ('sp') is in a certain radius of the center of another sprite ('center').
 * @param center Pointer to the center Sprite.
 * @param sp Pointer to the Sprite to check.
 * @param lant_radius The radius to check.
 * @return true if the sprite is in the radius and false if it isn't.
 */
bool is_sprite_inside_radius(Sprite *center, Sprite *sp, int lant_radius) {
  int center_x = center->x + center->width / 2;
  int center_y = center->y + center->height / 2;

  if((sp->x >= center_x - lant_radius) && (sp->x <= center_x + lant_radius) &&
     (sp->y >= center_y - lant_radius) && (sp->y <= center_y + lant_radius)) {return true;}
    return false;
}


/**
 * @brief Draws a lantern in the background.
 * @param bg Pointer to the background Sprite.
 * @param sp Pointer to the Sprite that the lantern will be drawn to.
 * @param lant_radius The radius of the lantern.
 * @return 0 if success and 1 if error.
 */
int (drawing_lantern)(Sprite *bg, Sprite *sp, int lant_radius) {
  if (drawing_to_buffer_lantern(bg, sp, draw_buffer, lant_radius) != 0) {
    printf("Drawing background failed \n");
    return 1;
  }

  return 0;
}

/**
 * @brief Draws the background with a global draw buffer.
 * @return 0 if success and 1 if error.
 */
int (drawing_bg)() {
  if (draw_background() != 0) {
    printf("Drawing background failed \n");
    return 1;
  }

  return 0;
}

/**
 * @brief Loads the background sprite to a buffer.
 * @param sp Pointer to the Sprite representing the background.
 * @return 0 if success and 1 if error.
 */
int (loading_bg_sprite)(Sprite *sp) {
  if (drawing_to_buffer(sp, bg_buffer) != 0) {
    printf("Loading background failed \n");
    return 1;
  }

  return 0;
}


/**
 * @brief Sees if there is a collision between the player sprite and the maze sprite.
 * @param player Pointer to the player Sprite.
 * @param maze Pointer to the maze Sprite.
 * @param x_diff The coordinate x that the player will next move to.
 * @param y_diff The coordinate y that the player will next move to.
 * @return true if there is a collision and false if there isn't.
 */
bool (check_collision)(Sprite *player, Sprite *maze, int x_diff, int y_diff) {
  int player_h, player_w;
  if(player == player2){
    player_h = PLAYER2_HEIGHT;
    player_w = PLAYER2_WIDTH;
    }
  else{
    player_h = PLAYER1_HEIGHT;
    player_w = PLAYER1_WIDTH;
    }
  int x_top = player->x;
  int y_top = player->y + player_h/2;
  uint32_t color = maze->map[30 * maze->width + 5];

  uint32_t next_color1 = color, next_color2 = color;

  if (x_diff > 0) {   // Right
    next_color1 = maze->map[(y_top) * maze->width + (x_top + player_w + x_diff)];
    next_color2 = maze->map[(y_top + player_h/2) * maze->width + (x_top + player_w + x_diff)];

  } else if (x_diff < 0) {    // Left
    next_color1 = maze->map[(y_top) * maze->width + (x_top + x_diff)];
    next_color2 = maze->map[(y_top + player_h/2) * maze->width + (x_top + x_diff)];
  
  } else if (y_diff > 0) {    // Bottom
    next_color1 = maze->map[(y_top + player_h/2 + y_diff) * maze->width + (x_top)];
    next_color2 = maze->map[(y_top + player_h/2 + y_diff) * maze->width + (x_top + player_w)];

  } else if (y_diff < 0) {    // Top
    next_color1 = maze->map[(y_top + y_diff) * maze->width + (x_top)];
    next_color2 = maze->map[(y_top + y_diff) * maze->width + (x_top + player_w)];

  }
    if(!door1_open){
    if((x_top + x_diff >= (door1->x - (door1->width/2)) && x_top + x_diff <= (door1->x + door1->width))
    && (y_top + y_diff >= (door1->y - (door1->height/2)) && y_top + y_diff <= (door1->y + door1->height)) ) return true;
  }
    if(!door2_open){
    if((x_top + x_diff >= (door2->x - (door2->width/2)) && x_top + x_diff <= (door2->x + door2->width))
    && (y_top + y_diff >= (door2->y - (door2->height/2)) && y_top + y_diff <= (door2->y + door2->height)) ) return true;
  }
  if ((color != next_color1) || (color != next_color2)) 
    return true;
  return false;
}


