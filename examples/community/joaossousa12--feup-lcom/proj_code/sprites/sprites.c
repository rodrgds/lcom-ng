#include "sprites.h"
#include "stdlib.h"

extern int gameMode;
extern int x;
extern int y;

void createSpriteArr(){
    char **fileNames[MAX_SPRITES] = { InitialMenu_xpm, instructionsFIrst_xpm, border_xpm, middle_xpm, obstacle_xpm, hole_xpm, ball_xpm, mouse_xpm};
    
    for(int i=0; i < MAX_SPRITES; i++){
        Sprite *s = (Sprite *) malloc (sizeof(Sprite));
        if(s == NULL) continue;

        xpm_image_t image;
        s->pixels = (uint32_t *) xpm_load((xpm_map_t) fileNames[i], XPM_8_8_8_8, &image);
        s->height = image.height;
        s->width = image.width;

        if(s->pixels == NULL){
            free(s);
            continue;
        }

        sprites[i] = s;
    }
}

void cleanSpritesArr(){
    for(int i = 0; i < MAX_SPRITES; i++){
        if(sprites[i] == NULL) continue;
        else if(sprites[i]->pixels) free(sprites[i]->pixels);

        free(sprites[i]);

        sprites[i] = NULL;
    }
}


int drawSprite(Sprite *sprite, int x, int y){
    uint16_t height = sprite->height, width = sprite->width;

    for (int h = 0 ; h < height ; h++) {
        for (int w = 0 ; w < width ; w++) {
            if (draw_pixel(x + w, y + h, sprite->pixels[w + h*width])) return 1;
        }
    }

    return 0; 
}
