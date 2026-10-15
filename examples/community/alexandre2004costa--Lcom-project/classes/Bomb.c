#include "Bomb.h"

/** @defgroup Bomb Bomb
  * @{
 */

/**
 * @brief Creates an array of bombs Objects.
 * @param maxBombNumber Number of bombs.
 * @return The created bombs Object.
 * @details A bombs Object is a struct made of an array of arrays of Objects, and number of bombs
 */
Bombs *create_bombsArray(int maxBombNumber){
    Bombs* newBombs = (Bombs*)malloc(sizeof(Bombs)); 
    newBombs->bombs = (Object**)malloc(sizeof(Object*) * maxBombNumber); 
    newBombs->sizeBombs = 0;
    return newBombs;
}

/**
 * @brief Creates a bomb/bullet Object with vertical speed.
 * @param b Bombs Object that will hold the new bombs.
 * @param x Position on the x axis.
 * @param y Position on the y axis.
 * @param down If true is bomb, bullet otherwise
 * @see create_object() for used method of creating an object.
 */
void (createBomb)(Bombs *b, int x, int y, bool down){
  Object *bomb;
  if (down) bomb = create_object((xpm_map_t) bomb_xpm, x, y);
  else bomb = create_object((xpm_map_t) bala_xpm, x, y);
  if (bomb != NULL){
    if (down) bomb->yspeed = 5;
    else bomb->yspeed = -10;
    b->bombs[b->sizeBombs] = bomb;
    b->sizeBombs++;
  }
}

/**
 * @brief Removes Bombs Object from array of Bombs.
 * @param b Bombs Object.
 * @param indexToRemove Index of the bomb to be removed.
 */
void (removeBomb)(Bombs *b, int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= b->sizeBombs) return;
    free(b->bombs[indexToRemove]);
    for (int i = indexToRemove; i < (b->sizeBombs - 1); i++) {
        b->bombs[i] = b->bombs[i + 1];
    }
    update_score(1); // Update the score when a bomb explodes
    b->sizeBombs--;
}
/**
 * @brief Checks colision between player and a bomb and changes the bomb sprite when it hits the ground.
 * @param bombsArray All bombs in the field.
 * @param count A certain frequency that defines when the bombs speed increase (acceleration).
 * @return The number of colisions.
 * @see removeBomb() for bomb removal from array of Bombs.
 * @see changeSprite() for changing xpm map associated with a sprite.
 * @see update_score() to increment score.
 */
int (bombMovement)(Bombs *bombsArray, int count){
    for (int i = 0; i < bombsArray->sizeBombs; i++){
      Object *bomb = bombsArray->bombs[i];
      if (bomb->sprite->y + bomb->sprite->height >= 700){
          if (bombsArray->bombs[i]->yspeed != 900) bomb->yspeed = 0;
          if (bomb->xspeed == 0){
            changeSprite(bomb->sprite, (xpm_map_t) explode_xpm);
            
            bombsArray->bombs[i]->xspeed++;
          }
          else if (bombsArray->bombs[i]->xspeed == 14){
            removeBomb(bombsArray, i);
            i--;
            continue;
          }else{
            if (count % 7 == 0)bombsArray->bombs[i]->xspeed++;
          }     
      }
      if (bombsArray->bombs[i]->yspeed != 900) bomb->sprite->y += bomb->yspeed;
    }
    return 0;
}

/**
 * @brief Checks colision between any bullet and any Plane Object, and handles plane crashing animations.
 * @param bulletArray All bullets fired by the player.
 * @param planesArray A Planes Object representing an array of all planes.
 * @see removeBomb() for bomb removal from array of Bombs.
 * @see update_score() to increment score.
 * @details For the sake of simplicity, the Bombs Object is used to represent bullets, with a different xpm.
 */
void (bulletMovement)(Bombs *bulletArray, Planes *planesArray){
  for (int i = 0; i < bulletArray->sizeBombs; i++){
    Object *bullet = bulletArray->bombs[i];
    bullet->sprite->y += bullet->yspeed;
    bool removed = false;
    if (bullet->sprite->y > 100 + 127 || bullet->sprite->y + bullet->sprite->height < 100) continue; // Bullet is not in the planes aerial space
    for (int j = 0; j < planesArray->sizePlanes; j++){
      Plane *plane = planesArray->plane[j];
      if (bullet->sprite->x + bullet->sprite->width < plane->planeObject->sprite->x + plane->planeObject->sprite->width/3 &&
                bullet->sprite->x > plane->planeObject->sprite->x) {
        removeBomb(bulletArray, i);
        removed = true;
        i--;
        plane->state = 2; // Crashed state
        plane->planeObject->yspeed = 10;
        plane->planeObject->xspeed = 0;
        break;
      }
    }
    if (removed) continue;
  }
}


/**
 * @brief Draws bombs from the Bombs Object array, depending if they are on the ground or not.
 * @param bombsArray All bombs in the field.
 * @see drawMultipleSprite() to understand how division of the total xpm works.
 * @see drawObject() to understand how object drawing works.
 */
void (drawBombs)(Bombs *bombsArray){
    for (int i = 0; i < bombsArray->sizeBombs; i++){
        if (bombsArray->bombs[i]->sprite->y + bombsArray->bombs[i]->sprite->height >= 700){ // is on the ground
          drawMultipleSprite(bombsArray->bombs[i]->xspeed, 5, 3, bombsArray->bombs[i]->sprite);
        }else drawObject(bombsArray->bombs[i]);
    }
}

/**
 * @brief Draws bullets from the Bombs Object array.
 * @param bulletArray All bullets fired by the player.
 * @details For the sake of simplicity, the Bombs Object is used to represent bullets, with a different xpm.
 * @see drawObject() to understand how object drawing works.
 */
void (drawBullets)(Bombs *bulletArray){
  for (int i = 0; i < bulletArray->sizeBombs; i++){
      drawObject(bulletArray->bombs[i]);
    }
}

int bombCollision(Bombs *bombsArray, Player *player){
    int pX = player->sprite->x + player->sprite->width;
    int pY = player->sprite->y + player->sprite->height;
    int lostLifes = 0;
    for (int i = 0; i < bombsArray->sizeBombs; i++){
      Object *bomb = bombsArray->bombs[i];
      if (bomb->sprite->y + bomb->sprite->height >= 700){
          if (bombsArray->bombs[i]->yspeed == 900) continue;
          int mX = bomb->sprite->x + bomb->sprite->width/10;
          int mY = bomb->sprite->y + bomb->sprite->height/6;
          int distance = sqrt((mY - pY)*(mY - pY) + (mX - pX)*(mX - pX));
          if (distance < 50){
              bombsArray->bombs[i]->yspeed = 900; // Para marcar q já atingiu
              lostLifes++;
            }
          } 
      }
    return lostLifes;
  }
/**@}*/
