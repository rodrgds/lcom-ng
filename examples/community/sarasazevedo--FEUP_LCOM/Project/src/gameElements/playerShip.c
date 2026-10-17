#include <lcom/lcf.h>
#include "playerShip.h"

uint32_t fireCounter = 0;
extern Spaceship_Explosion* explosionPlayer;


PlayerShip* createPlayerShip(xpm_map_t sprite, uint16_t x, uint16_t y) {
  PlayerShip* playerShip =  (PlayerShip*)malloc(sizeof(PlayerShip));
  
  playerShip->x =x;
  playerShip->y= y;

  playerShip->currLives = 3;
  playerShip->currScore = 0;

  playerShip->curr_sprite = create_sprite(sprite,x,y,0,0);
  playerShip->alive = true;
  return playerShip;
}




void drawPlayerShip(PlayerShip* playerShip) {
    draw_sprite(playerShip->curr_sprite);
}

void makeSpaceShipExplosion(PlayerShip* playerShip) {
    explosionPlayer->alive = true;
    
    for(int i = 0; i < 4; i++) {
        explosionPlayer->sprites[i]->x = playerShip->curr_sprite->x;
        explosionPlayer->sprites[i]->y = playerShip->curr_sprite->y;
    }
}

void updateSpaceshipExplosions() {
    if(explosionPlayer->alive) {
        updateSpaceshipExplosionSprite(explosionPlayer);
        drawCurrExplosionSprite(explosionPlayer);
    }
}

void destroyPlayerShip(PlayerShip* playerShip) {
    if(playerShip == NULL) {
        return;
    }

    destroy_sprite(playerShip->curr_sprite);
    free((playerShip));
}

void updatePlayerScore(PlayerShip* playership) {
    playership->currScore += ENEMY_POINTS;
}

