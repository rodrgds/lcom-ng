#include <lcom/lcf.h>
#include "enemyExplosion.h"

extern int timerIntCounter;



EnemyExplosion* createEnemyExplosion(xpm_map_t sprite1, xpm_map_t sprite2, xpm_map_t sprite3, xpm_map_t sprite4, xpm_map_t sprite5, uint16_t x, uint16_t y) {

  EnemyExplosion* explosion =  (EnemyExplosion*)malloc(sizeof(EnemyExplosion));
  explosion ->sprites = malloc(5 * sizeof(Sprite *));
  explosion ->x =x;
  explosion ->y=  y;
  explosion ->sprites[0] = create_sprite(sprite1,explosion->x,explosion ->y,0,0);
  explosion ->sprites[1] = create_sprite(sprite2,explosion->x,explosion ->y,0,0);
  explosion ->sprites[2] = create_sprite(sprite3,explosion->x,explosion ->y,0,0);
  explosion ->sprites[3] = create_sprite(sprite4,explosion->x,explosion ->y,0,0);
  explosion ->sprites[4] = create_sprite(sprite5,explosion->x,explosion ->y,0,0);
  explosion ->curr_sprite = explosion ->sprites[0];
  explosion ->sprite_quant = 5;
  explosion ->curr_sprite_no = 0;
  explosion->alive = false;
  return explosion;

}
void updateEnemyExplosionSprite(EnemyExplosion* explosion) {

    if(!explosion || explosion == NULL) return;

    explosion->curr_sprite_no++;
  
    // resets the sprite number after reaching the final explosion frame
    if(explosion->curr_sprite_no >= explosion->sprite_quant) {
        explosion->alive = false;
        explosion->curr_sprite_no=0;
    } 
}

void drawEnemyExplosion(EnemyExplosion* explosion) {
    if(!explosion || explosion == NULL) return;
    draw_sprite(explosion->sprites[explosion->curr_sprite_no]);
}

void destroyEnemyExplosion(EnemyExplosion* explosion){
    if(explosion == NULL) {
        return;
    }

    for(uint32_t i = 0; i<explosion->sprite_quant;i++) {
        destroy_sprite(explosion->sprites[i]);
    }
    free(explosion->sprites);
    free(explosion);
}

