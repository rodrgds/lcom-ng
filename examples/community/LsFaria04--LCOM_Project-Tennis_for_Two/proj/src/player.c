#include "player.h"
#include "devices/video.h"
#include "color.h"
#include "devices/i8042.h"

Player *(createPlayer1)(){
  Player *player = (Player*)malloc(sizeof(Player));

  if( player == NULL){
    return NULL;
  }

  player->x = 500;
  player->y = 690;
  player->xspeed = 6;
  player->yspeed = 6;
  player->moveanim = 0;
  player->startanim = 0;
  player->hitanim = 0;
  player->player_numb = PLAYER1;
  player->movement = RIGHT_PLAYER;
  player->state = CHOOSE_START_STOP;

  xpm_image_t img;
  Sprite *sprite = (Sprite*)malloc(sizeof(Sprite));

  sprite->map = (uint32_t *)malloc(sizeof(char*));

  //load the move sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) move1_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->move[0] = *sprite;


  sprite->map = (uint32_t *) xpm_load((xpm_map_t) move2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->move[1] = *sprite;

  //load the move_rev sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) move1_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->moverev[0] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) move2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->moverev[1] = *sprite;

  //load the hit sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit1_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hit[0] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hit[1] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit3_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hit[2] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit4_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hit[3] = *sprite;

  //load the hitrev sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit1_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hitrev[0] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hitrev[1] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit3_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hitrev[2] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit4_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hitrev[3] = *sprite;

  //load the starting sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start1_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[0] = *sprite;

  player->currentSprite = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[1] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start3_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[2] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start4_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[3] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start5_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[4] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start6_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[5] = *sprite;

  //load the start rev sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start1_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[0] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[1] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start3_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[2] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start4_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[3] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start5_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[4] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start6_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[5] = *sprite;

  player->direction = RIGHTD;

  free(sprite);


  return player;
}

Player *(createPlayer2)(){
  Player *player = (Player*)malloc(sizeof(Player));

  if( player == NULL){
    return NULL;
  }

  player->x = 500;
  player->y = 350;
  player->xspeed = 3;
  player->yspeed = 3;
  player->moveanim = 0;
  player->startanim = 0;
  player->hitanim = 0;
  player->player_numb = PLAYER2;
  player->movement = RIGHT_PLAYER;
  player->state = STOP;

  xpm_image_t img;
  Sprite *sprite = (Sprite*)malloc(sizeof(Sprite));

  sprite->map = (uint32_t *)malloc(sizeof(char*));

  //load the move sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) move1_2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->move[0] = *sprite;


  sprite->map = (uint32_t *) xpm_load((xpm_map_t) move2_2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->move[1] = *sprite;

  //load the move_rev sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) move1_2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->moverev[0] = *sprite;

   player->currentSprite = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) move2_2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->moverev[1] = *sprite;

  //load the hit sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit1_2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hit[0] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit2_2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hit[1] = *sprite;


  //load the hitrev sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit1_2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hitrev[0] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) hit2_2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->hitrev[1] = *sprite;


  //load the starting sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start1_2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[0] = *sprite;

 

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start2_2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[1] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start3_2_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->start[2] = *sprite;


  //load the start rev sprites
  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start1_2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[0] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start2_2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[1] = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) start3_2_rev_xpm, XPM_8_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  player->startrev[2] = *sprite;

  player->direction = RIGHTD;

  free(sprite);


  return player;
}

int (drawPlayer)(Player *player){
  int width = player->currentSprite.width;
  int height = player->currentSprite.height;
  uint32_t *map = player->currentSprite.map;

  for(int j =  player->y ; j < player->y + height; j++){
    for(int i = player->x ; i < player->x + width; i++){
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

void (updatePlayerDirection)(Player_direction direction, Player *player){
  player->direction = direction;
}

void (changePlayerMovementKBD)(Player *player, uint8_t scancode){
  if((player->state == MOVE) || (player->state == STOP)){
    
    if((scancode == ARROW_LEFT) || (scancode == A_KEY)){
      updatePlayerDirection(LEFTD, player);
      player->state = MOVE;
      player->movement = LEFT_PLAYER;
    }

    else if((scancode == ARROW_RIGHT) || (scancode == D_KEY)){
      updatePlayerDirection(RIGHTD, player);
      player->state = MOVE;
      player->movement = RIGHT_PLAYER;
    }

    else if((scancode == ARROW_DOWN) || (scancode == S_KEY)){
      player->state = MOVE;
      player->movement = DOWN_PLAYER;
    }

    else if((scancode == ARROW_UP) || (scancode == W_KEY)){
      player->state = MOVE;
      player->movement = UP_PLAYER;
    }

    else if(stopPlayer(scancode, player->movement)){
      player->state = STOP;
    }

  }
  else if((player->state == CHOOSE_START) || (player->state == CHOOSE_START_STOP)){
    if((scancode == ARROW_LEFT) || (scancode == A_KEY)){
      updatePlayerDirection(LEFTD, player);
      player->movement = LEFT_PLAYER;
      player->state = CHOOSE_START;
    }

    else if((scancode == ARROW_RIGHT) || (scancode == D_KEY)){
      updatePlayerDirection(RIGHTD, player);
      player->movement = RIGHT_PLAYER;
      player->state = CHOOSE_START;
    }
    else{
      player->state = CHOOSE_START_STOP;
    }
  }
}

void (updatePlayerMovementsTimer)(Player *player, int counter, bool canHitAfterServe){

  switch (player->state)
  {

  case MOVE:

    //move the player and draws him in the new position
    movePlayer(player);

    

    if(counter % 2 == 0){
      moveAnim(player);
    }
    break;

  case HIT:

    if(counter % 4 == 0){
      hitAnim(player);

      //the animation ended
      if(player->hitanim == 0){
        player->state = STOP;
      }
    }
    break;

  case CHOOSE_START:


    chooseStartAnim(player);
    movePlayer(player);

    break;

  case START:

    if(player->player_numb == PLAYER2){
        if(counter % 6 == 0){
          startAnim(player);

          //the animation ended
          if(player->startanim == 0){
            player->state = STOP;
          }
      }
    }
    else{

       if(counter % 4 == 0){
        startAnim(player);

        //the animation ended
        if(player->startanim == 0){
          player->state = STOP;
        }
      }
    }
    break;
  default:

    break;
  }
}

void updatePlayerMovementMouse(Player *player, bool isLB, int *newBallX, bool canHitAfterServe) {
    if ((player->state == CHOOSE_START || player->state == CHOOSE_START_STOP) && !canHitAfterServe) {
        if (isLB) {
            player->state = START;

            // Calculate the initial position of the ball based on the player's chosen place to start
            *newBallX = player->direction == RIGHTD ? player->x + 70 : player->x + 10;
        }
    } else {
        if (isLB) {
            player->state = HIT;
        }
    }
}

bool (stopPlayer)(uint8_t scancode, Player_movement movement){
  switch (movement)
  {
  case UP_PLAYER:
    if((scancode == (KBD_BREAKCODE | ARROW_UP)) || (scancode == (KBD_BREAKCODE | W_KEY))){
      return true;
    }
    break;
  
  case DOWN_PLAYER:
    if((scancode == (KBD_BREAKCODE | ARROW_DOWN)) || (scancode == (KBD_BREAKCODE | S_KEY))){
      return true;
    }
    break;

  case RIGHT_PLAYER:
    if((scancode == (KBD_BREAKCODE | ARROW_RIGHT)) || (scancode == (KBD_BREAKCODE | D_KEY))){
      return true;
    }
    break;

  case LEFT_PLAYER:
    if((scancode & (KBD_BREAKCODE | ARROW_LEFT)) || (scancode & (KBD_BREAKCODE | A_KEY))){
      return true;
    }
    break;

  default:

    break;
  }

  return false;
}

void (get_current_hit_limits)(Player *player, int *x_min, int *x_max, int *y_min, int *y_max){
  switch (player->player_numb)
  {
  case PLAYER1:
      if(player->direction == RIGHT_PLAYER){
        *x_min = player->x + HIT_P1_X_MIN;
        *x_max = player->x + HIT_P1_X_MAX;
        *y_min = player->y + HIT_P1_Y_MIN;
        *y_max = player->y + HIT_P1_Y_MAX;
      }
      else{
        *x_min = player->x + HIT_REV_P1_X_MIN;
        *x_max = player->x + HIT_REV_P1_X_MAX;
        *y_min = player->y + HIT_P1_Y_MIN;
        *y_max = player->y + HIT_P1_Y_MAX;
      }
    break;
  
  case PLAYER2:
    if(player->direction == RIGHT_PLAYER){
        *x_min = player->x + HIT_REV_P2_X_MIN;
        *x_max = player->x + HIT_REV_P2_X_MAX;
        *y_min = player->y + HIT_P2_Y_MIN;
        *y_max = player->y + HIT_P2_Y_MAX;
      }
      else{
        *x_min = player->x + HIT_P2_X_MIN;
        *x_max = player->x + HIT_P2_X_MAX;
        *y_min = player->y + HIT_P2_Y_MIN;
        *y_max = player->y + HIT_P2_Y_MAX;
      }
    break;
  }
}

void (movePlayer)(Player *player){
  int new_x = player->x;
  int new_y = player->y;
  switch (player->movement)
  {
  //changes the player position accordingly to their movement
  case RIGHT_PLAYER:
    new_x += player->xspeed;
    break;
  case LEFT_PLAYER:
    new_x -= player->xspeed;
    break;
  case UP_PLAYER:
    new_y -= player->yspeed;
    break;
  case DOWN_PLAYER:
    new_y += player->yspeed;
    
    break;
  
  default:
    break;
  }

  
  if(player->player_numb == PLAYER1){
    //corrects the player position to avoid it from going out of the screen
    if (new_x < MIN_X) {
          new_x = MIN_X;
      } else if (new_x + player->currentSprite.width > MAX_X) {
          new_x = MAX_X - player->currentSprite.width;
      }
    if (new_y < MIN_Y) {
          new_y = MIN_Y;
      } else if (new_y + player->currentSprite.height > MAX_Y) {
          new_y = MAX_Y - player->currentSprite.height;
      }
    //avoids the player to go out of the field when he is choosing a place so start
    if((player->state == CHOOSE_START) || (player->state == CHOOSE_START_STOP)){
        if(new_x < 200){
          new_x = 200;
        }
        else if(new_x > 850){
          new_x = 850;
        }
    }
  }

  //the same but for the player2
  else{
    if (new_x < MIN_X_P2) {
     
          new_x = MIN_X_P2;
      } else if (new_x + player->currentSprite.width > MAX_X_P2) {
          new_x = MAX_X_P2 - player->currentSprite.width;
      }
    if (new_y < MIN_Y_P2) {
          new_y = MIN_Y_P2;
          
      } else if (new_y + player->currentSprite.height > MAX_Y_P2) {
          new_y = MAX_Y_P2 - player->currentSprite.height;
      }
      if((player->state == CHOOSE_START) || (player->state == CHOOSE_START_STOP)){
        if(new_x < 400){
          new_x = 400;
        }
        else if(new_x > 690){
          new_x = 690;
        }
    }
  }
    player->x = new_x;
    player->y = new_y;

}

void (moveAnim)(Player *player){
  unsigned int moveanim = player->moveanim;
  if(player->direction == RIGHTD){
    if(player->player_numb == PLAYER1){
      player->currentSprite = player->move[moveanim];
    }
    else{
      player->currentSprite = player->moverev[moveanim];
    }

  } else{
    if(player->player_numb == PLAYER1){
      player->currentSprite = player->moverev[moveanim];
    }
    else{
      player->currentSprite = player->move[moveanim];
    }
  }

  moveanim++;

  if(moveanim == 2){
    moveanim = 0;
  }

  player->moveanim = moveanim;
}

void(hitAnim)(Player *player){
  unsigned int hitanim = player->hitanim;
  if(player->direction == RIGHTD){
    if(player->player_numb == PLAYER1){
      player->currentSprite = player->hit[hitanim];
    }
    else{
      player->currentSprite = player->hitrev[hitanim];
    }
  } else{
    if(player->player_numb == PLAYER1){
      player->currentSprite = player->hitrev[hitanim];
    } else{
      player->currentSprite = player->hit[hitanim];
    }
  }

  hitanim++;

  if(player->player_numb == PLAYER1){
    if(hitanim == 4){
      hitanim = 0;
    }
  }
  else if(player->player_numb == PLAYER2){
    if(hitanim == 2){
      hitanim = 0;
    }
  }

  player->hitanim = hitanim;
}

void (startAnim)(Player *player){
  unsigned int startanim = player->startanim;
  if(player->direction == RIGHTD){
     if(player->player_numb == PLAYER1){
      player->currentSprite = player->start[startanim];
     }
     else{
      player->currentSprite = player->startrev[startanim];
     }
  } else{
     if(player->player_numb == PLAYER1){
      player->currentSprite = player->startrev[startanim];
     }
     else{
      player->currentSprite = player->start[startanim];
     }
  }

  startanim++;

  if(player->player_numb == PLAYER1){
    if(startanim == 6){
      startanim = 0;
    }
  }
  else if(player->player_numb == PLAYER2){
    if(startanim == 3){
      startanim = 0;
    }
  }

  player->startanim = startanim;
}

void (chooseStartAnim)(Player *player){
  if(player->direction == RIGHTD){
    if(player->player_numb == PLAYER1){
      player->currentSprite = player->start[0];
    }
    else{
      player->currentSprite = player->startrev[0];
    }
  }
  else{
    if(player->player_numb == PLAYER1){
      player->currentSprite = player->startrev[0];
    }
    else{
       player->currentSprite = player->start[0];
    }
  }
}

void (resetPlayer)(Player *player, bool hasScored){
  if(hasScored){
    player->state = CHOOSE_START_STOP;
    switch (player->player_numb)
    {
    case PLAYER1:
      player->x = 500;
      player->y = 690;
      player->currentSprite = player->start[0];
      break;

    case PLAYER2:
      player->x = 500;
      player->y = 100;
      player->currentSprite = player->start[0];
      break;
    
    default:
      break;
    }
  }

  else{

    player->state = STOP;
    switch (player->player_numb)
    {
    case PLAYER1:
      player->x = 500;
      player->y = 590;
      player->currentSprite = player->move[0];
      break;

    case PLAYER2:
      player->x = 500;
      player->y = 150;
      player->currentSprite = player->move[0];
      break;
    
    default:
      break;
    }

  }
}

void (destroyPlayer1)(Player *player){
  //if the player isn't player 1, changes function
  if(player->player_numb == PLAYER2){
    destroyPlayer2(player);
    return;
  }

  if(player == NULL){
    return;
  }

  //free the memory allocated for the currrent sprite
  if(player -> currentSprite.map){
    free(player -> currentSprite.map);
  }

  //free the allocated memory for the move arrays
  for(int i = 0; i < 2 ; i++){
    if(player -> move[i].map){
      free(player -> move[i].map);
    }
    if(player -> moverev[i].map){
      free(player -> moverev[i].map);
    }
  }

  //free the allocated memory for the hit arrays
  for(int i = 0; i < 4; i++){
    if(player -> hit[i].map){
      free(player ->hit[i].map);
    }
    if(player->hitrev[i].map){
      free(player->hitrev[i].map);
    }
  }

  //free the allocated memory for the start arrays
  for(int i = 0; i < 6; i++){
    if(player -> start[i].map){
      free(player ->start[i].map);
    }
    if(player ->startrev[i].map){
      free(player -> startrev[i].map);
    }
  }

  free(player->start);
  free(player->startrev);
  free(player->hit);
  free(player->hitrev);
  free(player->moverev);
  free(player->move);
  free(player);
  player = NULL;
}

void (destroyPlayer2)(Player * player){
  //if the player isn't player 2, changes function
  if(player->player_numb == PLAYER1){
    destroyPlayer1(player);
    return;
  }

  if(player == NULL){
    return;
  }

  //free the memory allocated for the currrent sprite
  if(player -> currentSprite.map){
    free(player -> currentSprite.map);
  }

  //free the allocated memory for the move arrays
  for(int i = 0; i < 2 ; i++){
    if(player -> move[i].map){
      free(player -> move[i].map);
    }
    if(player -> moverev[i].map){
      free(player -> moverev[i].map);
    }
  }

  //free the allocated memory for the hit arrays
  for(int i = 0; i < 2; i++){
    if(player -> hit[i].map){
      free(player ->hit[i].map);
    }
    if(player->hitrev[i].map){
      free(player->hitrev[i].map);
    }
  }

  //free the allocated memory for the start arrays
  for(int i = 0; i < 3; i++){
    if(player -> start[i].map){
      free(player ->start[i].map);
    }
    if(player ->startrev[i].map){
      free(player -> startrev[i].map);
    }
  }

  free(player->start);
  free(player->startrev);
  free(player->hit);
  free(player->hitrev);
  free(player->moverev);
  free(player->move);
  free(player);
  player = NULL;
}
