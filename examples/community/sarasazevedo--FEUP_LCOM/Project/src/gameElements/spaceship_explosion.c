#include <lcom/lcf.h>
#include "spaceship_explosion.h"

extern int timerIntCounter;



Spaceship_Explosion* createExplosion(xpm_map_t sprite1, xpm_map_t sprite2, xpm_map_t sprite3, xpm_map_t sprite4, uint16_t x, uint16_t y) {

  Spaceship_Explosion* explosion =  (Spaceship_Explosion*)malloc(sizeof(Spaceship_Explosion));
  explosion ->sprites = malloc(4 * sizeof(Sprite *));
  explosion ->x =x;
  explosion ->y=  y;
  explosion ->sprites[0] = create_sprite(sprite1,explosion->x,explosion ->y,0,0);
  explosion ->sprites[1] = create_sprite(sprite2,explosion->x,explosion ->y,0,0);
  explosion ->sprites[2] = create_sprite(sprite3,explosion->x,explosion ->y,0,0);
  explosion ->sprites[3] = create_sprite(sprite4,explosion->x,explosion ->y,0,0);
  explosion ->curr_sprite = explosion ->sprites[0];
  explosion ->sprite_quant = 4;
  explosion ->curr_sprite_no = 0;
  explosion->alive = false;
  return explosion;

}


void updateSpaceshipExplosionSprite(Spaceship_Explosion* explosion) {
  if(!explosion || explosion == NULL) return;

  //slows down the explosion rate
  if(timerIntCounter%5 == 0) explosion->curr_sprite_no++;
  
  // resets the explosion frame
  if(explosion->curr_sprite_no >= explosion->sprite_quant) {
    explosion->alive = false;
    explosion->curr_sprite_no=0;
  }
}

void drawCurrExplosionSprite(Spaceship_Explosion* explosion) {
  if(!explosion || explosion == NULL) return;
  draw_sprite(explosion->sprites[explosion->curr_sprite_no]);
}

void destroySpaceshipExplosion(Spaceship_Explosion* explosion) {
    if(explosion == NULL) {
        return;
    }

    for(uint32_t i = 0; i<explosion->sprite_quant;i++) {
        destroy_sprite(explosion->sprites[i]);
    }
    free(explosion->sprites);
    free(explosion);
}






