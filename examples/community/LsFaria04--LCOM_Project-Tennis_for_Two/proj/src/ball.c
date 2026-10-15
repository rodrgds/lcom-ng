#include "ball.h"
#include "devices/video.h"
#include "color.h"

Ball *(createBall)(){
  Ball *ball = (Ball*)malloc(sizeof(Ball));

  if(ball == NULL){
    return NULL;
  }

  ball->x = 550;
  ball->y = 710;
  ball->xspeed = 3;
  ball->yspeed = 6;
  ball->incline = 0;
  ball->direction = UP_BALL;

  xpm_image_t img;
  Sprite *sprite = (Sprite*)malloc(sizeof(Sprite));

  sprite->map = (uint32_t *)malloc(sizeof(char*));

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) medium_ball_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  ball->medium = *sprite;
  ball->currentSprite = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) small_ball_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  ball->small = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) large_ball_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  ball->large = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) very_large_ball_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  ball->very_large = *sprite;

  return ball;
}

int (drawBall)(Ball *ball){
  int width = ball->currentSprite.width;
  int height = ball->currentSprite.height;
  uint32_t *map = ball->currentSprite.map;

  for(int j = ball->y; j < ball->y + height; j++){
    for(int i = ball->x; i < ball->x + width; i++){
      if(*map != TRANSPARENCY_COLOR){
        if(vg_draw_color(i,j,*map) != 0){
          return EXIT_FAILURE;
        }
      }
      map++;
    }
  }
  return EXIT_SUCCESS;
}

void (updateBallDirection)(Ball_direction direction, Ball *ball){
  ball->direction = direction;
}

void (collisionPlayer)(Ball *ball, Player *player){
  int x_min, x_max, y_min, y_max;
  get_current_hit_limits(player, &x_min, &x_max, &y_min, &y_max);

  if((ball->x + 25  >= x_min) && (ball->x + 4 <= x_max) && (ball->y + 30 >= y_min) && (ball->y + 4  <= y_max)){
    //there is a colision
    

    //change the ball direction
    if((ball->direction == UP_BALL) && (player->player_numb == PLAYER2)){
      ball->direction = DOWN_BALL;
    }
    else if((ball->direction == DOWN_BALL) && (player->player_numb == PLAYER1)){
      ball->direction = UP_BALL;
    }


    //the ball tragetory has only 3 modes for now (could be infinite in the future but will be more dificult to hit the ball in a specific direction)
    if((ball->x + 4 >= x_min - 5) && (ball->x + 4 < x_min + 30)){
      ball->incline = -1;
    }

    else if((ball->x + 4 >= x_min + 30) && (ball->x + 4 < x_min + 60)){
      ball->incline = 0;
    }

    else if((ball->x + 4 >= x_min + 60)){
      ball->incline = 1;
    }
  }
}

void (moveBall)(Ball *ball, bool slow){

  if(ball->incline < 0){
    ball->x -= ball->xspeed;

    if(ball->direction == UP_BALL){
      ball->y -= ball->yspeed * ball->incline * (-1);
    }
    else{
      ball->y += ball->yspeed * ball->incline * (-1) ;
    }

  }

  else if(ball->incline == 0){
    
    if(ball->direction == UP_BALL){
      if(slow){
        ball->y -= 1;
      }
      else{
        ball->y -= ball->yspeed;
      }
    }
    else{
      ball->y += ball->yspeed;
    }
  }

  else{
    ball->x += ball->xspeed;

    if(ball->direction == UP_BALL){
      ball->y -= ball->yspeed * ball->incline;
    }
    else{
      ball->y += ball->yspeed * ball->incline;
    }
  }


}

bool (checkCollisionLine)(Ball *ball, uint32_t *background){
  int width = ball->currentSprite.width;
  int height = ball->currentSprite.height;

  uint32_t *map = background;
  uint32_t *ballMap = ball->currentSprite.map;

  for(int j = ball->y; j < ball->y + height; j++){
    for(int i = ball->x; i < ball->x + width; i++){
      if((*(map + (j * get_hres() + i)) == ENDLINE_COLOR) && (*ballMap != TRANSPARENCY_COLOR)){
        return true;
      }
      ballMap++;
    }
  }

  return false;
}

void (resetBall)(Ball *ball, Player_numb scoredPlayer){
  switch (scoredPlayer)
  {
  case PLAYER1:
    ball->x = 550;
    ball->y = 710;
    ball->direction = UP_BALL;
    ball->incline = 0;
    break;
  
  case PLAYER2:
    ball->x = 500;
    ball->y = 100;
    ball->direction = DOWN_BALL;
    ball->incline = 0;

  default:
    break;
  }
}

void (destroyBall)(Ball *ball){
  if(ball == NULL){
    return;
  }

  if(ball->currentSprite.map != NULL){
    free(ball ->currentSprite.map);
  }

  if(ball->small.map != NULL){
    free(ball->small.map);
  }

  if(ball ->medium.map != NULL){
    free(ball->medium.map);
  }

  if(ball ->large.map != NULL){
    free(ball->large.map);
  }

  if(ball ->very_large.map != NULL){
    free(ball->very_large.map);
  }

  free(ball);
  ball = NULL;
}
