#include "snake.h"

static enum KEY MovGeneral;
static enum KEY MovAux;
static double velocity;
static SnakeBody snake; // the snake object
extern enum STATE GameState; // the game state
extern int counter;
static Object array[5000]; // array to store the fruits
static Object walls[5000]; // array to store the blocks
static int numObjects = 0; // number of objects
static int fruitEaten = 1; // variable important to make a fruit spawn when there is none
static int cleanMouseX = 0; // save mouse last X
static int cleanMouseY = 0; // save mouse last Y
static int numFruits = 0; // number of fruits
static int numBlocks = 0; // number of blocks
bool start = true;
static int mouseUsed = 0;

extern Character letters[40]; // array of the letters

// loads xpm
extern uint8_t *font;
extern xpm_image_t imgFont;

extern uint8_t *snakeUp;
extern xpm_image_t imgSnakeUp;

extern uint8_t *snakeLeft;
extern xpm_image_t imgSnakeLeft;

extern uint8_t *snakeRight;
extern xpm_image_t imgSnakeRight;

extern uint8_t *snakeDown;
extern xpm_image_t imgSnakeDown;


extern uint8_t *fruitI;
extern xpm_image_t imgFruit;

extern uint8_t *gameBG;
extern xpm_image_t imgGameBG;

extern uint8_t *snakeTail;
extern xpm_image_t imgSnakeTail;

extern mouseInfo mouse;

extern uint8_t *cursor;
extern xpm_image_t imgCursor;

extern uint8_t *brickD;
extern xpm_image_t imgBrick;

extern uint8_t *broken;
extern xpm_image_t imgBroken;



void MenuStarter(){
  MovGeneral = DOWN;
  snake.y = 96;
  snake.bodySize = 0;
  snake.x = 288;
  snake.bodyType = HEAD;
  velocity = 48;
  numObjects = 0;
  fruitEaten = 1;
  cleanMouseX = 0;
  cleanMouseY = 0;
  numFruits = 0;
  numBlocks = 0;
  start = true;
  mouseUsed = 0;
  memset(array, 0, sizeof array);
  memset(walls, 0, sizeof walls);
}

void spawnFruits() {
  bool canSpawn = true;
  int x,y;
  x = (rand() % 22) + 1;
  y = (rand() % 16) + 1;

  for(int i = 0; i < numObjects; i++) {
    if(x == array[i].x && y == array[i].y && array[i].active) {
      canSpawn = false;
    }
  }

  for(int i = 0; i < snake.bodySize; i++) {
    if(x == snake.bodyX[i] && y == snake.bodyY[i]) {
      canSpawn = false;
    }
  }

  if(x == snake.x && y == snake.y) {
    canSpawn = false;
  } 

  /*for(int i = 0; i < numBlocks; i++) {
    if(x == walls[i].x && y == walls[i].y && walls[i].active) {
      canSpawn = false;
    }
  }*/

  if(canSpawn && fruitEaten && (!numFruits)) {
    Object obj;
    obj.x = x;
    obj.y = y;
    obj.type = FRUIT;
    obj.active = true;
    array[numObjects] = obj;
    numObjects++;
    numFruits++;
    fruitEaten = 0;
  }
}

void InterruptHandlerTimer(){
  cleanAllBG();
  if(start) {
     startBlocks();
     start = false;
  }
  spawnFruits();
  updateMov();
  if(counter % 8 == 0){
    moveSnake();
  }
  CheckColisions();
  drawSnake();
  drawObjects();
  drawScore();
}

void drawSnake() {
  
  switch (MovGeneral)
  {
  case UP:
    draw_pix_map(snake.x ,snake.y,snakeUp,imgSnakeUp);
    break;
  
  case RIGHT:
    draw_pix_map(snake.x ,snake.y,snakeDown,imgSnakeDown);
    break;

  case LEFT:
    draw_pix_map(snake.x ,snake.y,snakeLeft,imgSnakeLeft);
    break;
    
  case DOWN:
    draw_pix_map(snake.x ,snake.y,snakeRight,imgSnakeRight);
    break;    

  default:
    break;
  }


  for(int i = 0; i < snake.bodySize; i++) {
    draw_pix_map(snake.bodyX[i] * PIXELOFFSET,snake.bodyY[i] *PIXELOFFSET,snakeTail,imgSnakeTail);
  }
}

void drawObjects() {
  for(int j = 0; j < numBlocks; j++) {
    if(walls[j].active) {
        if(walls[j].type == BLOCK) {
          draw_pix_map(walls[j].x,walls[j].y,brickD,imgBrick);
        }
        else {
          draw_pix_map(walls[j].x,walls[j].y,broken,imgBroken);
        }
    }
  }

  for(int i = 0; i < numObjects; i++) {
    if(array[i].active) {
      draw_pix_map(array[i].x * PIXELOFFSET,array[i].y  * PIXELOFFSET,fruitI,imgFruit);
    }
  }
  cleanMouseX = mouse.delta_x;
  cleanMouseY = mouse.delta_y;
  draw_pix_map(mouse.delta_x,mouse.delta_y,cursor,imgCursor);
}

void drawBG() {
   draw_pix_map(0,0,gameBG,imgGameBG);
}

void moveSnake() {
  for (int i = snake.bodySize; i > 0; i--)
  {
    snake.bodyX[i] = snake.bodyX[i - 1];
    snake.bodyY[i] = snake.bodyY[i - 1];
  }
  snake.bodyX[0] = snake.x / PIXELOFFSET;
  snake.bodyY[0] = snake.y / PIXELOFFSET;


  switch (MovGeneral)
  {
  case UP:
    snake.y -= velocity;
    break;
  
  case DOWN:
    snake.y += velocity;
    break;

  case LEFT:
    snake.x -= velocity;
    break;
    
  case RIGHT:
    snake.x += velocity;
    break;    

  default:
    break;
  }
}

int GameOverLeaderBoard(){
  leaderboard(snake.bodySize);
  GameState = DEAD;
  return 0;
}

int CheckColisions(){
  for(int i = 0; i < numObjects; i++) {
    if((snake.x == array[i].x * PIXELOFFSET) && (snake.y == array[i].y * PIXELOFFSET) && (array[i].active) && (array[i].type == FRUIT)) {
      array[i].active = false;
      fruitEaten = 1;
      numFruits--;
      snake.bodySize++;
      return 0;
    }
    else if((snake.x == array[i].x * PIXELOFFSET) && (snake.y == array[i].y * PIXELOFFSET) && (array[i].active) && (array[i].type == BLOCK)) {
      GameOverLeaderBoard();
      return 0;
    }
  }
  if(snake.x <= PIXELOFFSET - 1 || snake.x >= (23 * PIXELOFFSET) || snake.y <= PIXELOFFSET - 1 || snake.y >= ((17 * PIXELOFFSET) - 1)) {
      GameOverLeaderBoard();
      return 0;
    }
  for(int i = 0; i < snake.bodySize; i++) {
    if((snake.x == snake.bodyX[i] * PIXELOFFSET) && (snake.y == snake.bodyY[i] * PIXELOFFSET)) {
      GameOverLeaderBoard();
      return 0;
    }
  }
  for(int i = 0; i < numBlocks; i++) {
    if((snake.x == walls[i].x) && (snake.y == walls[i].y) && walls[i].active) {
      GameOverLeaderBoard();
      return 0;
    }
  }
  return 0;
}


void InterruptHandlerKBC(enum KEY k){
  if(k == ENTER) {
    return;
  }
  if(k == ESC) {
    GameState = PAUSE;
    return;
  }
  MovAux = k;
}

void updateMov(){
  if(MovGeneral == LEFT && MovAux == RIGHT) {
    return;
  }
  else if(MovGeneral == RIGHT && MovAux == LEFT) {
    return;
  }
  else if(MovGeneral == UP && MovAux == DOWN) {
    return;
  }
  else if(MovGeneral == DOWN && MovAux == UP) {
    return;
  }
  else {
    MovGeneral = MovAux;
  }
}

void cleanAllBG() {
  for(int i = 0; i < snake.bodySize; i++) {
    cleanBG(snake.bodyX[i] * PIXELOFFSET,snake.bodyY[i] * PIXELOFFSET,PIXELOFFSET,PIXELOFFSET,imgGameBG,gameBG);
  }
  cleanBG(snake.x ,snake.y,PIXELOFFSET,PIXELOFFSET,imgGameBG,gameBG);
  cleanBG(cleanMouseX,cleanMouseY,imgCursor.width,imgCursor.height,imgGameBG,gameBG);
}

void InterruptHandlerMouse() {
  if(mouse.delta_x >= 1152 - PIXELOFFSET) {
    mouse.delta_x = 1152 - PIXELOFFSET - 1;
  }
  else if(mouse.delta_x < PIXELOFFSET) {
    mouse.delta_x = PIXELOFFSET;
  }
  if(mouse.delta_y >= 864 - PIXELOFFSET) {
    mouse.delta_y = 863 - PIXELOFFSET;
  }
  else if(mouse.delta_y < PIXELOFFSET) {
    mouse.delta_y = PIXELOFFSET;
  }
  if(mouse.rb && ((counter - mouseUsed) > 60)) {
    mouse.rb = false;
    mouseUsed = counter;
    if(checkFruit(mouse.delta_x / PIXELOFFSET,mouse.delta_y / PIXELOFFSET)) {
      return;
    }
    Object obj;
    obj.x = mouse.delta_x / PIXELOFFSET;
    obj.y = mouse.delta_y / PIXELOFFSET;
    obj.type = FRUIT;
    obj.active = true;
    array[numObjects] = obj;
    numObjects++;
    numFruits++;
  }
  if(mouse.lb && ((counter - mouseUsed) > 60)) {
     mouse.lb = false;
     mouseUsed = counter;
     int idx = findIdxBlock(mouse.delta_x / PIXELOFFSET, mouse.delta_y / PIXELOFFSET);
     printf("%d",idx);
     if(idx == -1) {
       return;
     }
     if(walls[idx].type == BLOCK) {
       walls[idx].type = BROKEN;
     }
     else if(walls[idx].type == BROKEN) {
       walls[idx].active = false;
       cleanBG(walls[idx].x,walls[idx].y,PIXELOFFSET,PIXELOFFSET,imgGameBG,gameBG);
     }
  }
}

bool checkFruit(int x, int y) {
  for(int i = 0; i < numObjects; i++) {
    if(array[i].x == x && array[i].y == y && array[i].active == true && array[i].type == FRUIT) {
      return true;
    }
  }
  for(int i = 0; i < snake.bodySize; i++) {
    if(snake.bodyX[i] == x && snake.bodyY[i] == y) {
      return true;
    }
  }
  for(int i = 0; i < numBlocks; i++) {
    if(walls[i].x == x && walls[i].y == y && walls[i].active == true) {
      return true;
    }
  }
  return false;
}

int findIdxBlock(int x, int y) {
  for(int i = 0; i < numBlocks; i++) {
    if((walls[i].x / PIXELOFFSET) == x && (walls[i].y  / PIXELOFFSET) == y && walls[i].active) {
      return i;
    }
  }
  return -1;
}

void initBlock(int x, int y) {
  Object w;
  w.x = x * PIXELOFFSET;
  w.y = y * PIXELOFFSET;
  w.type = BLOCK;
  w.active = true;
  walls[numBlocks] = w;
  numBlocks++;
}

void startBlocks() {

  // 1st corner
  initBlock(2,2);
  initBlock(3,2);
  initBlock(2,3);
  initBlock(3,3);

  // 2nd corner
  initBlock(2,14);
  initBlock(3,14);
  initBlock(2,15);
  initBlock(3,15);

  // 3rd corner
  initBlock(20,2);
  initBlock(21,2);
  initBlock(20,3);
  initBlock(21,3);

   // 3rd corner
  initBlock(20,14);
  initBlock(21,14);
  initBlock(20,15);
  initBlock(21,15);
  
  // midle
  initBlock(11,8);
  initBlock(12,8);
  initBlock(11,9);
  initBlock(12,9);

  initBlock(6,8);
  initBlock(7,8);
  initBlock(6,9);
  initBlock(7,9);

  initBlock(16,8);
  initBlock(17,8);
  initBlock(16,9);
  initBlock(17,9);
}

void drawScore() {
  int startPosX = 576;
  char score[4];
  sprintf(score,"%d",snake.bodySize);
  cleanBG(startPosX,0,PIXELOFFSET * 4,PIXELOFFSET,imgGameBG,gameBG);
  for(int i = 0; i < (int) strlen(score); i++) {
    draw_letter_map(startPosX - ((int)(score[i] - '0')*37) + i * 20 ,-40,font,imgFont,letters[(int)(score[i] - '0') + 26].x,letters[(int)(score[i] - '0') + 26].y);
  }

}
