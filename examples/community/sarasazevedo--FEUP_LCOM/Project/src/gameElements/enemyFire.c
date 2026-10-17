#include <lcom/lcf.h>
#include "enemyFire.h"

extern EnemyFire* enemyFireArray[MAX_ENEMY_FIRE];
extern uint32_t enemyFireCounter;
extern PlayerShip* player;
extern Spaceship_Explosion* explosionPlayer;


EnemyFire* createEnemyFire(xpm_map_t sprite, uint16_t x, uint16_t y, uint16_t speed)  {
    EnemyFire* enemyFire =  (EnemyFire*)malloc(sizeof(EnemyFire));
  
    enemyFire->x =x;
    enemyFire->y= y;
    enemyFire->v_x = 0;
    enemyFire->v_y= speed;
    enemyFire->curr_sprite = create_sprite(sprite,x,y,0,speed);
    enemyFire->alive = false;
    return enemyFire;
}

void drawEnemyFire(EnemyFire* enemyFire) {
    draw_sprite(enemyFire->curr_sprite);
}

void createEnemiesFire(Enemy* enemy) {
    enemyFireArray[enemyFireCounter]->alive=true;

    // centers the shot position with the enemy center
    enemyFireArray[enemyFireCounter]->x = enemy->curr_sprite->x + (enemy->curr_sprite->width / 2) - (enemyFireArray[enemyFireCounter]->curr_sprite->width / 2);
    enemyFireArray[enemyFireCounter]->y = enemy->curr_sprite->y+enemy->curr_sprite->height - enemyFireArray[enemyFireCounter]->curr_sprite->height;
    enemyFireArray[enemyFireCounter]->curr_sprite->x = enemyFireArray[enemyFireCounter]->x;
    enemyFireArray[enemyFireCounter]->curr_sprite->y = enemyFireArray[enemyFireCounter]->y;

    //resets the the enemy fire counter 
    if(enemyFireCounter == MAX_ENEMY_FIRE-1 ) {
        enemyFireCounter = 0;
    }
    else {
        enemyFireCounter++;
    }
}

void moveEnemyFire() {
    for(int i = 0; i<MAX_ENEMY_FIRE; i++) {

        //if shot not alive, ignores it
        if(!enemyFireArray[i]->alive) {
            continue;
        }

        //updates position in the enemy fire struct
        enemyFireArray[i]->x += enemyFireArray[i]->v_x;
        enemyFireArray[i]->y += enemyFireArray[i]->v_y;

        //updates position in the sprite
        move_sprite(enemyFireArray[i]->curr_sprite);
        
        //kills the enemy fire shot when it goes out the screen
        if(enemyFireArray[i]->x < 0 || enemyFireArray[i]->x > get_hres() || enemyFireArray[i]->y < 0 || enemyFireArray[i]->y > get_vres()) {
            enemyFireArray[i]->alive = false;
        }
        drawEnemyFire(enemyFireArray[i]);
        checkSpaceShipHit(player, enemyFireArray[i]);
    }
}

void checkSpaceShipHit(PlayerShip* playerShip, EnemyFire* enemyFire ) {
    

    //Player boundaries
    uint16_t player_left_pos = playerShip->curr_sprite->x;
    uint16_t player_right_pos = player_left_pos + playerShip->curr_sprite->width;
    uint16_t player_top_pos = playerShip->curr_sprite->y;
    uint16_t player_bottom_pos = player_top_pos + playerShip->curr_sprite->height;

    //Fire boundaries
    uint16_t fire_left_pos = enemyFire->curr_sprite->x;
    uint16_t fire_right_pos = fire_left_pos + enemyFire->curr_sprite->width;
    uint16_t fire_top_pos = enemyFire->curr_sprite->y;
    uint16_t fire_bottom_pos = fire_top_pos + enemyFire->curr_sprite->height;

    // no collision condition
    if(player_right_pos < fire_left_pos || player_left_pos > fire_right_pos || player_bottom_pos < fire_top_pos || player_top_pos > fire_bottom_pos ) {
        return;
    }

    // collision check
    for(uint16_t y = player_top_pos; y < player_bottom_pos; y++){
        for(uint16_t x = player_left_pos; x < player_right_pos; x++) {
        
            uint16_t fire_x = x - fire_left_pos;
            uint16_t fire_y = y - fire_top_pos;
            uint16_t player_x = x - player_left_pos;
            uint16_t player_y = y - player_top_pos;

            //checks if coordinates are in the boundaries of the sprites limits
            if (fire_x >= 0 && fire_x < enemyFire->curr_sprite->width && fire_y >= 0 && fire_y < enemyFire->curr_sprite->height &&
                player_x >= 0 && player_x < playerShip->curr_sprite->width && player_y >= 0 && player_y < playerShip->curr_sprite->height) {
                
                //pixel position
                uint32_t fire_pixel = playerShip->curr_sprite->map[fire_y * enemyFire->curr_sprite->width + fire_x];
                uint32_t player_pixel = playerShip->curr_sprite->map[player_y * playerShip->curr_sprite->width + player_x];

                // collision detected, kills fire and decreases player lives
                if ((fire_pixel != TRANSPARENCY_COLOR_8_8_8_8) && (player_pixel != TRANSPARENCY_COLOR_8_8_8_8) && (!explosionPlayer->alive)) {
                    playerShip->currLives--;
                    playerShip->curr_sprite->xspeed = 0;
                    playerShip->curr_sprite->yspeed = 0;
                    if(playerShip->currLives==0)  playerShip->alive = false; 
                    enemyFire->alive = false; 
                    makeSpaceShipExplosion(playerShip);
                    return;
                }
            }
        }
    }
}   


void destroyEnemyFire(EnemyFire* enemyFire) {
    if(enemyFire == NULL) {
        return;
    }

    destroy_sprite(enemyFire->curr_sprite);
    free((enemyFire));

}



