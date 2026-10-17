#include "gun.h"

static xpm_image_t *pistol_images[4] = {&pistol_0_image, &pistol_1_image, &pistol_2_image, &pistol_3_image};

Gun *getPistol(Game *game, uint8_t isEquipped, int x, int y) {
  Gun *gun = (Gun *) malloc(sizeof(Gun));
  gun->game = game;
  gun->sprite = createSprite(&pistol_0_image, x, y);
  gun->shotSpeed = 10;
  gun->shotDelay = 100;
  gun->timeToNextShot = 0;
  gun->isEquipped = isEquipped;
  gun->isShooting = 0;
  gun->current_animation_tick = 0;
  gun->current_image = 0;
  return gun;
}

Gun *getSMG(Game *game, uint8_t isEquipped, int x, int y) {
  Gun *gun = (Gun *) malloc(sizeof(Gun));
  gun->game = game;
  gun->sprite = createSprite(&pistol_0_image, x, y);
  gun->sprite->isMovementUnique = 1;
  gun->shotSpeed = 10;
  gun->shotDelay = 300;
  gun->timeToNextShot = 0;
  gun->isEquipped = isEquipped;
  gun->isShooting = 0;
  gun->current_animation_tick = 0;
  gun->current_image = 0;
  return gun;
}

void shoot(Gun *gun, unsigned targetX,unsigned targetY){
  //TODO::: get a better position for the shot
  Bullet * newShot = (Bullet *) malloc(sizeof(Bullet));
  newShot->origin = gun;
  int gunBarrelX, gunBarrelY;
  getRotationCoordinate(gun->sprite, gun->sprite->x + gun->sprite->image->width, gun->sprite->y + gun->sprite->image->height, &gunBarrelX, &gunBarrelY);
  newShot->sprite = createSprite(&player_shot_image, gunBarrelX, gunBarrelY); 
  setSpeed(newShot->sprite, gun->shotSpeed, targetX - newShot->sprite->x, targetY - newShot->sprite->y);
  addEntity(&gun->game->entitiesList, newShot, BULLET);
  gun->timeToNextShot = gun->shotDelay;
  gun->isShooting = 1;
  gun->current_animation_tick = 0;
  gun->current_image = 1;

}


void updateGunAnimation(Gun * gun){
  if(gun->isShooting){
    
    if(gun->current_animation_tick++ == 5){
      gun->current_image++;
      if(gun->current_image == 4){
        gun->current_image = 0;
        gun->isShooting = 0;
      }
      gun->current_animation_tick = 0;
    }
  }
  gun->sprite->image = pistol_images[gun->current_image];
}
