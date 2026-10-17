#include <lcom/lcf.h>
#include "enemy.h"
#include "gameElements/fire.h"
 
#define ENEMY_WIDTH 60
#define ENEMY_HEIGHT 64

extern Enemy* enemyArray[MAX_ENEMY];
extern int  timerIntCounter;
extern EnemyExplosion* enemyExplosion[];
extern Fire* fireArray[];


Enemy* createEnemy(xpm_map_t sprite1, xpm_map_t sprite2, uint16_t x, uint16_t y) {
  Enemy* enemy =  (Enemy*)malloc(sizeof(Enemy));
  enemy->sprites = malloc(2 * sizeof(Sprite *));
  enemy->x =x;
  enemy->y=  y;
  enemy->sprites[0] = create_sprite(sprite1,enemy->x,enemy->y,ENEMY_SPEED,0);
  enemy->sprites[1] = create_sprite(sprite2,enemy->x,enemy->y,ENEMY_SPEED,0);
  enemy->curr_sprite = enemy->sprites[0];
  enemy->sprite_quant = 2;
  enemy->curr_sprite_no = 0;
  enemy->alive = false;
  enemy->direction = RIGHT;
  return enemy;
}

void drawCurrEnemySprite(Enemy* enemy) {
  draw_sprite(enemy->curr_sprite);
}

void destroyEnemy(Enemy* enemy) {
    if(enemy == NULL) {
      return;
    }

    for(uint32_t i = 0; i < enemy->sprite_quant; i++) {
      destroy_sprite(enemy->sprites[i]);
    }

    free(enemy->sprites);
    free(enemy);
}

void setEnemiesInitialPos() {
    int jump = 100;
    int i=0, x=100, y=150;
    for (int r = 0; r < 4; r++) {
        for (int c=0; c<8; c=c+4) {
        x=x+jump;
        enemyArray[i]->x = x;
        enemyArray[i]->y = y;
        x=x+jump;
        enemyArray[i+1]->x = x;
        enemyArray[i+1]->y = y;
        x=x+jump;
        enemyArray[i+2]->x = x;
        enemyArray[i+2]->y = y;
        x=x+jump;
        enemyArray[i+3]->x = x;
        enemyArray[i+3]->y = y;
        i =i+4;
        }
        x=100;
        y=y+jump;
    }
}

void updateEnemiesAndExplosions() {
  for(int i = 0; i < MAX_ENEMY; i++) {

    //Updates the frame of the enemy every half second
    if(timerIntCounter % 30 == 0 && enemyArray[i]->alive== true) { 
    enemyArray[i]->curr_sprite_no = (enemyArray[i]->curr_sprite_no+3)%2;
    enemyArray[i]->curr_sprite = enemyArray[i]->sprites[enemyArray[i]->curr_sprite_no];
    }
  
    //Shifts the direction to the left of the enemy after reaching maximum movement displacement
    if(((enemyArray[i]->curr_sprite->x-enemyArray[i]->x) >= MAX_MOVEMENT) && enemyArray[i]->direction == RIGHT ) {
      enemyArray[i]->sprites[0]->xspeed=-ENEMY_SPEED;
      enemyArray[i]->sprites[1]->xspeed=-ENEMY_SPEED;
      enemyArray[i]->direction = LEFT;
    }

    //Shifts the direction to the right of the enemy after reaching maximum movement displacement
    if(((enemyArray[i]->x-enemyArray[i]->curr_sprite->x) >= MAX_MOVEMENT) && enemyArray[i]->direction == LEFT) {
      enemyArray[i]->sprites[0]->xspeed=ENEMY_SPEED;
      enemyArray[i]->sprites[1]->xspeed=ENEMY_SPEED;
      enemyArray[i]->direction = RIGHT;
    }

    //updates enemy position
    move_sprite(enemyArray[i]->sprites[0]);
    move_sprite(enemyArray[i]->sprites[1]);      

    //updates enemyExplosion if there's one ocurring 
    if(enemyExplosion[i]->alive) {
      updateEnemyExplosionSprite(enemyExplosion[i]);
      drawEnemyExplosion(enemyExplosion[i]);
    }
  }
}

