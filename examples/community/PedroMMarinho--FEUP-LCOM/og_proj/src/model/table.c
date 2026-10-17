
#include "table.h"
#include "../labs/graphics.h"
#include "../physics/utilities.h"
#include "../viewer/lineViewer.h"
#include "../viewer/cueViewer.h"
#include "../xpms/ball.xpm"
#include "../xpms/biggerTable.xpm"
#include "../xpms/finalTable.xpm"
#include "../xpms/font/font.xpm"
#include "../xpms/matchUI.xpm"
#include "../xpms/slotBalls/ballSlot6.xpm"
#include "../xpms/table.xpm"
#include "../xpms/tableBackground.xpm"
#include "../xpms/tableBalls/bola1.xpm"
#include "../xpms/tableBalls/bola10.xpm"
#include "../xpms/tableBalls/bola11.xpm"
#include "../xpms/tableBalls/bola12.xpm"
#include "../xpms/tableBalls/bola13.xpm"
#include "../xpms/tableBalls/bola14.xpm"
#include "../xpms/tableBalls/bola15.xpm"
#include "../xpms/tableBalls/bola2.xpm"
#include "../xpms/tableBalls/bola3.xpm"
#include "../xpms/tableBalls/bola4.xpm"
#include "../xpms/tableBalls/bola5.xpm"
#include "../xpms/tableBalls/bola6.xpm"
#include "../xpms/tableBalls/bola7.xpm"
#include "../xpms/tableBalls/bola8.xpm"
#include "../xpms/tableBalls/bola9.xpm"
#include "../xpms/tableBalls/bolaBranca.xpm"
#include "../xpms/spinCircle.xpm"
#include "../xpms/exitButton.xpm"
#include "../xpms/exitButtonSelected.xpm"
#include "../xpms/cueCursor.xpm"

#include "math.h"

Table *newTable() {
  Table *table = (Table *) malloc(sizeof(Table));

  table->player1 = newPlayer("player1", PLAYERBALLNONE, 1);
  table->player2 = newPlayer("player2", PLAYERBALLNONE, 0);
  table->player1Won = true;

  table->font = malloc(sizeof(xpm_image_t) * 36);
  for (int i = 0; i < 36; i++) {
    xpm_load(xpm_table[i], XPM_8_8_8, &table->font[i]);
  }

  // Set balls
  xpm_image_t ball8;
  xpm_image_t whiteBall;
  xpm_image_t bola1;
  xpm_image_t bola2;
  xpm_image_t bola3;
  xpm_image_t bola4;
  xpm_image_t bola5;
  xpm_image_t bola6;
  xpm_image_t bola7;
  xpm_image_t bola9;
  xpm_image_t bola10;
  xpm_image_t bola11;
  xpm_image_t bola12;
  xpm_image_t bola13;
  xpm_image_t bola14;
  xpm_image_t bola15;

  xpm_load(bolaBrancaXpm, XPM_8_8_8, &whiteBall);
  xpm_load(bola8Xpm, XPM_8_8_8, &ball8);
  xpm_load(bola1Xpm, XPM_8_8_8, &bola1);
  xpm_load(bola2Xpm, XPM_8_8_8, &bola2);
  xpm_load(bola3Xpm, XPM_8_8_8, &bola3);
  xpm_load(bola4Xpm, XPM_8_8_8, &bola4);
  xpm_load(bola5Xpm, XPM_8_8_8, &bola5);
  xpm_load(bola6Xpm, XPM_8_8_8, &bola6);
  xpm_load(bola7Xpm, XPM_8_8_8, &bola7);
  xpm_load(bola9Xpm, XPM_8_8_8, &bola9);
  xpm_load(bola10Xpm, XPM_8_8_8, &bola10);
  xpm_load(bola11Xpm, XPM_8_8_8, &bola11);
  xpm_load(bola12Xpm, XPM_8_8_8, &bola12);
  xpm_load(bola13Xpm, XPM_8_8_8, &bola13);
  xpm_load(bola14Xpm, XPM_8_8_8, &bola14);
  xpm_load(bola15Xpm, XPM_8_8_8, &bola15);

  table->ballNumber = 16;
  table->balls = (Ball **) malloc(sizeof(Ball *) * table->ballNumber);


  vector_t cueBallPosition = {269, 442};

  table->balls[0] = newBall(cueBallPosition, whiteBall, WHITE);

  vector_t otherBallPosition = {782, 442};
  table->balls[1] = newBall(otherBallPosition, ball8, BLACK);
  otherBallPosition.x = 722;
  table->balls[2] = newBall(otherBallPosition, bola1, SOLID);
  otherBallPosition.x = 752;
  otherBallPosition.y = 426;
  table->balls[3] = newBall(otherBallPosition, bola2, SOLID);
  otherBallPosition.x = 782;
  otherBallPosition.y = 473;
  table->balls[4] = newBall(otherBallPosition, bola3, SOLID);
  otherBallPosition.x = 842;
  otherBallPosition.y = 502;
  table->balls[5] = newBall(otherBallPosition, bola4, SOLID);
  otherBallPosition.x = 812;
  otherBallPosition.y = 458;
  table->balls[6] = newBall(otherBallPosition, bola5, SOLID);
  otherBallPosition.x = 842;
  otherBallPosition.y = 411;
  table->balls[7] = newBall(otherBallPosition, bola6, SOLID);
  otherBallPosition.x = 812;
  otherBallPosition.y = 396;
  table->balls[8] = newBall(otherBallPosition, bola7, SOLID);
  otherBallPosition.x = 842;
  otherBallPosition.y = 472;
  table->balls[9] = newBall(otherBallPosition, bola9, STRIPED);
  otherBallPosition.x = 752;
  otherBallPosition.y = 458;
  table->balls[10] = newBall(otherBallPosition, bola10, STRIPED);
  otherBallPosition.x = 812;
  otherBallPosition.y = 427;
  table->balls[11] = newBall(otherBallPosition, bola11, STRIPED);
  otherBallPosition.x = 782;
  otherBallPosition.y = 412;
  table->balls[12] = newBall(otherBallPosition, bola12, STRIPED);
  otherBallPosition.x = 842;
  otherBallPosition.y = 380;
  table->balls[13] = newBall(otherBallPosition, bola13, STRIPED);
  otherBallPosition.x = 842;
  otherBallPosition.y = 442;
  table->balls[14] = newBall(otherBallPosition, bola14, STRIPED);
  otherBallPosition.x = 812;
  otherBallPosition.y = 489;
  table->balls[15] = newBall(otherBallPosition, bola15, STRIPED);

  xpm_image_t img;
  xpm_image_t matchUI;
  xpm_image_t spinCircle;
  xpm_image_t exitButton;
  xpm_image_t exitButtonSelected;
  xpm_image_t cursor;
  
  xpm_load(cueCursorXpm, XPM_8_8_8, &cursor);
  xpm_load(matchUIXpm, XPM_8_8_8, &matchUI);
  xpm_load(tableBackgroundXpm, XPM_8_8_8, &img);
  xpm_load(spinCircleXpm, XPM_8_8_8, &spinCircle);
  xpm_load(exitButtonXpm, XPM_8_8_8, &exitButton);
  xpm_load(exitButtonSelectedXpm, XPM_8_8_8, &exitButtonSelected);

  table->img = img;
  table->ui = matchUI;
  table->spinCircle = spinCircle;
  table->cursor = cursor;
  vector_t pos = {18, 9};
  vector_t size = {51, 51};
  table->exitButton = newButton(exitButton, exitButtonSelected, pos, size);
  table->exitSelected = false;

  // Set pockets
  vector_t p1, p2;
  p1.x = 40;
  p1.y = 200;
  table->pockets[0] = newPocket(p1, 29);
  p1.x = 512;
  p1.y = 192;
  table->pockets[1] = newPocket(p1, 29);
  p1.x = 986;
  p1.y = 200;
  table->pockets[2] = newPocket(p1, 29);
  p1.x = 986;
  p1.y = 686;
  table->pockets[3] = newPocket(p1, 29);
  p1.x = 512;
  p1.y = 697;
  table->pockets[4] = newPocket(p1, 29);
  p1.x = 40;
  p1.y = 686;
  table->pockets[5] = newPocket(p1, 29);

  // Set linear cushions
  double radius = 27;
  p1.x = 55;
  p1.y = 257;
  p2.x = 55;
  p2.y = 630;
  table->linearCushions[0] = newLinearCushion(p1, p2);
  p1.x = 27;
  p2.x = 27;
  table->circularCushions[0] = newCircularCushion(p1, radius);
  table->circularCushions[1] = newCircularCushion(p2, radius);
  p1.x = 95;
  p1.y = 672;
  p2.x = 463;
  p2.y = 672;
  table->linearCushions[1] = newLinearCushion(p1, p2);
  p1.y = 700;
  p2.y = 700;
  table->circularCushions[2] = newCircularCushion(p1, radius);
  table->circularCushions[3] = newCircularCushion(p2, radius);

  p1.x = 564;
  p1.y = 672;
  p2.x = 928;
  p2.y = 672;
  table->linearCushions[2] = newLinearCushion(p1, p2);
  p1.y = 700;
  p2.y = 700;
  table->circularCushions[4] = newCircularCushion(p1, radius);
  table->circularCushions[5] = newCircularCushion(p2, radius);

  p1.x = 972;
  p1.y = 257;
  p2.x = 972;
  p2.y = 630;
  table->linearCushions[3] = newLinearCushion(p1, p2);
  p1.x = 999;
  p1.y = 630;
  p2.x = 999;
  p2.y = 257;
  table->circularCushions[6] = newCircularCushion(p1, radius);
  table->circularCushions[7] = newCircularCushion(p2, radius);

  p1.x = 564;
  p1.y = 216;
  p2.x = 928;
  p2.y = 216;
  table->linearCushions[4] = newLinearCushion(p1, p2);
  p1.x = 928;
  p1.y = 188;
  p2.x = 564;
  p2.y = 188;
  table->circularCushions[8] = newCircularCushion(p1, radius);
  table->circularCushions[9] = newCircularCushion(p2, radius);

  p1.x = 95;
  p1.y = 216;
  p2.x = 463;
  p2.y = 216;
  table->linearCushions[5] = newLinearCushion(p1, p2);
  p1.x = 463;
  p1.y = 188;
  p2.x = 95;
  p2.y = 188;
  table->circularCushions[10] = newCircularCushion(p1, radius);
  table->circularCushions[11] = newCircularCushion(p2, radius);

  // Set mouse
  table->mouse = newMouse();

  // Set cue
  table->cue = newCue();
  updateCueState(table, false);
  table->maxSpeedShot = 4000;

  // Set state
  table->state = AIMING;

  // Set physics attributes
  table->gravityAcceleration = 9.81;
  table->slidingFriction = 90;
  table->spinningFriction = 6;
  table->rollingFriction = 30;
  table->cushionRestitution = 0.7;
  table->cushionFriction = 0.5;
  // game logic variables
  table->firstBallHit = NULL;
  table->pocketedOwnBall = false;

  return table;
}

void destroyTable(Table *table) {
  for (size_t i = 0; i < table->ballNumber; i++) {
    destroyBall(table->balls[i]);
    free(table->balls[i]);
  }
  for (size_t i = 0; i < 6; i++) {
    free(table->linearCushions[i]);
  }
  for (size_t i = 0; i < 12; i++) {
    free(table->circularCushions[i]);
  }
  for (size_t i = 0; i < 6; i++) {
    free(table->pockets[i]);
  }

  destroyButton(table->exitButton);
  free(table->balls);
  free(table->mouse);
  free(table->cue);
}




/**
 * @brief Draws the game time in minutes and seconds on the screen using the provided font.
 *
 * @param minText Text representing the minutes.
 * @param secText Text representing the seconds.
 * @param font Font used to render the text.
 * @return int 0 on success, 1 on failure.
 */
int drawTime(char *minText, char *secText, xpm_image_t *font) {
  if (drawText(minText, font, 478, 76, 16))
    return 1;
  if (vg_draw_rectangle(513, 70, 5, 5, 0x000000))
    return 1;
  if (vg_draw_rectangle(513, 80, 5, 5, 0x000000))
    return 1;
  if (drawText(secText, font, 535, 76, 16))
    return 1;
  return 0;
}
/**
 * @brief Draws the countdown timer on the screen.
 *
 * @param font Font used to render the text.
 * @param roundTime Time remaining in the round.
 * @param x X coordinate for the countdown timer.
 * @param y Y coordinate for the countdown timer.
 * @return int 0 on success, 1 on failure.
 */
int drawCountDown(xpm_image_t *font, int roundTime, int x, int y) {
  char secBuffer[3];
  snprintf(secBuffer, sizeof(secBuffer), "%02d", roundTime);
  if (drawText(secBuffer, font, x, y, 16))
    return 1;
  return 0;
}
/**
 * @brief Draws the total game time on the screen.
 *
 * @param gameTime Total game time in seconds.
 * @param font Font used to render the text.
 * @return int 0 on success, 1 on failure.
 */
int drawGameTime(int gameTime, xpm_image_t *font) {
  int minutes = gameTime / 60;
  int seconds = gameTime % 60;
  char minBuffer[3]; // "MM" + null terminator
  char secBuffer[3]; // "SS" + null terminator
                     // Format minutes and seconds as strings
  snprintf(minBuffer, sizeof(minBuffer), "%02d", minutes);
  snprintf(secBuffer, sizeof(secBuffer), "%02d", seconds);
  if ((drawTime(minBuffer, secBuffer, font)))
    return 1;
  return 0;
}

bool isPlayerBall(Player *player, Ball *ball) {

  return ((player->ballType == PLAYERSOLID && ball->type == SOLID) || (player->ballType == PLAYERSTRIPED && ball->type == STRIPED));
}


void drawBalls(Table *table) {

  bool ballTypesAttributed = table->player1->ballType != PLAYERBALLNONE;

  int jump = 34;
  int i1 = 0;
  int i2 = 0;

  for (size_t i = 0; i < table->ballNumber; i++) {
    Ball *ball = table->balls[i];
    if (ball->state != POCKETED || ball->type == WHITE) {
      drawXPMImage(getBallImage(ball), getBallPosition(ball).x, getBallPosition(ball).y, 0);
    }

    if (ballTypesAttributed) {
      if (ball->state == POCKETED || ball->type == WHITE || ball->type == BLACK)
        continue;
      if (isPlayerBall(table->player1, ball)) {
        drawXPMImage(getBallImage(ball), 104 + jump * i1, 83, 0);
        i1++;
      }
      else {

        drawXPMImage(getBallImage(ball), 924 - jump * i2, 86, 0);
        i2++;
      }
    }
  }
}


bool updateSpin(Table* table){

  vector_t click = table->mouse->pos;
  vector_t centerPoint = {978, 42};
  vector_t connection = {click.x - centerPoint.x, click.y - centerPoint.y};

  double distance = magnitudeOf(connection);
  if (distance > 30) return 0;

  table->cue->verticalEnglish = - connection.y / 30;
  table->cue->sideEnglish = -connection.x / 30;
  return true;
}

/**
 * @brief Draws the spin circle on the screen.
 *
 * @param table Pointer to the Table structure.
 */

void drawSpinCircle(Table* table){

  vector_t noSpinPos = {978, 42};
  double radius = 30;

  double up = table->cue->verticalEnglish * radius;
  double side = table->cue->sideEnglish * radius;

  drawXPMImage(table->spinCircle, noSpinPos.x - side, noSpinPos.y - up, 0);

}


int drawTable(Table *table, int gameTime, int roundTime) {

  drawXPMImage(table->img, 512, 442, 0);
  drawXPMImage(table->ui, 512, 62, 0);
  drawSpinCircle(table);
  drawInGamePlayerName(table->player1, table->font, 145, 47, 16);
  drawInGamePlayerName(table->player2, table->font, 748, 49, 16);

  //DEBUG
  
  /* vector_t p1, p2;
  p1.x = 55;
  p1.y = 257;
  p2.x = 55;
  p2.y = 630;

  drawThickLine(p1, p2, 5, 0xFFFFFF);
  p1.x = 27;
  p2.x = 27;
  drawXPMImage()

  p1.x = 95;
  p1.y = 672;
  p2.x = 463;
  p2.y = 672;
  drawThickLine(p1, p2, 5, 0xFFFFFF);

  p1.x = 564;
  p1.y = 672;
  p2.x = 928;
  p2.y = 672;
  drawThickLine(p1, p2, 5, 0xFFFFFF);

  p1.x = 972;
  p1.y = 257;
  p2.x = 972;
  p2.y = 630;
  drawThickLine(p1, p2, 5, 0xFFFFFF);
  p1.x = 564;
  p1.y = 216;
  p2.x = 928;
  p2.y = 216;
  drawThickLine(p1, p2, 5, 0xFFFFFF); 

  p1.x = 95;
  p1.y = 216;
  p2.x = 463;
  p2.y = 216;

  drawThickLine(p1, p2, 5, 0xFFFFFF);
 */
  // END OF DEBUG
  drawXPMImage(table->exitSelected ? table->exitButton->imgSelected:table->exitButton->img, table->exitButton->pos.x + 30, table->exitButton->pos.y + 30, 0);

  // DRAW BALLS
  drawBalls(table);

  if (drawGameTime(gameTime, table->font))
    return 1; // game Time
  if (table->player1->isPlaying) {
    drawCountDown(table->font, roundTime, 372, 60);
  }
  else {
    drawCountDown(table->font, roundTime, 632, 60);
  }

  switch (table->state) {
    case AIMING:
    case SHOOTING:
      drawCue(table);
      break;
    default:
      break;
  }
  if (drawXPMImage(table->cursor,table->mouse->pos.x, table->mouse->pos.y,0))
    return 1;

  return 0;
}


bool getColisionPoint(Table *table, vector_t *colisionPoint) {

  Cue *cue = table->cue;
  Ball *cueBall = table->balls[0];
  Ball *collisionBall = NULL;
  double minimalDistance = INFINITY;

  for (size_t i = 1; i < table->ballNumber; i++) {
    Ball *ball = table->balls[i];
    if (ball->state == POCKETED)
      continue;
    vector_t s = {cueBall->position.x - ball->position.x, cueBall->position.y - ball->position.y};
    double b = s.x * cue->directionVector.x + s.y * cue->directionVector.y;
    // TODO Fix ball radius hard code
    double c = s.x * s.x + s.y * s.y - 30 * 30;
    double h = b * b - c;
    if (h < 0) {
      continue;
    }
    else {
      h = sqrt(h);
      double t = 0 - b - h;
      if (t < 0)
        continue;
      else {
        vector_t connection = {ball->position.x - cueBall->position.x, ball->position.y - cueBall->position.y};
        double distance = magnitudeOf(connection);
        if (distance < minimalDistance) {
          minimalDistance = distance;
          collisionBall = ball;

          colisionPoint->x = cueBall->position.x + cue->directionVector.x * t;
          colisionPoint->y = cueBall->position.y + cue->directionVector.y * t;

          cue->targetBallCenter = ball->position;
        }
      }
    }
  }
  if (!collisionBall) {
    // TODO Colision with the sides
    colisionPoint->x = cueBall->position.x + cue->directionVector.x * 500;
    colisionPoint->y = cueBall->position.y + cue->directionVector.y * 500;
    return false;
  }
  return true;
}


int updateCueState(Table *table, bool power) {

  vector_t target;
  vector_t cueBall = table->balls[0]->position;
  Mouse *mouse = table->mouse;
  Cue *cue = table->cue;
  if (power) {

    target = table->mouse->savedPos;
    // Calculate the dot product
    cue->charge += (cue->directionVector.x * -mouse->delta.x + cue->directionVector.y * -mouse->delta.y) / cue->maximumDistance * cue->powerSensitivity;

    cue->charge = MAX(0, cue->charge);
    cue->charge = MIN(1, cue->charge);
  }
  else {
    target = table->mouse->pos;
    cue->directionVector = unitVector(cueBall, target);

    cue->angle = angle(cue->directionVector);

    if (getColisionPoint(table, &cue->colisionPoint)) {

      // Vector that connects target ball and colision point
      cue->targetBallVec = unitVector(cue->colisionPoint, cue->targetBallCenter);
      cue->guideScalar = dotProduct(cue->directionVector, cue->targetBallVec);

      double sinOfImpact = cue->directionVector.x * cue->targetBallVec.y - cue->directionVector.y * cue->targetBallVec.x;

      if (sinOfImpact < 0) {

        cue->whiteBallVec.x = -cue->targetBallVec.y;
        cue->whiteBallVec.y = cue->targetBallVec.x;
      }
      else {
        cue->whiteBallVec.x = cue->targetBallVec.y;
        cue->whiteBallVec.y = -cue->targetBallVec.x;
      }

      cue->colides = true;
    }
    else {
      cue->targetBallVec.x = 0;
      cue->targetBallVec.y = 0;
      cue->whiteBallVec.x = 0;
      cue->whiteBallVec.y = 0;

      cue->colides = false;
    }
  }

  float distance = cue->charge * cue->maximumDistance + cue->minimalDistance;
  cue->position.x = 212 + distance;
  cue->position.y = 0;
  cue->position = rotate2d(cue->position, cue->angle + M_PI);
  cue->position.x += cueBall.x;
  cue->position.y += cueBall.y;

  return 0;
}

void glueBall(Table* table) {
  table->balls[0]->position.x = table->mouse->pos.x;
  table->balls[0]->position.y = table->mouse->pos.y;
}
bool canDropBall(Table* table) {
  for(size_t i = 1; i < table->ballNumber; i++){
    if(table->balls[i]->state != POCKETED){
      vector_t distance = {table->balls[0]->position.x - table->balls[i]->position.x, table->balls[0]->position.y - table->balls[i]->position.y};
      if(magnitudeOf(distance) < table->balls[0]->radius + table->balls[i]->radius){
        return false;
      }
    }
  }
  if(table->mouse->pos.x < 55 + table->balls[0]->radius || table->mouse->pos.x > 972 - table->balls[0]->radius || table->mouse->pos.y < 188 + table->balls[0]->radius|| table->mouse->pos.y > 672 - table->balls[0]->radius){
    return false;
  }
  return true;
}

/**
 * @brief Switches the turn between players.
 *
 * This function switches the turn between players after a shot is taken.
 * It also resets some game state variables for the new turn.
 *
 * @param table Pointer to the Table structure.
 */
void switchTurn(Table* table){
  table->player1->isPlaying = !table->player1->isPlaying;
  table->player2->isPlaying = !table->player2->isPlaying;
  table->firstBallHit = NULL;
  table->pocketedOwnBall = false;
}
Player* getPlayingPlayer(Table* table){
  return table->player1->isPlaying ? table->player1 : table->player2;
}
Player* getNotPlayingPlayer(Table* table){
  return table->player1->isPlaying ? table->player2 : table->player1;
}

