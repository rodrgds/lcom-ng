#include "game.h"

/* Game players */
Player *player1 = NULL;       /** @brief Current player */
Player *player2 = NULL;       /** @brief Second player */
bool is_multiplayer = false;  /** @brief Whether the game is in multiplayer mode */

/* Game time variables */
int timeLeft;      /** @brief Time left in seconds * 60 */
bool slowTime;     /** @brief Whether the game is in slow time */
int timerSlowTime; /** @brief Time left in seconds * 60 for slow time */

/* Game explosion variables */
bool checkExplosion = false;      /** @brief Whether there is an explosion */
int explosionX;                   /** @brief X position of the explosion */
int explosionY;                   /** @brief Y position of the explosion */
int frameExplosion = 0;           /** @brief Frame of the explosion */
bool isUpdatingDynamites = false; /** @brief Whether the dynamite is updating */

Target *targets[NUM_TARGETS]; /** @brief Array of targets */
Dynamite *dynamite;           /** @brief Dynamite */

Player *(getPlayer1) () {
  return player1;
}

Player *(getPlayer2) () {
  return player2;
}

bool(isMultiplayer)() {
  return is_multiplayer;
}

void(setMultiplayer)(bool value) {
  is_multiplayer = value;
}

/**
 * GENERIC GAME FUNCTIONS
 */


void(initGame)(bool multiplayer) {
  is_multiplayer = multiplayer;
  player1 = createPlayer();
  if (multiplayer) {
    player2 = createPlayer();
  }
  timeLeft = 30 * 60;
  timerSlowTime = 0;
  slowTime = false;
  checkExplosion = false;

  int targetX = -100;
  int targetY = 100;

  for (int i = 0; i < NUM_TARGET_LINES; i++) {
    for (int j = 0; j < NUM_TARGETS_PER_LINE; j++) {
      targets[i * NUM_TARGETS_PER_LINE + j] = createTarget(targetX, targetY, (i % 2) ? LEFT : RIGHT);
      targetX += 200;
    }

    targetX = -100;
    targetY += 150;
  }

  dynamite = createDynamite(100, -100, DOWN);
}

void(endGame)() {
  destroyPlayer(player1);
  if (is_multiplayer && player2 != NULL)
    destroyPlayer(player2);

  for (int i = 0; i < NUM_TARGETS; i++)
    destroyTarget(targets[i]);

  destroyDynamite(dynamite);
}

// GAME UPDATES

void(updateTimes)() {
  timeLeft--;
  if (timerSlowTime > 0)
    timerSlowTime--;

  if (slowTime && (timerSlowTime == 0)) {
    endSlowTime();
  }
}

void(updateTargets)() {
  int step = slowTime ? 2 : 5;

  for (int i = 0; i < NUM_TARGETS; i++) {
    if (targets[i]->dir == RIGHT)
      targets[i]->pos.x = targets[i]->pos.x + step;
    else if (targets[i]->dir == LEFT)
      targets[i]->pos.x = targets[i]->pos.x - step;

    if (targets[i]->pos.x > 1200) {
      targets[i]->pos.x = -200;
      targets[i]->active = true;
      targets[i]->fallCounter = 0;
    }

    else if (targets[i]->pos.x < -200) {
      targets[i]->pos.x = 1200;
      targets[i]->active = true;
      targets[i]->fallCounter = 0;
    }
  }
}

void(updateDynamite)() {
  int step = slowTime ? 2 : 5;

  if (!isUpdatingDynamites) {
    dynamite->pos.x = ((getPlayerX(player1) + getPlayerY(player1) * timeLeft) % MAX_X); // pseudorandom
    isUpdatingDynamites = true;
  }

  dynamite->pos.y += step;
  if (getDynamiteY(dynamite) > 900) {
    dynamite->pos.y = -100;
    subractToScore(player1, 2);
    isUpdatingDynamites = false;
  }
}

// SHOTS AND COLLISIONS

int(checkAllCollisions)(Player *player) {

  if (checkCollisionWithDynamite(player))
    return true;

  int targetShot = checkCollisionWithTargets(player);

  return targetShot;
}

int(checkCollisionWithTargets)(Player *player) {
  int targetShot;

  if ((getPlayerY(player) && (getPlayerY(player) < 154))) {
    for (int i = 0; i < 7; i++) {
      targetShot = checkCollisionWithTarget(player, i);
      if (targetShot != -1)
        return targetShot;
    }
  }

  else if ((getPlayerY(player) > 196) && (getPlayerY(player) < 304)) {
    for (int i = 7; i < 14; i++) {
      targetShot = checkCollisionWithTarget(player, i);
      if (targetShot != -1)
        return targetShot;
    }
  }

  else if ((getPlayerY(player) > 346) && (getPlayerY(player) < 454)) {
    for (int i = 14; i < 21; i++) {
      targetShot = checkCollisionWithTarget(player, i);
      if (targetShot != -1)
        return targetShot;
    }
  }

  return -1;
}

int(checkCollisionWithTarget)(Player *player, int i) {
  int distance = (getPlayerX(player) - getTargetX(targets[i])) * (getPlayerX(player) - getTargetX(targets[i])) + (getPlayerY(player) - getTargetY(targets[i])) * (getPlayerY(player) - getTargetY(targets[i]));
  if (distance < TARGET_RADIUS_2) {
    setActiveTarget(i, false);

    if (distance < TARGET_RADIUS_2_CENTER) {
      addToScore(player, 5);
    }

    else if (distance < TARGET_RADIUS_2_MIDDLE) {
      addToScore(player, 3);
    }

    else {
      addToScore(player, 1);
    }

    return i;
  }

  return -1;
}

bool(checkCollisionWithDynamite)(Player *player) {
  int distance = (getPlayerX(player) - getDynamiteX(dynamite)) * (getPlayerX(player) - getDynamiteX(dynamite)) + (getPlayerY(player) - getDynamiteY(dynamite)) * (getPlayerY(player) - getDynamiteY(dynamite));

  if (distance < TARGET_RADIUS_2) {
    setActiveDynamite(false);
    checkExplosion = true;
    explosionX = getDynamiteX(dynamite);
    explosionY = getDynamiteY(dynamite);
    isUpdatingDynamites = false;
    dynamite->pos.y = -100;
    dynamite->active = true;
    return true;
  }

  return false;
}

// SLOW TIME

bool(canSlowTime)() {
  return (!slowTime && (timerSlowTime == 0) && !isMultiplayer());
}

void(setSlowTime)() {
  if (canSlowTime()) {
    slowTime = true;
    timerSlowTime = 5 * 60;
  }
}

void(endSlowTime)() {
  slowTime = false;
  timerSlowTime = 10 * 60;
}

/**
 * GETTERS AND SETTERS OF GAME VARIABLES
 */

// PLAYER POSITION
void(addToX)(Player *player, int16_t delta_x) {

  setPlayerX(player, getPlayerX(player) + delta_x);

  if (getPlayerX(player) < 0)
    setPlayerX(player, 0);
  if (getPlayerX(player) > MAX_X)
    setPlayerX(player, MAX_X);
}

void(addToY)(Player *player, int16_t delta_y) {
  setPlayerY(player, getPlayerY(player) - delta_y);

  if (getPlayerY(player) < 0)
    setPlayerY(player, 0);
  if (getPlayerY(player) > MAX_Y)
    setPlayerY(player, MAX_Y);
}

// TRGETS
int16_t(getFallCounterOfTarget)(int i) {
  return targets[i]->fallCounter;
}

void(incrementFallCounterOfTarget)(int i) {
  targets[i]->fallCounter++;
}

bool(isActiveTarget)(int i) {
  return targets[i]->active;
}

void(setActiveTarget)(int i, bool value) {
  targets[i]->active = value;
}

// DYNAMITE

bool(isActiveDynamite)() {
  return dynamite->active;
}

void(setActiveDynamite)(bool value) {
  dynamite->active = value;
}

void(addToScore)(Player *player, int value) {
  setPlayerScore(player, getPlayerScore(player) + value);

  if (getPlayerScore(player) > 9999) {
    setPlayerScore(player, 9999);
  }
}

void(subractToScore)(Player *player, int value) {
  if (getPlayerScore(player) <= value)
    setPlayerScore(player, 0);
  else
    setPlayerScore(player, getPlayerScore(player) - value);
}

// TIME

int(getTimeLeft)() {
  return timeLeft;
}

bool(endTime)() {
  return (timeLeft == 0);
}

/**
 * DRAW FUNCTIONS
 */

void(draw_game)(bool isDay) {
  if (isDay) {
    draw_background(dayDesert->map);
  }
  else {
    draw_background(nightDesert->map);
  }
  draw_lines();
  draw_targets();
  draw_dynamite();
  draw_aim(player1);
  if(is_multiplayer) {
    draw_aim(player2);
  }
  draw_sprite(scoreSprite, MAX_X - 400, MAX_Y - 65);
  draw_score();
  draw_timeLeft();

  if (canSlowTime())
    draw_sprite(clockIcon, 70, MAX_Y - 70);

  vg_page_flipping();
}

void(draw_aim)(Player *player) {
  draw_sprite(aim, getPlayerX(player), getPlayerY(player));
}

void(draw_lines)() {

  int lineY = 100 + TARGET_RADIUS - 15;
  for (int i = 0; i < 3; i++) {
    vg_draw_rectangle(0, lineY, MAX_X, 5, 0x696969);
    lineY += 150;
  }
}

void(draw_targets)() {
  for (int i = 0; i < NUM_TARGETS; i++) {
    if (isActiveTarget(i))
      draw_sprite(target, getTargetX(targets[i]), getTargetY(targets[i]));

    else if (targets[i]->fallCounter < 12) {
      draw_sprite(fall[getFallCounterOfTarget(i) / 4], getTargetX(targets[i]), getTargetY(targets[i]));
      targets[i]->fallCounter++;
    }
  }
}

void(draw_dynamite)() {
  if (isActiveDynamite()) {
    draw_sprite(dynamiteIcon, getDynamiteX(dynamite), getDynamiteY(dynamite));
  }

  if (checkExplosion) {
    draw_sprite(explosion[frameExplosion / 5], explosionX, explosionY);
    frameExplosion++;

    if (frameExplosion == 25) {
      checkExplosion = false;
      frameExplosion = 0;
    }
  }
}

void(draw_score)() {
  int numDigits = 0;
  int tempScore = getPlayerScore(player1);
  while (tempScore != 0) {
    tempScore /= 10;
    numDigits++;
  }

  if (numDigits == 0)
    numDigits = 1;

  int startX = MAX_X - 30;

  tempScore = getPlayerScore(player1);
  for (int i = numDigits - 1; i >= 0; i--) {
    int digit = tempScore % 10;
    draw_sprite(numbers[digit], startX, MAX_Y - 65);
    startX -= 50;
    tempScore /= 10;
  }
}

void(draw_timeLeft)() {
  int tempTime = getTimeLeft() / 60;
  int numDigitsTime = 0;
  while (tempTime != 0) {
    tempTime /= 10;
    numDigitsTime++;
  }

  if (numDigitsTime == 0)
    numDigitsTime = 1;

  int startX = MAX_X / 2;
  tempTime = getTimeLeft() / 60;
  for (int i = numDigitsTime; i > 0; i--) {
    int digit = tempTime % 10;
    draw_sprite(numbers[digit], startX, MAX_Y - 65);
    startX -= 50;
    tempTime /= 10;
  }
}
