#include <lcom/lcf.h>

#include "sprite.h"
#include "gameElements/cursor.h"
#include "gameElements/playerShip.h"
#include "gameElements/spaceship_explosion.h"
#include "gameElements/enemyExplosion.h"
#include "gameElements/enemy.h"
#include "gameElements/fire.h"
#include "gameElements/enemyFire.h"
#include "devices/videocard.h"
#include "devices/VBE.h"



#define SCREEN_WIDTH 1152
#define SCREEN_HEIGHT 864


extern Cursor* cursor;

Sprite* background;
Sprite* gameName;
Sprite* gameOver;
Sprite* sun;

Sprite* numberArray[10];
Sprite* twoDots;
Sprite* slash;
Sprite* secondS;
Sprite* points;
Sprite* heart;
Sprite* levelSprite;
Sprite* fireShots;

Sprite* instructions;


extern PlayerShip* player;
extern Spaceship_Explosion* explosionPlayer;



extern Enemy* enemyArray[MAX_ENEMY];


extern EnemyExplosion*  enemyExplosion[];


Sprite* quitButton;
Sprite* playButton;
Sprite* leaderboardButton;
Sprite* instructionsButton;
Sprite* quitButton2;
Sprite* playButton2;
Sprite* leaderboardButton2;
Sprite* instructionsButton2;
Sprite* backButton;
Sprite* backButton2;

extern Fire* fireArray[MAX_FIRE];
extern EnemyFire* enemyFireArray[MAX_ENEMY_FIRE];


Sprite * create_sprite(xpm_map_t xpm , uint16_t x, uint16_t y, uint16_t x_speed, uint16_t y_speed) {
  Sprite *sprite = (Sprite*)(malloc(sizeof(Sprite)));
  if(sprite == NULL) {
    printf("Error: failed to create Sprite \n");
    return NULL;
  }
  xpm_image_t new_xpm = my_load_xpm(xpm);

  sprite->width = new_xpm.width;
  sprite->height = new_xpm.height;

  sprite->x = x;
  sprite->y = y;

  sprite->xspeed = x_speed;
  sprite->yspeed = y_speed;

  sprite->map = (uint32_t*)new_xpm.bytes;

  if(sprite->map == NULL)  {
    free(sprite);
    return NULL;
  }
  
  return sprite;

}

bool check_borders_collisions(Sprite* sprite) {

  uint16_t next_x = sprite->x +sprite->xspeed +sprite->width ;
  uint16_t next_x_neg = sprite->x +sprite->xspeed;
  uint16_t next_y = sprite->y +sprite->yspeed +sprite->height;
  uint16_t next_y_neg = sprite->y +sprite->yspeed;

  if(next_x> get_hres() || next_x_neg > get_hres() || next_y > get_vres() || next_y_neg > get_vres() ) {
    return true;
  }
  
  return false;
}


void destroy_sprite(Sprite *sprite) {
  if(sprite == NULL) return;
  free(sprite->map);
  free(sprite);
}

void move_sprite(Sprite *sprite) {

  if(check_borders_collisions(sprite)) {
  return;
  }

  sprite->x += sprite->xspeed;
  sprite->y += sprite->yspeed;
}


int draw_sprite(Sprite* sprite) {
    uint32_t* map_ptr = sprite->map;
    for(int rows = 0; rows < sprite->height; rows++) {
    for (int cols = 0; cols < sprite->width; cols++) {
      if(fill_pixel(sprite->x+cols, sprite->y+rows, *map_ptr) != 0) return 1;
      map_ptr++;
    }
  }

  return 0;
}

int draw_sprite_with_angle(Sprite* sprite, double angle) {

  uint32_t* map_ptr = sprite->map;

  double angle_rad = angle * M_PI / 180.0;  
  double cos_rad = cos(angle_rad);
  double sin_rad = sin(angle_rad);
  double x_center = sprite->width/2;
  double y_center = sprite->height/2;

    for(double rows = 0; rows < sprite->height; rows++) {
      for (double cols = 0; cols < sprite->width; cols++) {

        double x_rel_pos = cols-x_center;
        double y_rel_pos = rows-y_center;

        double x_rot = (x_rel_pos * cos_rad - y_rel_pos * sin_rad);
        double y_rot = (x_rel_pos * sin_rad + y_rel_pos * cos_rad);

        uint16_t x = round(sprite->x + x_rot + x_center);
        uint16_t y = round(sprite->y + y_rot + y_center);

        if(fill_pixel(x, y, *map_ptr) != 0) return 1;
        map_ptr++;
      }
    }

  return 0;
}



void set_x_sprite(Sprite* sprite, uint16_t x){
  sprite->x = x;
}


void set_y_sprite(Sprite* sprite, uint16_t y){
  sprite->y = y;
}


void initialize_sprites() {
 //cursor
  cursor = createCursor(pizacursor_xpm, 550, 425);

  //game
  background = create_sprite(darkbackground_xpm,0,0,0,0);
  gameName = create_sprite(agalag_xpm, 296,80,0,0);  /*559x265*/
  gameOver = create_sprite(gameOver_xpm, 376,300,0,0);  /*400x250*/
  sun = create_sprite(sun_xpm, 1000,0,0,0);  /*150x150*/

  //numbers
  numberArray[0] = create_sprite(number_0_xpm,800,650,0,0);
  numberArray[1] = create_sprite(number_1_xpm,numberArray[0]->x+numberArray[0]->width,650,0,0);
  numberArray[2] = create_sprite(number_2_xpm,numberArray[1]->x+numberArray[1]->width,650,0,0);
  numberArray[3] = create_sprite(number_3_xpm,numberArray[2]->x+numberArray[2]->width,650,0,0);
  numberArray[4] = create_sprite(number_4_xpm,numberArray[3]->x+numberArray[3]->width,650,0,0);
  numberArray[5] = create_sprite(number_5_xpm,numberArray[4]->x+numberArray[4]->width,650,0,0);
  numberArray[6] = create_sprite(number_6_xpm,numberArray[5]->x+numberArray[5]->width,650,0,0);
  numberArray[7] = create_sprite(number_7_xpm,numberArray[6]->x+numberArray[6]->width,650,0,0);
  numberArray[8] = create_sprite(number_8_xpm,numberArray[7]->x+numberArray[7]->width,650,0,0);
  numberArray[9] = create_sprite(number_9_xpm,numberArray[8]->x+numberArray[8]->width,650,0,0);

  twoDots = create_sprite(twoDots_xpm,800,650,0,0);
  slash= create_sprite(slash_xpm,800,650,0,0);
  secondS = create_sprite(s_xpm,0,0,0,0);
  points = create_sprite(points_xpm,0,0,0,0); /*70x20*/
  heart = create_sprite(heart_xpm,0,0,0,0);
  levelSprite = create_sprite(level_xpm,0,0,0,0);  /*70x20*/
  fireShots = create_sprite(shots_xpm,0,0,0,0);
  instructions = create_sprite(instructions_xpm,25,150,0,0);  /*1100x500*/

  player  = createPlayerShip(spaceship_2_xpm,600, 400);
  explosionPlayer = createExplosion(spaceship_explosion_1_xpm,spaceship_explosion_2_xpm,spaceship_explosion_3_xpm,spaceship_explosion_4_xpm, 600,600);
  
  //Enemies and their initial positions
  int jump = 100;
  int i=0, x=100, y=150;
  for (int r = 0; r < 4; r++) {
    for (int c=0; c<8; c=c+4) {
      x=x+jump;
      enemyArray[i] = createEnemy(enemy_1_1_xpm, enemy_1_2_xpm, x, y);
      x=x+jump;
      enemyArray[i + 1] = createEnemy(enemy_2_1_xpm, enemy_2_2_xpm, x, y);
      x=x+jump;
      enemyArray[i + 2] = createEnemy(enemy_3_1_xpm, enemy_3_2_xpm, x, y);
      x=x+jump;
      enemyArray[i + 3] = createEnemy(enemy_4_1_xpm, enemy_4_2_xpm, x, y);
      i =i+4;
    }
    x=100;
    y=y+jump;
  }

  explosionPlayer = createExplosion(spaceship_explosion_1_xpm,spaceship_explosion_2_xpm,spaceship_explosion_3_xpm,spaceship_explosion_4_xpm, 600,600);
  
  for(int i = 0; i < MAX_ENEMY; i++) {
    enemyExplosion[i] = createEnemyExplosion(enemy_explosion_1_xpm, enemy_explosion_2_xpm, enemy_explosion_3_xpm, enemy_explosion_4_xpm, enemy_explosion_5_xpm, 0, 0);
  }
  

  //fire
  for (int i = 0; i < MAX_FIRE; i++){
    fireArray[i] = createFire(fire_1_xpm,0,0,-5);
  }

  //enemy fire

  for(int i = 0; i < MAX_ENEMY_FIRE; i++) {
    enemyFireArray[i] = createEnemyFire(enemy_fire_1_xpm,0,0,5); 
  }
  //menu
  playButton = create_sprite(play_1_xpm, 100, 426, 0, 0);   //Size 414x159
  instructionsButton = create_sprite(instructions_1_xpm, 638, 426, 0, 0);  //Size 414x159
  leaderboardButton = create_sprite(leaderboard_1_no_bg_xpm, 100, 655, 0, 0);  //Size 414x159
  quitButton = create_sprite(quit_1_xpm, 638, 655 ,0,0);  //Size 414x159
  backButton = create_sprite(back1_xpm, 638, 655 ,0,0);  //Size 414x159

  playButton2 = create_sprite(play_2_xpm, 100, 426, 0, 0);   //Size 414x159
  instructionsButton2 = create_sprite(instructions_2_xpm, 638, 426, 0, 0);  //Size 414x159
  leaderboardButton2 = create_sprite(leaderboard_2_xpm, 100, 655, 0, 0);  //Size 414x159
  quitButton2 = create_sprite(quit_2_xpm, 638, 655 ,0,0);  //Size 414x159
  backButton2 = create_sprite(back2_xpm, 638, 655 ,0,0);  //Size 414x159

}


void destroy_sprites() {
  destroyCursor(cursor);

  destroy_sprite(background);
  destroy_sprite(gameName);
  destroy_sprite(gameOver);
  destroy_sprite(sun);

  for (int i=0; i<10; i++) {
    destroy_sprite(numberArray[i]);
  }
  destroy_sprite(twoDots);
  destroy_sprite(slash);


  destroy_sprite(secondS);
  destroy_sprite(points);
  destroy_sprite(heart);
  destroy_sprite(levelSprite);
  destroy_sprite(instructions);

  destroy_sprite(fireShots);

  destroyPlayerShip(player);

  for(int i = 0; i<MAX_FIRE; i++){
    destroyFire(fireArray[i]);
  }

  for(int i = 0; i < MAX_ENEMY_FIRE;i++){
    destroyEnemyFire(enemyFireArray[i]);
  }

   for(int i = 0; i<MAX_ENEMY; i++){
    destroyEnemy(enemyArray[i]);
    destroyEnemyExplosion(enemyExplosion[i]);
  }

  destroySpaceshipExplosion(explosionPlayer);



  destroy_sprite(quitButton);
  destroy_sprite(backButton);
  destroy_sprite(playButton);
  destroy_sprite(leaderboardButton);
  destroy_sprite(instructionsButton);
  destroy_sprite(quitButton2);
  destroy_sprite(playButton2);
  destroy_sprite(leaderboardButton2);
  destroy_sprite(instructionsButton2);

  destroy_sprite(quitButton2);
  destroy_sprite(backButton2);
  destroy_sprite(playButton2);
  destroy_sprite(leaderboardButton2);
  destroy_sprite(instructionsButton2);

}



/* ------------- Functions to draw different sized numbers ------------------------------------------*/


void draw_digit(int x, int y, int digit) {
    set_x_sprite(numberArray[digit], x);
    set_y_sprite(numberArray[digit], y);
    draw_sprite(numberArray[digit]);
}



void draw_two_digits(int twoDigitNumber, uint16_t xStart, uint16_t yStart, uint16_t digitSize) {
    int digit;

    //Tens
    digit = (twoDigitNumber / 10) %10;
    draw_digit(xStart, yStart, digit);
    xStart = xStart + digitSize;

    //Units
    digit = twoDigitNumber % 10;
    draw_digit(xStart, yStart, digit);
    xStart = xStart + digitSize;

}

void draw_three_digits(int threeDigitNumber, uint16_t xStart, uint16_t yStart, uint16_t digitSize) {
    int digit;

    // Hundreds
    digit = (threeDigitNumber / 100) % 10;
    draw_digit(xStart, yStart, digit);
    xStart = xStart + digitSize;

    // Tens
    digit = (threeDigitNumber / 10) % 10;
    draw_digit(xStart, yStart, digit);
    xStart = xStart + digitSize;

    // Units
    digit = threeDigitNumber % 10;
    draw_digit(xStart, yStart, digit);
    xStart = xStart + digitSize;
}

