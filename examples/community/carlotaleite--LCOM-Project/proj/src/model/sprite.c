
#include <lcom/lcf.h>
#include "controller/gpu.h"
#include <stdbool.h>
#include <stdio.h>
#include "sprite.h"



//Create sprites and define them 
Sprite *game_over_sprite;
Sprite *apple_sprite;
Sprite *mini_apple_sprite;


/** Creates a new sprite from XPM "pic", with specified
* position (within the screen limits) and speed;
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
Sprite *create_sprite(xpm_map_t pic) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;
    if( sp == NULL )
        return NULL;
    // read the sprite pixmap
    sp->map = (uint32_t *) xpm_load(pic, XPM_8_8_8_8, &img);
    if( sp->map == NULL ) {
    free(sp);
    return NULL;
    }
    sp->width = img.width; 
    sp->height=img.height;
    return sp;
}


void destroy_sprite(Sprite *sp) {
if( sp == NULL )
return;
if( sp ->map )
free(sp->map);
free(sp);
sp = NULL;
}

/* Some useful non-visible functions */
int draw_sprite(Sprite *sp, int x, int y) {
    uint16_t height = sp->height;
    uint16_t width = sp->width;
    sp->x = x;
    sp->y = y;
    uint32_t current_color;
    for (int h = 0; h < height; h++) {
        for( int w = 0; w < width; w++) {
            current_color = sp->map[w + h*width];
            if(current_color == 0xFFFF58) continue;
            if (paint_pixel( x + w, y + h, current_color)) return 1;
            //FFFF58 is a random color that i will attribute to the pixels i want to ignore
        }
    }
    return 0;
}




void create_sprites(){
if (!game_over_sprite) game_over_sprite = create_sprite((xpm_map_t)game_over_xpm);
if (!apple_sprite) apple_sprite = create_sprite((xpm_map_t)apple_xpm);
if (!mini_apple_sprite) mini_apple_sprite = create_sprite((xpm_map_t)mini_apple_xpm);
}


//to destroy them and free memory
void destroy_sprites(){
    destroy_sprite(game_over_sprite);
    destroy_sprite(apple_sprite);
    destroy_sprite(mini_apple_sprite);
    game_over_sprite = NULL;
    apple_sprite = NULL;
    mini_apple_sprite = NULL;
}


void render_game_over(int x, int y){
  draw_sprite(game_over_sprite, x, y);
}

void render_apple(int x, int y){
    draw_sprite(apple_sprite, x, y);
}
