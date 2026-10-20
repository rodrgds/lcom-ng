#include "game.h"
#include "enemy.h"
#include "images.h"
#include "linkedEntities.h"
//Extern variables located in images.h
xpm_image_t pistol_image;
xpm_image_t player_image;
xpm_image_t player_image_left;
xpm_image_t player_image_right;
xpm_image_t player_shot_image;
xpm_image_t cursor_image;
xpm_image_t enemy1_image;
xpm_image_t enemy2_image;

xpm_image_t pistol_0_image;
xpm_image_t pistol_1_image;
xpm_image_t pistol_2_image;
xpm_image_t pistol_3_image;

xpm_image_t letters_image;

xpm_image_t letters_image;
xpm_image_t numbers_image;
xpm_image_t bar_image;

/*
xpm_image_t explosion_0_image;
xpm_image_t explosion_1_image;
xpm_image_t explosion_2_image;
xpm_image_t explosion_3_image;
*/

Game game;
EntitiesLinkedElement *firstLinkedSprite;
EntitiesLinkedElement *lastLinkedSprite;

//static EntitiesLinkedElement *firstLinkedEnemy;
//static EntitiesLinkedElement *lastLinkedEnemy;
Player *player1;

void initGame(Game *game) {
  loadHighScore(game);
  getResolution(&game->x_res, &game->y_res);
  game->numOfPlayers = 1;
  game->state = MENU;
  initImages();
  player1 = (Player *) malloc(sizeof(Player));
  initPlayer(game, player1);
  game->players[0] = player1;
  game->entitiesList.firstElement = NULL;
  game->entitiesList.lastElement = NULL;
  addEntity(&game->entitiesList, player1, PLAYER);
  addEntity(&game->entitiesList, player1->cursor, SPRITE);
  game->spawnTime = ENEMY_INITIAL_TIME;
  game->nextSpawn = game->spawnTime;
}

void startGame(Game *game) {
  game->gameTime = 0;
  game->players[0]->score = 0;
  game->state = GAME;
}

void initImages() {
  enum xpm_image_type type = XPM_8_8_8;

  pistol_image.bytes = xpm_load(pistol_xpm, type, &(pistol_image));
  player_image.bytes = xpm_load(DEFAULTPLAYER, type, &(player_image));
  player_shot_image.bytes = xpm_load(player_shot_xpm, type, &(player_shot_image));
  cursor_image.bytes = xpm_load(cursor_xpm, type, &(cursor_image));
  enemy1_image.bytes = xpm_load(enemy1_xpm, type, &(enemy1_image));
  enemy2_image.bytes = xpm_load(enemy2_xpm, type, &(enemy2_image));
  player_image_left.bytes = xpm_load(MOV2PLAYER, type, &(player_image_left));
  player_image_right.bytes = xpm_load(MOV1PLAYER, type, &(player_image_right));

  pistol_0_image.bytes = xpm_load(pistol_0_xpm, type, &pistol_0_image);
  pistol_1_image.bytes = xpm_load(pistol_1_xpm, type, &pistol_1_image);
  pistol_2_image.bytes = xpm_load(pistol_2_xpm, type, &pistol_2_image);
  pistol_3_image.bytes = xpm_load(pistol_2_xpm, type, &pistol_3_image);

  letters_image.bytes = xpm_load(letters_xpm, type, &letters_image);
  numbers_image.bytes = xpm_load(numbers_xpm, type, &numbers_image);
  bar_image.bytes = xpm_load(bar_xpm, type, &bar_image);

  /*
  explosion_0_image.bytes = xpm_load(explosion_0_xpm, type, &explosion_0_image);
  explosion_1_image.bytes = xpm_load(explosion_1_xpm, type, &explosion_0_image);
  explosion_2_image.bytes = xpm_load(explosion_2_xpm, type, &explosion_0_image);
  explosion_3_image.bytes = xpm_load(explosion_3_xpm, type, &explosion_0_image);
  */
}

static void freeEntityPayload(DoubleLinkedList *list, EntitiesLinkedElement *element) {
  switch (element->type) {
    case ENEMY: {
      Enemy *enemy = (Enemy *) element->data;
      if (enemy == NULL) break;
      if (enemy->gun != NULL) {
        EntitiesLinkedElement *other = list->firstElement;
        while (other != NULL) {
          if (other->type == BULLET && ((Bullet *) other->data)->origin == enemy->gun) {
            ((Bullet *) other->data)->origin = NULL;
          }
          other = other->next;
        }
        if (enemy->gun->sprite != NULL) free(enemy->gun->sprite);
        free(enemy->gun);
      }
      free(enemy->sprite);
      free(enemy);
      break;
    }
    case BULLET: {
      Bullet *bullet = (Bullet *) element->data;
      if (bullet != NULL) {
        free(bullet->sprite);
        free(bullet);
      }
      break;
    }
    case GUN: {
      Gun *gun = (Gun *) element->data;
      if (gun != NULL) {
        free(gun->sprite);
        free(gun);
      }
      break;
    }
    case PLAYER:
    case SPRITE:
      break;
  }
}

void moveEntities(Game *game, double gameSpeed) {
  updateAnimations(game);
  EntitiesLinkedElement *elem = game->entitiesList.firstElement;
  while (elem != NULL) {
    Sprite *sprite = getSprite(elem);
    moveEntity(gameSpeed, elem);
    if (is_out_of_bounds(sprite->x, sprite->y, 100)) {
      //printf("Out of bounds %d %d\n", sprite->x, sprite->y);
      EntitiesLinkedElement *next = elem->next;
      freeEntityPayload(&game->entitiesList, elem);
      removeLinkedElement(&game->entitiesList, elem);
      elem = next;
    }
    else {
      elem = elem->next;
    }
  }
}

void moveEntity(double gameSpeed, EntitiesLinkedElement *entity) {
  if (entity->type == ENEMY) {
    Enemy *enemy = (Enemy *) entity->data;
    moveEnemy(enemy, gameSpeed);
  }
  else {
    moveSprite(gameSpeed, getSprite(entity));
  }
}

void moveSprite(double gameSpeed, Sprite *sprite) {
  if (!sprite->isMovementUnique) {
    double spriteSpeed = gameSpeed;
    if (sprite->ignoresTime) {
      spriteSpeed = 1;
    }
    sprite->realx += sprite->speedX * spriteSpeed;
    sprite->realy += sprite->speedY * spriteSpeed;
    sprite->x = (int) sprite->realx;
    sprite->y = (int) sprite->realy;
  }
}
void updateAnimations(Game *game) {
  //Player animation
  //updatePlayerAnimation(player1);
  EntitiesLinkedElement *elem = game->entitiesList.firstElement;
  while (elem != NULL) {
    updateEntityAnimation(elem);
    elem = elem->next;
  }
}

void updateEntityAnimation(EntitiesLinkedElement *entity) {
  switch (entity->type) {
    case PLAYER:
      updatePlayerAnimation((Player *) entity->data);
      updateGunAnimation(((Player *) entity->data)->gun);
      break;
    case SPRITE:

      break;
    case ENEMY:
      updateGunAnimation(((Enemy *) entity->data)->gun);
      break;
    case BULLET:

      break;
    case GUN:
      updateGunAnimation((Gun *) entity->data);
      break;
  }
}

void drawEntities(Game *game) {

  EntitiesLinkedElement *elem = game->entitiesList.firstElement;
  while (elem != NULL) {
    //sprite = getSprite(elem);
    //total++;

    //drawSprite(sprite);
    drawEntity(elem);
    elem = elem->next;
  }

  char score[30];
  sprintf(score, "Score %d", game->players[0]->score);
  drawString(score, 10, 10);
  applyChanges();
}

void drawEntity(EntitiesLinkedElement *entity) {
  switch (entity->type) {
    case PLAYER:
      drawPlayer((Player *) entity->data);
      break;
    case SPRITE:
      drawSprite((Sprite *) entity->data);
      break;
    case ENEMY:
      drawEnemy((Enemy *) entity->data);
      break;
    case BULLET:
      drawBullet((Bullet *) entity->data);
      break;
    case GUN:
      drawGun((Gun *) entity->data);
      break;
  }
}

int receiveKeyboardInterrupt(Game *game, uint32_t kbd_code) {
  if (game->state == GAME) {
    player_kb_input(player1, kbd_code);
  }
  else if (game->state == MENU) {
    if (kbd_code == BREAK_CODE(KEYCODE_ENTER)) {
      startGame(game);
    }
  }
  else if (game->state == GAMEOVER && game->gameTime > 3 * 60) {

    initGame(game);
  }
  return 0;
}

int receiveTimerInterrupt(Game *game) {
  if (game->state == GAME) {
    if (player1->input == 0) {
      game->gameSpeed = 0.05;
    }
    else if (player1->input & ~BIT(4)) { // if WASD is pressed, time is 1
      game->gameSpeed = 1;
    }
    game->gameTime = game->gameTime + game->gameSpeed * 17; // 1000 is aprox 1 second (in 60fps)
    if (game->gameTime >= game->nextSpawn) {
      spawnEnemy(game);

      //Increase difficulty
      if (game->enemyCount > ENEMIES_TO_INCREASE_DIFFICULTY & game->spawnTime > ENEMY_MIN_SPAWN_TIME) {
        game->spawnTime -= ENEMY_TIME_TO_DECREASE;
        game->enemyCount = 0;
      }

      game->nextSpawn += game->spawnTime;
    }

    moveEntities(game, game->gameSpeed);
    player_movement(game, game->gameSpeed);
    updateRotation(player1);
    drawEntities(game);

    handleCollisions(game);
    deleteEntities(game);
  }
  else if (game->state == MENU) {

    drawString("Super Cold", 200, 250);
    if(game->highScore > 0){
      char message[60];
      sprintf(message, "Highscore %d  %d-%d-%d", game->highScore, game->dateHighScore.day, game->dateHighScore.month, game->dateHighScore.year);
      drawString(message, 200, 300);
    }
    drawString("Press Enter to play", 200, 350);
    applyChanges();
  }
  else if (game->state == GAMEOVER) {
    if (game->gameTime <= 3 * 120) {
      game->gameTime += 1;
    }
    else {
      drawString("Press any key to go back to the main menu", 200, 350);
    }

    drawString("Super Cold", 200, 250);
    char message[60];
    sprintf(message, "You lost and got the score %d", game->players[0]->score);
    drawString(message, 200, 300);

    applyChanges();
  }

  return 0;
}

int receiveMouseInterrupt(Game *game, struct packet dataPacket) {
  if (game->state == GAME) {
    player_mouse_input(game, dataPacket);
    if (game->gameSpeed == 0.05) {
      //No keys were pressed
      //game->gameSpeed = 0.2;
    }
  }
  return 0;
}

void receivePeriodicInterrupt(Game *game) {
  if (game->state == GAME) {
    //Update Enemy movement
    EntitiesLinkedElement *elem = game->entitiesList.firstElement;
    while (elem != NULL) {
      if (elem->type == ENEMY) {
        enemyAction((Enemy *) elem->data, game->players[0]);
      }
      elem = elem->next;
    }
  }
}

void spawnEnemy(Game *game) {
  int x = 0, y = 0, max = 75;
  int randNumber = rand() % 4;
  if (randNumber == 0) {
    //Spawn Up side
    y = -(rand() % max);
    x = (rand() % (max * 2 + game->x_res)) - max;
  }
  else if (randNumber == 1) {
    //Spawn Right side
    y = (rand() % (max * 2 + game->y_res)) - max;
    x = rand() % max + game->x_res;
  }
  else if (randNumber == 2) {
    //Spawn bottom side
    y = rand() % max + game->y_res;
    x = (rand() % (max * 2 + game->x_res)) - max;
  }
  else if (randNumber == 3) {
    //Spawn left side
    //Spawn Right side
    y = (rand() % (max * 2 + game->y_res)) - max;
    x = -(rand() % max);
  }
  Enemy *enemy = (Enemy *) malloc(sizeof(Enemy));
  *enemy = createEnemy(game, player1, x, y);
  addEnemy(game, enemy);

  game->enemyCount++;
}

void addEnemy(Game *game, Enemy *enemy) {
  addEntity(&game->entitiesList, enemy, ENEMY);
}

Sprite *getSprite(EntitiesLinkedElement *elem) {
  Sprite *sprite;
  switch (elem->type) {
    case PLAYER:
      sprite = ((Player *) elem->data)->sprite;
      break;
    case SPRITE:
      sprite = (Sprite *) elem->data;
      break;
    case ENEMY:
      sprite = ((Enemy *) elem->data)->sprite;
      break;
    case BULLET:
      sprite = ((Bullet *) elem->data)->sprite;
      break;
    case GUN:
      sprite = ((Gun *) elem->data)->sprite;
      break;
    default:
      sprite = NULL;
      break;
  }
  return sprite;
}

void handleCollisions(Game *game) {
  DoubleLinkedList list = game->entitiesList;
  EntitiesLinkedElement *first = list.firstElement;
  while (first != NULL) {
    EntitiesLinkedElement *second = first->next;
    Sprite *firstSprite = getSprite(first);
    while (second != NULL) {
      if (checkColision(firstSprite, getSprite(second))) {
        handleCollision(game, first, second);
      }
      second = second->next;
    }
    first = first->next;
  }
}

void handleCollision(Game *game, EntitiesLinkedElement *first, EntitiesLinkedElement *second) {
  /*
    Bullet - Bullet
    Bullet - Enemy
    Bullet - Player
    Player -> Enemy
    Gun -> Enemy // Ignorar
    Gun -> Player // Ignorar
  */
  if (first->type == SPRITE || second->type == SPRITE) {
    return;
  }
  if (first->type == BULLET || second->type == BULLET) {
    if (second->type == BULLET) {
      EntitiesLinkedElement *aux = first;
      first = second;
      second = aux;
    }
    switch (second->type) {
      case BULLET:
        destroyEntity(game, first);
        destroyEntity(game, second);
        break;
      case ENEMY:
        if (((Bullet *) first->data)->origin != ((Enemy *) second->data)->gun) {
          destroyEntity(game, first);
          destroyEntity(game, second);
          game->players[0]->score += ((Enemy *) second->data)->score;
        }
        break;

      case PLAYER:
        if (((Bullet *) first->data)->origin != ((Player *) second->data)->gun) {
          destroyEntity(game, first);
          destroyEntity(game, second);
        }
        break;
      case GUN:
        break;
      case SPRITE:
        break;
    }
  }
}

void destroyEntity(Game *game, EntitiesLinkedElement *entity) {
  if (entity->type != PLAYER) {
    entity->remove = 1;
  }
  else if (entity->type == PLAYER) {
    game->gameTime = 0;
    game->state = GAMEOVER;
    saveHighScore(game, game->players[0]->score);
  }
}

void deleteEntities(Game *game) {
  EntitiesLinkedElement *elem = game->entitiesList.firstElement;
  while (elem != NULL) {
    EntitiesLinkedElement *next = elem->next;
    if (elem->remove) {
      freeEntityPayload(&game->entitiesList, elem);
      removeLinkedElement(&game->entitiesList, elem);
    }

    elem = next;
  }
}

void saveHighScore(Game *game, int score) {
  if (score > game->highScore) {
    FILE *file = fopen("score.txt", "w");
    if (file != NULL) {
      Date date = getDate();
      printf("%x %x %x\n", date.day, date.month, date.year);
      fprintf(file, "%d %x-%x-%x %x-%x-%x", score, date.day, date.month, date.year, date.hours, date.minutes, date.seconds);
      game->dateHighScore = date;
      fclose(file);
    }
  }
}

void loadHighScore(Game *game) {
  game->highScore = 0;
  FILE *file = fopen("score.txt", "r");
  if (file != NULL) {
    int score;
    Date date;
    fscanf(file, "%d %d-%d-%d %d-%d-%d", &score, &date.day, &date.month, &date.year, &date.hours, &date.minutes, &date.seconds);
    fclose(file);
    game->dateHighScore = date;
    game->highScore = score;
  }
}
