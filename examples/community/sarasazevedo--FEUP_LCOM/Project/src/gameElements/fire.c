#include <lcom/lcf.h>
#include "fire.h"

extern PlayerShip* player;
extern Fire* fireArray[];
extern Enemy* enemyArray[];
extern EnemyExplosion* enemyExplosion[];
extern uint32_t fireCounter;
extern Cursor* cursor;


Fire* createFire(xpm_map_t sprite, uint16_t x, uint16_t y, uint16_t speed)  {
    Fire* fire =  (Fire*)malloc(sizeof(Fire));
  
    fire->x =x;
    fire->y= y;
    fire->v_x = 0;
    fire->v_y= speed;
    fire->curr_sprite = create_sprite(sprite,x,y,0,speed);
    fire->alive = false;
    return fire;
}

void drawFire(Fire* fire) {
    draw_sprite(fire->curr_sprite);
}

void createSpaceShipFire(PlayerShip* playerShip) {

    if(fireCounter == MAX_FIRE) return;

    fireArray[fireCounter]->alive=true;
    fireArray[fireCounter]->x = playerShip->curr_sprite->x + (playerShip->curr_sprite->width / 2) - (fireArray[fireCounter]->curr_sprite->width / 2);
    fireArray[fireCounter]->y = playerShip->curr_sprite->y - fireArray[fireCounter]->curr_sprite->height;
    fireArray[fireCounter]->curr_sprite->x = fireArray[fireCounter]->x;
    fireArray[fireCounter]->curr_sprite->y = fireArray[fireCounter]->y;

//     if(fireCounter == MAX_FIRE-1 ) {
//         fireCounter = 0;
//     }
//     else {
    fireCounter++;
//     }
 }

void moveFire() {
    for(int i = 0; i<MAX_FIRE; i++) {
        if(!fireArray[i]->alive) {
            continue;
        }
        fireArray[i]->x += fireArray[i]->v_x;
        fireArray[i]->y += fireArray[i]->v_y;
        move_sprite(fireArray[i]->curr_sprite);
        
        if(fireArray[i]->x < 0 || fireArray[i]->x > get_hres() || fireArray[i]->y < 0 || fireArray[i]->y > get_vres()) {
            fireArray[i]->alive = false;
        }
        drawFire(fireArray[i]);
        
        for(int j = 0; j < MAX_ENEMY; j++) {
            if(enemyArray[j]->alive) {
            if(checkEnemyHit(enemyArray[j],fireArray[i])) {
                enemyExplosion[j]->alive = true;
                enemyExplosion[j]->curr_sprite_no = 0;
                for(uint32_t k = 0; k < enemyExplosion[j]->sprite_quant; k++) {
                    enemyExplosion[j]->sprites[k]->x =enemyArray[j]->curr_sprite->x;
                    enemyExplosion[j]->sprites[k]->y =enemyArray[j]->curr_sprite->y;      
                }
            }
        }
    }
}
}

int checkEnemyHit(Enemy* enemy, Fire* fire ) {
    

    //Player boundaries
    uint16_t enemy_left_pos = enemy->curr_sprite->x;
    uint16_t enemy_right_pos = enemy_left_pos + enemy->curr_sprite->width;
    uint16_t enemy_top_pos = enemy->curr_sprite->y;
    uint16_t enemy_bottom_pos = enemy_top_pos + enemy->curr_sprite->height;

    //Fire boundaries
    uint16_t fire_left_pos = fire->curr_sprite->x;
    uint16_t fire_right_pos = fire_left_pos + fire->curr_sprite->width;
    uint16_t fire_top_pos = fire->curr_sprite->y;
    uint16_t fire_bottom_pos = fire_top_pos + fire->curr_sprite->height;

    // no collision condition
    if(enemy_right_pos < fire_left_pos || enemy_left_pos > fire_right_pos || enemy_bottom_pos < fire_top_pos || enemy_top_pos > fire_bottom_pos ) {
        return 0;
    }

    // collision check
    for(uint16_t y = enemy_top_pos; y < enemy_bottom_pos; y++){
        for(uint16_t x = enemy_left_pos; x < enemy_right_pos; x++) {
        
            uint16_t fire_x = x - fire_left_pos;
            uint16_t fire_y = y - fire_top_pos;
            uint16_t enemy_x = x - enemy_left_pos;
            uint16_t enemy_y = y - enemy_top_pos;

            //checks if coordinates are in the boundaries of the sprites limits
            if (fire_x >= 0 && fire_x < fire->curr_sprite->width && fire_y >= 0 && fire_y < fire->curr_sprite->height &&
                enemy_x >= 0 && enemy_x < enemy->curr_sprite->width && enemy_y >= 0 && enemy_y < enemy->curr_sprite->height) {
                
                //pixel position
                uint32_t fire_pixel = enemy->curr_sprite->map[fire_y * fire->curr_sprite->width + fire_x];
                uint32_t player_pixel = enemy->curr_sprite->map[enemy_y * enemy->curr_sprite->width + enemy_x];

                // collision detected, kills fire and decreases player lives
                if ((fire_pixel != TRANSPARENCY_COLOR_8_8_8_8) && (player_pixel != TRANSPARENCY_COLOR_8_8_8_8)) {
                    enemy->alive = false; 
                    fire->alive = false; 
                    updatePlayerScore(player);
                    return 1;
                }
            }
        }
    }
    return 0;
}

void destroyFire(Fire* fire) {
    if(fire == NULL) {
        return;
    }

    destroy_sprite(fire->curr_sprite);
    free((fire));

}



